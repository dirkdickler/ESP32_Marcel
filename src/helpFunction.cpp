#include <Arduino.h>
#include <AsyncElegantOTA.h> //https://randomnerdtutorials.com/esp32-ota-over-the-air-arduino/#1-basic-elegantota
#include <elegantWebpage.h>
#include <Hash.h>
#include <Arduino_JSON.h>
#include <EEPROM.h>
#include "main.h" //kvolu u8,u16..
#include "Pin_assigment.h"
#include "index.html"
#include "constants.h"
#include "HelpFunction.h"

//toto spituje retezec napr cas  21:56, ti rozdeli podla delimetru ":"
char **split(char **argv, int *argc, char *string, const char delimiter, int allowempty)
{
	*argc = 0;
	do
	{
		if (*string && (*string != delimiter || allowempty))
		{
			argv[(*argc)++] = string;
		}
		while (*string && *string != delimiter)
			string++;
		if (*string)
			*string++ = 0;
		if (!allowempty)
			while (*string && *string == delimiter)
				string++;
	} while (*string);
	return argv;
}

String ConvetWeekDay_UStoCZ(tm *timeInfoPRT)
{
	char loc_buf[30];
	String ee;

	strftime(loc_buf, 30, " %A", timeInfoPRT);
	ee = String(ee + loc_buf);

	if (ee == " Monday")
	{
		ee = "Pondelí";
	}
	else if (ee == " Tuesday")
	{
		ee = "Úterý";
	}
	else if (ee == " Wednesday")
	{
		ee = "Středa";
	}
	else if (ee == " Thursday")
	{
		ee = "Štvrtek";
	}
	else if (ee == " Friday")
	{
		ee = "Pátek";
	}
	else if (ee == " Saturday")
	{
		ee = "Sobota";
	}
	else if (ee == " Sunday")
	{
		ee = "Nedele";
	}
	else
	{
		ee = "? den ?";
	}
	return ee;
}

String ConvetWeekDay_UStoSK(tm *timeInfoPRT)
{
	char loc_buf[30];
	String ee;

	strftime(loc_buf, 30, " %A", timeInfoPRT);
	ee = String(ee + loc_buf);

	if (ee == " Monday")
	{
		ee = "Pondelok";
	}
	else if (ee == " Tuesday")
	{
		ee = "Útorok";
	}
	else if (ee == " Wednesday")
	{
		ee = "Streda";
	}
	else if (ee == " Thursday")
	{
		ee = "Štvrtok";
	}
	else if (ee == " Friday")
	{
		ee = "Piatok";
	}
	else if (ee == " Saturday")
	{
		ee = "Sobota";
	}
	else if (ee == " Sunday")
	{
		ee = "Nedeľa";
	}
	else
	{
		ee = "? den ?";
	}
	return ee;
}

String ipToString(IPAddress ip)
{
	String s = "";
	for (int i = 0; i < 4; i++)
		s += i ? "." + String(ip[i]) : String(ip[i]);
	return s;
}

