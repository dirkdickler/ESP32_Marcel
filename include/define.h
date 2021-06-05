#ifndef __DEFINE_H
#define __DEFINE_H

typedef struct
{
	String Nazov;
	float Tset;
}ROOM_t;

//const int LedGreen = 26;
const int LedOrange = 21;
const int RS485_DirPin = 16;
#define   RS485_RxMode  digitalWrite (RS485_DirPin, LOW);
#define   RS485_TxMode  digitalWrite (RS485_DirPin, HIGH);

#define WDT_TIMEOUT 5
#define  firmwaree "1ddx2"

// Assign output variables to GPIO pins
const int Rele_K1 = 23;
const int Rele_K2 = 24;
const int Rele_K3 = 25;
const int Rele_K4 = 26;
const int Rele_K5 = 29;
const int Rele_K6 = 30;
const int Rele_K7 = 31;


//EEPROM adrese
#define EE_IPadresa 00				//16bytes
#define EE_SUBNET 16				//16bytes
#define EE_Brana 32					//16bytes
#define EE_NazovSiete 48			//16bytes
#define EE_Heslosiete 64			//20bytes
#define EE_citacZapisuDoEEPORM 84   //2bytes
#define EE_citac2_ZapisuDoEEPORM 86   //2bytes
#define EE_dalsi 88
#define EE_zacateKaret_1 200
#define EE_zacateKaret_2 1300   //EE_zacateKaret + 100*11tj 1300


#endif 