int getIpBlock(int index, String str)
{
	char separator = '.';
	int found = 0;
	int strIndex[] = {0, -1};
	int maxIndex = str.length() - 1;

	for (int i = 0; i <= maxIndex && found <= index; i++)
	{
		if (str.charAt(i) == separator || i == maxIndex)
		{
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}

	return found > index ? str.substring(strIndex[0], strIndex[1]).toInt() : 0;
}

IPAddress str2IP(String str)
{

	IPAddress ret(getIpBlock(0, str), getIpBlock(1, str), getIpBlock(2, str), getIpBlock(3, str));
	return ret;
}

//TODO tu pri tomto osoetri este ked nieje cas z WEBu tak sa to resetne
void OdosliCasDoWS(void)
{
	String DenvTyzdni = "! Čas nedostupný !";
	char loc_buf[60];
	DenvTyzdni = ConvetWeekDay_UStoSK(&MyRTC_cas);
	char hodiny[5];
	char minuty[5];
	strftime(loc_buf, sizeof(loc_buf), " %H:%M    %d.%m.%Y    ", &MyRTC_cas);

	ObjDatumCas["Cas"] = loc_buf + DenvTyzdni; // " 11:22 Stredaaaa";
	String jsonString = JSON.stringify(ObjDatumCas);
	Serial.print("[10sek] Odosielam strankam ws Cas:");
	Serial.println(jsonString);
	ws.textAll(jsonString);
}

bool SkontrolujCiJePovolenyDenvTyzdni(u8 Obraz, tm *timeInfoPRT)
{
	char loc_buf[30];
	String ee;

	strftime(loc_buf, 30, " %A", timeInfoPRT);
	ee = String(ee + loc_buf);
	u8 vv = 8;
	if (ee == " Monday")
	{
		vv = 0;
	}
	else if (ee == " Tuesday")
	{
		vv = 1;
	}
	else if (ee == " Wednesday")
	{
		vv = 2;
	}
	else if (ee == " Thursday")
	{
		vv = 3;
	}
	else if (ee == " Friday")
	{
		vv = 4;
	}
	else if (ee == " Saturday")
	{
		vv = 5;
	}
	else if (ee == " Sunday")
	{
		vv = 6;
	}

	Serial.print("Ventil ma nastaveny dni:");
	Serial.print(Obraz);
	Serial.print(" a RTC vidi den:");
	Serial.print(vv);

	if (Obraz & (1 << vv))
	{
		return true;
	}

	return false;
}

bool Input_digital_filtering(VSTUP_t *input_struct, uint16_t filterCas)
{
	if (digitalRead(input_struct->pin) == LOW)
	{
		//Serial.println("[Input filter] hlasi nula..");
		if (input_struct->filter < filterCas)
		{
			input_struct->filter++;
		}
		else
		{
			input_struct->input = 1;
		}
	}
	else
	{
		//Serial.println("[Input filter] hlasi jenda..");
		input_struct->input = 0;
		input_struct->filter = 0;
	}
	if (input_struct->input_prew != input_struct->input)
	{
		input_struct->input_prew = input_struct->input;
		return true;
	}
	else
	{
		return false;
	}
}

void ScanInputs(void)
{
	//Serial.println("[ScanInputs] begin..");
	bool bolaZmenaVstupu = false;

	for (u8 i = 0; i < pocetDIN; i++)
	{
		DIN[i].zmena = Input_digital_filtering(&DIN[i], filterTime_DI);
		if (DIN[i].zmena == true && DIN[i].input == true)
		{
			DIN[i].counter++;
		} //tu si incrementuju citac impulzu
		bolaZmenaVstupu |= DIN[i].zmena;
	}

	if (bolaZmenaVstupu == true)
	{
		Serial.print("[ScanInputs] hlasi ze mam zmenu na vstupoch....");
	}

	// for (u8 u = 0; u < pocetDIN; u++) //toto musi by tu na konci funkcie lebo to nastavi ze aktualny do predchoziho stavu
	// {
	// 	DIN[u].input_prew = DIN[u].input;
	// }
}

void System_init(void)
{
	Serial.print("[Func:System_init]  begin..");
	pinMode(RS485_DirPin, OUTPUT);
	pinMode(LedOrange, OUTPUT);
	RS485_RxMode;
	//Serial1.println("test RS485.. Begin");
	digitalWrite(LedOrange, LOW);
	rtc.setTime(30, 24, 8, 17, 1, 2021); // 17th Jan 2021 15:24:30
	Serial.print("[Func:System_init]  end..");
}

int8_t NacitajEEPROM_setting(void)
{
	if (!EEPROM.begin(500))
	{
		Serial.println("Failed to initialise EEPROM");
		return -1;
	}

	Serial.println("Succes to initialise EEPROM");

	EEPROM.readBytes(EE_NazovSiete, NazovSiete, 16);

	if (NazovSiete[0] != 0xff) //ak mas novy modul tak EEPROM vrati prazdne hodnoty, preto ich neprepisem z EEPROM, ale necham default
	{
		String apipch = EEPROM.readString(EE_IPadresa); // "192.168.1.11";
		local_IP = str2IP(apipch);

		apipch = EEPROM.readString(EE_SUBNET);
		subnet = str2IP(apipch);

		apipch = EEPROM.readString(EE_Brana);
		gateway = str2IP(apipch);

		memset(NazovSiete, 0, sizeof(NazovSiete));
		memset(Heslo, 0, sizeof(Heslo));
		u8_t dd = EEPROM.readBytes(EE_NazovSiete, NazovSiete, 16);
		u8_t ww = EEPROM.readBytes(EE_Heslosiete, Heslo, 20);
		Serial.printf("Nacitany nazov siete a heslo z EEPROM: %s  a %s\r\n", NazovSiete, Heslo);
		return 0;
	}
	else
	{
		Serial.println("EEPROM je este prazna, nachavma default hodnoty");
		sprintf(NazovSiete, "semiart");
		sprintf(Heslo, "aabbccddff");
		return 1;
	}
}

void RS485sendBuffer(u8 buffr[], u8 pocetDat, u8 *timeToRx)
{
	for (u8 i = 0; i < pocetDat; i++)
	{
		RS485_TxModee(timeToRx);
		Serial1.write(buffr[i]);
	}
}
//** ked Webstrenaky - jejich ws posle nejake data napriklad "VratMiCas" tj ze strnaky chcu RTC aby ich napriklad zobrazili
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
	AwsFrameInfo *info = (AwsFrameInfo *)arg;
	if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
	{
		data[len] = 0;

		if (strcmp((char *)data, "VratMiCas") == 0)
		{
			OdosliCasDoWS();

			//notifyClients();
		}
		else if (strcmp((char *)data, "VratNamerane_TaH") == 0)
		{
			Serial.println("stranky poslali: VratNamerane_TaH ");

			OdosliStrankeVytapeniData();
		}

		else if (strcmp((char *)data, "ZaluzieAllOpen") == 0)
		{
			//Send:02 43 64 00 02 00 0e 80 0e 00 00 00 b8
			u8 loc_buf[14] = {0x2, 0x43, 0x64, 0x0, 0xb, 0x0, 0x01, 0x80, 0x0c, 0x0, 0x5, 0x0, 0xb9};
			Serial.println("stranky poslali: ZaluzieVsetkyOtvor");

			RS485sendBuffer(loc_buf, 13, &RS485_toRx_timeout);

			Serial.print("[Func:RS485_TxModee]  timeout davam:");
			Serial.println(RS485_toRx_timeout);

			String rr = "[HndlWebSocket] To RS485 posielam OTVOR zaluzie\r\n";
			DebugMsgToWebSocket(rr);
		}
		else if (strcmp((char *)data, "ZaluzieAllStop") == 0)
		{
			//Send:02 43 64 00 02 00 0c 80 0c 00 00 00 bc
			u8 loc_buf[14] = {0x2, 0x43, 0x64, 0x0, 0xb, 0x0, 0x01, 0x80, 0x0e, 0x0, 0x5, 0x0, 0xb7};

			Serial.println("stranky poslali: ZaluzieAllStop ");

			RS485sendBuffer(loc_buf, 13, &RS485_toRx_timeout);

			Serial.print("[Func:RS485_TxModee]  timeout davam:");
			Serial.println(RS485_toRx_timeout);

			String rr = "[HndlWebSocket] To RS485 posielam STOP zaluzie\r\n";
			DebugMsgToWebSocket(rr);
		}

		else if (strcmp((char *)data, "ZaluzieAllClose") == 0)
		{
			//Send:02 43 64 00 02 00 0d 80 0d 00 00 00 ba
			u8 loc_buf[14] = {0x2, 0x43, 0x64, 0x0, 0xb, 0x0, 0x01, 0x80, 0x0d, 0x0, 0x5, 0x0, 0xb8};
			Serial.println("stranky poslali: ZaluzieVsetky zatvor");
			RS485sendBuffer(loc_buf, 13, &RS485_toRx_timeout);

			Serial.print("[Func:RS485_TxModee]  timeout davam:");
			Serial.println(RS485_toRx_timeout);

			String rr = "[HndlWebSocket] To RS485 posielam ZATVOR zaluzie\r\n";
			DebugMsgToWebSocket(rr);
		}
	}
}

void onEvent(AsyncWebSocket *server,
			 AsyncWebSocketClient *client,
			 AwsEventType type,
			 void *arg,
			 uint8_t *data,
			 size_t len)
{
	switch (type)
	{
	case WS_EVT_CONNECT:
		Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
		break;
	case WS_EVT_DISCONNECT:
		Serial.printf("WebSocket client #%u disconnected\n", client->id());
		break;
	case WS_EVT_DATA:
		handleWebSocketMessage(arg, data, len);
		break;
	case WS_EVT_PONG:
	case WS_EVT_ERROR:
		break;
	}
}

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600; //letny cas

void WiFi_init(void)
{
	WiFi.mode(WIFI_MODE_APSTA);
	Serial.println("Creating Accesspoint");
	WiFi.softAP(soft_ap_ssid, soft_ap_password, 7, 0, 3);
	Serial.print("IP address:\t");
	Serial.println(WiFi.softAPIP());

	if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
	{
		Serial.println("STA Failed to configure");
	}

	WiFi.begin(NazovSiete, Heslo);
	u8_t aa = 0;
	while (WiFi.waitForConnectResult() != WL_CONNECTED && aa < 2)
	{
		Serial.print(".");
		aa++;
	}
	// Print ESP Local IP Address
	Serial.println(WiFi.localIP());

	ws.onEvent(onEvent);	//initWebSocket();
	server.addHandler(&ws); //initWebSocket();

	FuncServer_On();

	AsyncElegantOTA.begin(&server, "qqq", "www"); // Start ElegantOTA
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	server.begin();
}

void WiFi_connect_sequencer(void) //vplas kazdych 10 sek loop
{
	static u8_t loc_cnt_10sek = 0;

	Serial.print("Wifi status:");
	Serial.println(WiFi.status());

	//https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
	if (WiFi.status() != WL_CONNECTED)
	{
		loc_cnt_10sek++;
		Internet_CasDostupny = false;
	}
	else
	{
		loc_cnt_10sek = 0;
		Serial.println("[10sek] Parada WIFI je Connect davam loc_cnt na Nula");

		//TODO ak je Wifi connect tak pocitam ze RTC cas bude OK este dorob
		Internet_CasDostupny = true;
		RTC_cas_OK = true;
	}

	if (loc_cnt_10sek == 2)
	{
		Serial.println("[10sek] Odpajam WIFI, lebo wifi nieje: WL_CONNECTED ");
		WiFi.disconnect(1, 1);
	}
	else if (loc_cnt_10sek == 3)
	{
		loc_cnt_10sek = 255;
		WiFi.mode(WIFI_MODE_APSTA);
		Serial.println("znovu -Creating Accesspoint");
		WiFi.softAP(soft_ap_ssid, soft_ap_password, 7, 0, 3);

		if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
		{
			Serial.println("STA Failed to configure");
		}
		Serial.println("znovu -Wifi begin");
		WiFi.begin(NazovSiete, Heslo);
		u8_t aa = 0;
		while (WiFi.waitForConnectResult() != WL_CONNECTED && aa < 2)
		{
			Serial.print(".");
			aa++;
		}
	}
}

String handle_LenZobraz_IP_setting(void)
{
	char ttt[1000];
	char NazovSiete[56] = {"nazovSiete\0"};
	char ippadresa[56];
	char maskaIP[56];
	char brana[56];

	EEPROM.readString(EE_NazovSiete, NazovSiete, 16);
	IPAddress ip = WiFi.localIP();
	String stt = ipToString(ip);
	stt.toCharArray(ippadresa, 16);
	stt = ipToString(WiFi.subnetMask());
	stt.toCharArray(maskaIP, 16);
	stt = ipToString(WiFi.gatewayIP());
	stt.toCharArray(brana, 16);

	Serial.print("\r\nVyparsrovane IP: ");
	Serial.print(ippadresa);
	Serial.print("\r\nVyparsrovane MASKa: ");
	Serial.print(maskaIP);
	Serial.print("\r\nVyparsrovane Brana: ");
	Serial.print(brana);

	sprintf(ttt, LenzobrazIP_html, ippadresa, maskaIP, brana, NazovSiete);
	return ttt;
	//server.send (200, "text/html", ttt);
}

String handle_Zadavanie_IP_setting(void)
{
	char ttt[1000];
	char NazovSiete[56] = {"nazovSiete\0"};
	char HesloSiete[56] = {"HesloSiete\0"};
	char ippadresa[56];
	char maskaIP[56];
	char brana[56];

	IPAddress ip = WiFi.localIP();
	String stt = ipToString(ip);
	stt.toCharArray(ippadresa, 16);
	stt = ipToString(WiFi.subnetMask());
	stt.toCharArray(maskaIP, 16);
	stt = ipToString(WiFi.gatewayIP());
	stt.toCharArray(brana, 16);

	EEPROM.readString(EE_NazovSiete, NazovSiete, 16);
	Serial.print("\r\nNazov siete: ");
	Serial.print(NazovSiete);

	EEPROM.readString(EE_Heslosiete, HesloSiete, 16);
	Serial.print("\r\nHeslo siete: ");
	Serial.print(HesloSiete);

	Serial.print("\r\nVyparsrovane IP: ");
	Serial.print(ippadresa);
	Serial.print("\r\nVyparsrovane MASKa: ");
	Serial.print(maskaIP);
	Serial.print("\r\nVyparsrovane Brana: ");
	Serial.print(brana);

	sprintf(ttt, zadavaci_html, ippadresa, maskaIP, brana, NazovSiete, HesloSiete);
	//Serial.print ("\r\nToto je bufer pre stranky:\r\n");
	//Serial.print(ttt);

	return ttt;
}

void handle_Nastaveni(AsyncWebServerRequest *request)
{
	String inputMessage;
	String inputParam;
	Serial.println("Mam tu nastaveni");

	if (request->hasParam("input1"))
	{
		inputMessage = request->getParam("input1")->value();
		if (inputMessage.length() < 16)
		{
			EEPROM.writeString(EE_IPadresa, inputMessage);
		}
	}

	if (request->hasParam("input2"))
	{
		inputMessage = request->getParam("input2")->value();
		if (inputMessage.length() < 16)
		{
			EEPROM.writeString(EE_SUBNET, inputMessage);
		}
	}

	if (request->hasParam("input3"))
	{
		inputMessage = request->getParam("input3")->value();
		if (inputMessage.length() < 16)
		{
			EEPROM.writeString(EE_Brana, inputMessage);
		}
	}

	if (request->hasParam("input4"))
	{
		inputMessage = request->getParam("input4")->value();
		if (inputMessage.length() < 16)
		{
			EEPROM.writeString(EE_NazovSiete, inputMessage);
		}
	}

	if (request->hasParam("input5"))
	{
		inputMessage = request->getParam("input5")->value();
		if (inputMessage.length() < 20)
		{
			EEPROM.writeString(EE_Heslosiete, inputMessage);
		}
	}

	EEPROM.commit();
}

void OdosliStrankeVytapeniData(void)
{
	ObjTopeni["tep1"] = room[0].T_vzduch;
	ObjTopeni["hum1"] = room[0].RH_vlhkkost;
	ObjTopeni["tep2"] = room[1].T_vzduch;
	ObjTopeni["hum2"] = room[1].RH_vlhkkost;
	ObjTopeni["tep3"] = room[2].T_vzduch;
	ObjTopeni["hum3"] = room[2].RH_vlhkkost;
	ObjTopeni["tep4"] = room[3].T_vzduch;
	ObjTopeni["hum4"] = room[3].RH_vlhkkost;
	ObjTopeni["tep5"] = room[4].T_vzduch;
	ObjTopeni["hum5"] = room[4].RH_vlhkkost;
	ObjTopeni["tep6"] = room[5].T_vzduch;
	ObjTopeni["hum6"] = room[5].RH_vlhkkost;
	ObjTopeni["tep7"] = room[6].T_vzduch;
	ObjTopeni["hum7"] = room[6].RH_vlhkkost;
	ObjTopeni["tep8"] = room[7].T_vzduch;
	ObjTopeni["hum8"] = room[7].RH_vlhkkost;
	ObjTopeni["tep9"] = room[8].T_vzduch;
	ObjTopeni["hum9"] = room[8].RH_vlhkkost;
	ObjTopeni["tep10"] = room[9].T_vzduch;
	ObjTopeni["hum10"] = room[9].RH_vlhkkost;
	ObjTopeni["tep11"] = room[10].T_vzduch;
	ObjTopeni["hum11"] = room[10].RH_vlhkkost;

	String jsonString = JSON.stringify(ObjTopeni);
	Serial.print("[ event -VratNamerane_TaH] Odosielam strankam ObjTopeni:");
	//Serial.println(jsonString);
	ws.textAll(jsonString);
}

uint8_t VypocitajSumuBuffera(uint8_t *buffer, uint16_t kolko)
{
	uint8_t suma = 0;

	for (uint16_t i = 0; i < kolko; i++)
	{
		suma += buffer[i];
	}

	suma = 255 - suma;
	return suma;
}

uint8_t KontrolaSumyBuffera(uint8_t *buffer, uint16_t kolko)
{
	uint8_t suma = 0;
	for (uint16_t i = 0; i < kolko; i++)
	{
		suma += buffer[i];
	}

	suma = 255 - suma;

	return suma;
}

void RS485_TxModee(u8 *timeout)
{
	*timeout = RS485_TimeOut;
	RS485_TxMode;
}