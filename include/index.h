/* sekcia pre UPDATE*/

//tu je link na ikony
//https://www.angularjswiki.com/fontawesome/weather/


const char zadavaci_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta charset="utf-8">
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<body>
  <h1>aDum systém IP konfigurácia</h1>
  <form action="/Nastaveni">
    IPadresa: <input type="text" value=%s max="16" name="input1"><br><br>
    Maska: <input type="text" value=%s max="16" name="input2"><br><br>
    GateWay : <input type="text" value=%s max="16" name="input3"><br><br>
    Nazev site: <input type="text" value=%s max="16" name="input4"><br><br>
    Heslo site: <input type="text" value=%s max="16" name="input5"><br><br>
    <input type="submit" value="Uloz">
  </form>
</body>

</html>)rawliteral";

const char LenzobrazIP_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta charset="utf-8">
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<body>
  <h1>aDum systém IP konfigurácia</h1>
  <form action="/Nastaveni">
    IPadresa: <input type="text" value=%s max="30" name="input1"><br><br>
    Maska: <input type="text" value=%s max="30" name="input2"><br><br>
    GateWay : <input type="text" value=%s max="30" name="input3"><br><br>
    Nazev site: <input type="text" value=%s max="30" name="input4"><br>
  </form>
</body>

</html>
)rawliteral";

const char Main[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta charset="utf-8">
  <title>aDum - hlavná stránka systému</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.9.0/css/all.css">
  </link>
</head>
<style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }

  h1 {
    font-size: 1.8rem;
    color: white;
  }

  h2 {
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }

  body {
    margin: 0;
  }

  .content {
    padding: 30px;
    max-width: 620px;
    margin: 0 auto;
  }

  .card {
    background-color: #F8F7F9;
    ;
    box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    padding-top: 10px;
    padding-bottom: 20px;
  }

  .button {
    padding: 15px 50px;
    font-size: 20px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 7px;
    width: 90%;
    height: 110px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
    word-wrap: break-word;
  }

  /*.button:hover {background-color: #0f8b8d}*/
  .button:active {
    background-color: #0f8b8d;
    box-shadow: 2 2px #CDCDCD;
    transform: translateY(2px);
  }

  .button1 {
    box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  }

  .state {
    font-size: 1.5rem;
    color: #8c8c8c;
    font-weight: bold;
  }
</style>



<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var PocetZon = 7;
  var x = 2;
  var a = 4.3;
  var i, hum = 66;
  var cnt = 0;

  setInterval(function () {
    //console.log('Bla timer bude posial Ajax');
    //getData();
    //document.getElementById('button0').innerHTML = cnt;
    cnt++;

    console.log('WebSocket page posila togle po 3 sekundos');
    websocket.send('VratMiCas');
    //websocket.send ('wsPoslal');
  }, 3000);



  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;   // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    websocket.send('VratMiCas');   //vypytam si cas
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }


  //toto sa vykona ked dojde sprava z ESP 
  function onMessage(event) {
    var state;
    var obj = JSON.parse(event.data);
    console.log("Event data dosli:" + event.data);
    //if (event.data == "1")
    //{
    //	state = "ON";
    //}


    if (obj.Cas) {
      console.log("Supper mam Cas");
      document.getElementById("ButtonSett_time").innerHTML = obj.Cas;    //event.data; //obj.hello;
      console.log("OjbCas vraca:" + obj.Cas);
    }
    if (obj.Citac) {
      console.log("Supper mam Citac");
      document.getElementById("ButtonSett_time").innerHTML = obj.Citac;
      console.log("myOjb vraca:" + obj.Citac);
    }
    else { console.log("!!! kurba JSON obj nenasel nic v prichod datach"); }
  }
  function onLoad(event) {
    console.log('Main Page - funkcia onLoad a nastavuje ');
    initWebSocket();

    //initButton ( );
  }


  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var obj = JSON.parse(this.responseText);
        console.log(this.responseText);
        document.getElementById("ButtonSett_time").innerHTML = obj.hello;
        console.log("myOjb vraca:" + obj.hello);
      }
    };
    xhttp.open("GET", "/get?param1=11&param2=22&param3=33&param4=33&param5=55", true);
    xhttp.send();
  }



  function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
      currentDate = Date.now();
    } while (currentDate - date < milliseconds);
  }


  function myFunction(rt) {
    console.log('Button vola myFuncion a param funkcie je :' + rt);
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        //document.getElementById("state").innerHTML = this.responseText;
        window.location.assign("/bla1");
      }
    };

    if (rt == 1) { window.location.assign("/vytapeni"); }
    else if (rt == 2) { window.location.assign("/zaluzie_Main"); }
    else if (rt == 3) { window.location.assign("/update"); }
    //xhttp.send();
    //sleep(500);
    //window.location.assign("/bla1");
  }

</script>

<body>
  <div class="topnav">
    <h1>aDum systém</h1>
    <p><span id="ButtonSett_time" class="fa fa-clock-o" style="color:white;"> --:--</span></p>
  </div>
  <div class="content">
    <div id="myCard" class="card">
      <!-- <p class="state">state: <span id="state">%STATE%</span></p> -->
      <!--<p><button onclick="myFunction(100)" class="button button1"> </p><p><i class="fa fa-clock-o"></i><span id="ButtonSett_time" >--:--</span></button></p>  -->
      <p><button onclick="myFunction(1)" id="button1" class="button button1"> </p>
      <p><i class="fa fa-thermometer-half"></i> Kúrenie</button></p>
      <p><button onclick="myFunction(2)" id="button1" class="button button1"> </p>
      <p><i class="fa fa-cloud-sun"></i> Žalúzie</button></p>
      <p><button onclick="myFunction(3)" id="button1" class="button button1"> </p>
      <p><i class="fa fa-lightbulb"></i> Svetlá</button></p>
      <p><button onclick="myFunction(100)" class="button button1"> </p>
      <p><i class="fa fa-cogs"></i> Nastavenie</button></p>
      <!-- <p></><input type="number" id="Hood" name="hodnota" style="font-size:35px; width:85px;" value=23 min="-10" max="30000" step="0.1"></p>  -->
    </div>
  </div>
</body>

</html>
)rawliteral";

const char vytapeni[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta charset="utf-8">
  <title>aDum kúrenie - hlavná stránka</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.9.0/css/all.css">
  </link>
</head>
<style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }

  h1 {
    font-size: 1.8rem;
    color: white;
  }

  h2 {
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }

  body {
    margin: 0;
  }

  .content {
    padding: 30px;
    max-width: 620px;
    margin: 0 auto;
  }

  .card {
    background-color: #F8F7F9;
    ;
    box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    padding-top: 10px;
    padding-bottom: 20px;
  }

  .button {
    padding: 15px 50px;
    font-size: 20px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 7px;
    width: 90%;
    height: 110px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
    word-wrap: break-word;
  }

  /*.button:hover {background-color: #0f8b8d}*/
  .button:active {
    background-color: #0f8b8d;
    box-shadow: 2 2px #CDCDCD;
    transform: translateY(2px);
  }

  .button1 {
    box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  }

  .state {
    font-size: 1.5rem;
    color: #8c8c8c;
    font-weight: bold;
  }
</style>

<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var PocetZon = 7;
  var x = 2;
  var a = 4.3;
  var i, hum = 66;
  var cnt = 0;


  setInterval(function () {
    //console.log('Bla timer bude posial Ajax');
    //getData();
    //document.getElementById('button0').innerHTML = cnt;
    cnt++;

    console.log('WebSocket page posila togle po 3 sekundos');
    websocket.send('VratMiCas');
    //websocket.send ('VratNamerane_TaH');
  }, 5000);




  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;   // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    websocket.send('VratMiCas');   //vypytam si cas
    websocket.send('VratNamerane_TaH');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }


  //toto sa vykona ked dojde sprava z ESP 
  function onMessage(event) {
    var state;
    var obj = JSON.parse(event.data);
    console.log("Event data dosli:" + event.data);
    //if (event.data == "1")
    //{
    //	state = "ON";
    //}


    if (obj.Cas) {
      console.log("Supper mam Cas");
      document.getElementById("ButtonSett_time").innerHTML = obj.Cas;     //event.data; //obj.hello;
      console.log("OjbCas vraca:" + obj.Cas);
    }
    if (obj.Citac) {
      console.log("Supper mam Citac");
      document.getElementById("ButtonSett_time").innerHTML = obj.Citac;
      console.log("myOjb vraca:" + obj.Citac);
    }

    if (obj.tep1) { document.getElementById("tep1").innerHTML = obj.tep1; }
    if (obj.hum1) { document.getElementById("hum1").innerHTML = obj.hum1; }
    if (obj.tep2) { document.getElementById("tep2").innerHTML = obj.tep2; }
    if (obj.hum2) { document.getElementById("hum2").innerHTML = obj.hum2; }
    if (obj.tep3) { document.getElementById("tep3").innerHTML = obj.tep3; }
    if (obj.hum3) { document.getElementById("hum3").innerHTML = obj.hum3; }
    if (obj.tep4) { document.getElementById("tep4").innerHTML = obj.tep4; }
    if (obj.hum4) { document.getElementById("hum4").innerHTML = obj.hum4; }
    if (obj.tep5) { document.getElementById("tep5").innerHTML = obj.tep5; }
    if (obj.hum5) { document.getElementById("hum5").innerHTML = obj.hum5; }
    if (obj.tep6) { document.getElementById("tep6").innerHTML = obj.tep6; }
    if (obj.hum6) { document.getElementById("hum6").innerHTML = obj.hum6; }
    if (obj.tep7) { document.getElementById("tep7").innerHTML = obj.tep7; }
    if (obj.hum7) { document.getElementById("hum7").innerHTML = obj.hum7; }
    if (obj.tep8) { document.getElementById("tep8").innerHTML = obj.tep8; }
    if (obj.hum8) { document.getElementById("hum8").innerHTML = obj.hum8; }
    if (obj.tep9) { document.getElementById("tep9").innerHTML = obj.tep9; }
    if (obj.hum9) { document.getElementById("hum9").innerHTML = obj.hum9; }
    if (obj.tep10) { document.getElementById("tep10").innerHTML = obj.tep10; }
    if (obj.hum10) { document.getElementById("hum10").innerHTML = obj.hum10; }
    //if (obj.tep11) 	 { 	 document.getElementById("tep11").innerHTML = obj.tep11;  }
    //if (obj.hum11) 	 { 	 document.getElementById("hum11").innerHTML = obj.hum11;  }


    else { console.log("!!! kurba JSON obj nenasel nic v prichod datach"); }
  }
  function onLoad(event) {
    console.log('Main Page - funkcia onLoad a nastavuje ');
    initWebSocket();
    //initButton ( );
  }

  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var obj = JSON.parse(this.responseText);
        console.log(this.responseText);
        document.getElementById("ButtonSett_time").innerHTML = obj.hello;
        console.log("myOjb vraca:" + obj.hello);
      }
    };
    xhttp.open("GET", "/get?param1=11&param2=22&param3=33&param4=33&param5=55", true);
    xhttp.send();
  }



  function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
      currentDate = Date.now();
    } while (currentDate - date < milliseconds);
  }


  function myFunction(rt) {
    console.log('Button vola myFuncion a param funkcie je :' + rt);

    if (rt == 1000) { document.getElementById('Hood').value = cnt; }
    else if (rt == 272) { document.getElementById('button0').innerHTML = rt; }
    else if (rt == 1) { xhttp.open("GET", "/setPage?param1=1", true); }
    else if (rt == 2) { xhttp.open("GET", "/setPage?param1=2", true); }
    else if (rt == 3) { xhttp.open("GET", "/setPage?param1=3", true); }
    else if (rt == 4) { xhttp.open("GET", "/setPage?param1=4", true); }
    else if (rt == 5) { xhttp.open("GET", "/setPage?param1=5", true); }
    else if (rt == 6) { xhttp.open("GET", "/setPage?param1=6", true); }
    else if (rt == 7) { xhttp.open("GET", "/setPage?param1=7", true); }
    else if (rt == 8) { xhttp.open("GET", "/setPage?param1=8", true); }
    else if (rt == 9) { xhttp.open("GET", "/setPage?param1=9", true); }
    else if (rt == 10) { xhttp.open("GET", "/setPage?param1=10", true); }
    else if (rt == 11) { xhttp.open("GET", "/setPage?param1=11", true); }
    else if (rt == 100) { window.location.assign("/"); }
  }
</script>

<body>
  <div class="topnav">
    <h1>aDum systém - kúrenie</h1>
    <p><span id="ButtonSett_time" class="fa fa-clock-o" style="color:white;"> --:--</span></p>
  </div>
  <div class="content">
    <div id="myCard" class="card">
      <!-- <p class="state">state: <span id="state">%STATE%</span></p> -->
      <p><button onclick="myFunction(1)" id="button0" class="button button1">Garáž</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep1">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum1">--.-</span>%</button></p>
      <p><button onclick="myFunction(2)" id="button1" class="button button1">Detská Timea</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep2">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum2">--.-</span>%</button></p>
      <p><button onclick="myFunction(3)" id="button2" class="button button1">Zádverie</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep3">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum3">--.-</span>%</button></p>
      <p><button onclick="myFunction(4)" id="button3" class="button button1">Jedáleň</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep4">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum4">--.-</span>%</button></p>
      <p><button onclick="myFunction(5)" id="button4" class="button button1">Kuchyň</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep5">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum5">--.-</span>%</button></p>
      <p><button onclick="myFunction(6)" id="button5" class="button button1">Obývacia miestnosť</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep6">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum6">--.-</span>%</button></p>
      <p><button onclick="myFunction(7)" id="button6" class="button button1">Chodba</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep7">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum7">--.-</span>%</button></p>
      <p><button onclick="myFunction(8)" id="button7" class="button button1">Detská Filip</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep8">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum8">--.-</span>%</button></p>
      <p><button onclick="myFunction(9)" id="button8" class="button button1">Spálňa</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep9">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span
          id="hum9">--.-</span>%</button></p>
      <p><button onclick="myFunction(10)" id="button9" class="button button1">Kúpeľňa</p>
      <p><i class="fa fa-thermometer-half"></i> <span id="tep10">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i>
        <span id="hum10">--.-</span>%</button>
      </p>
      <!--  <p><button onclick="myFunction(11)" id="button10" class="button button1">Ložnice</p><p><i class="fa fa-thermometer-half"></i> <span id="tep11">--.-</span>&deg;C &nbsp;<i class="fa fa-tint"></i> <span id="hum11">--.-</span>%</button></p>  -->

      <p><button onclick="myFunction(100)" id="param1" class="button button1">Návrat na hlavní stránku</p>
      <p><i class="fas fa-home"></i></button></p>
      <p>
        </><input type="number" id="Hood" name="hodnota" style="font-size:35px; width:85px;" value=23 min="-10"
          max="30000" step="0.1"></p>
    </div>
  </div>


</body>

</html>
)rawliteral";

//******************************************* zaluzie_Main ******************************************
const char zaluzie_Main[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta charset="utf-8">
  <title>aDum žaluzie - hlavná stránka</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.9.0/css/all.css">
  </link>
</head>
<style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }

  h1 {
    font-size: 1.8rem;
    color: white;
  }

  h2 {
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }

  body {
    margin: 0;
  }

  .content {
    padding: 30px;
    max-width: 620px;
    margin: 0 auto;
  }

  .card {
    background-color: #F8F7F9;
    ;
    box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    padding-top: 10px;
    padding-bottom: 20px;
  }

  .button {
    padding: 15px 50px;
    font-size: 23px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 7px;
    width: 90%;
    height: 110px;

  }

  .button_stin {
    box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  }


  .state {
    font-size: 1.5rem;
    color: #8c8c8c;
    font-weight: bold;
  }
</style>


<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var PocetZon = 7;
  var x = 2;
  var a = 4.3;
  var i, hum = 66;
  var cnt = 0;


  setInterval(function () {
    //console.log('Bla timer bude posial Ajax');
    //getData();
    //document.getElementById('button0').innerHTML = cnt;
    cnt++;

    console.log('WebSocket page posila togle po 3 sekundos');
    websocket.send('VratMiCas');
    //websocket.send ('VratNamerane_TaH');
  }, 5000);




  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;   // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    websocket.send('VratMiCas');   //vypytam si cas
    websocket.send('VratNamerane_TaH');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }


  //toto sa vykona ked dojde sprava z ESP 
  function onMessage(event) {
    var state;
    var obj = JSON.parse(event.data);
    console.log("Event data dosli:" + event.data);

    if (obj.Cas) {
      console.log("Supper mam Cas");
      document.getElementById("ButtonSett_time").innerHTML = obj.Cas;     //event.data; //obj.hello;
      console.log("OjbCas vraca:" + obj.Cas);
    }


    else { console.log("!!! kurba JSON obj nenasel nic v prichod datach"); }
  }
  function onLoad(event) {
    console.log('Main Page - funkcia onLoad a nastavuje ');
    initWebSocket();
    //initButton ( );
  }

  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
        var obj = JSON.parse(this.responseText);
        console.log(this.responseText);
        document.getElementById("ButtonSett_time").innerHTML = obj.hello;
        console.log("myOjb vraca:" + obj.hello);
      }
    };
    xhttp.open("GET", "/get?param1=11&param2=22&param3=33&param4=33&param5=55", true);
    xhttp.send();
  }



  function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
      currentDate = Date.now();
    } while (currentDate - date < milliseconds);
  }


  function myFunction(rt) {
    console.log('Button vola myFuncion a param funkcie je :' + rt);

    if (rt == 1) { console.log('My func 1'); debugger; websocket.send('ZaluzieOtvorVsetky'); }
    else if (rt == 2) { console.log('My func 2'); debugger; websocket.send('ZaluzieZatvorVsetky'); }
    else if (rt == 3) { console.log('My func 3'); debugger; window.location.assign("/zaluzie_room_01"); }
    else if (rt == 4) { window.location.assign("/zaluzie_room_02"); }
    else if (rt == 5) { window.location.assign("/zaluzie_room_03"); }
    else if (rt == 6) { window.location.assign("/zaluzie_room_04"); }
    else if (rt == 7) { window.location.assign("/zaluzie_room_05"); }
    else if (rt == 8) { window.location.assign("/zaluzie_room_06"); }
    else if (rt == 9) { window.location.assign("/zaluzie_room_07"); }
    else if (rt == 10) { window.location.assign("/zaluzie_room_08"); }
    else if (rt == 11) { window.location.assign("/zaluzie_room_09"); }
    else if (rt == 100) { window.location.assign("/"); }
  }
</script>

<body>
  <div class="topnav">
    <h1>aDum systém - žaluzie</h1>
    <p><span id="ButtonSett_time" class="fa fa-clock-o" style="color:white;"> --:--</span></p>
  </div>
  <div class="content">
    <div id="myCard" class="card">
      <p><button onclick="myFunction(1)" id="button1" class="button button_stin fa fa-arrow-alt-circle-up"> otvoriť
          všetky žaluzie</button></p>
      <p><button onclick="myFunction(2)" id="button2" class="button button_stin fa fa-arrow-alt-circle-down	"> zatvoriť
          všetky žaluzie</button></p>
      <p><button onclick="myFunction(3)" id="button3" class="button button_stin">Obývacia miestnosť</button></p>
      <p><button onclick="myFunction(4)" id="button4" class="button button_stin">Kuchyň</button></p>
      <p><button onclick="myFunction(5)" id="button5" class="button button_stin">Jedáleň</button></p>
      <p><button onclick="myFunction(6)" id="button6" class="button button_stin">Spálňa</button></p>
      <p><button onclick="myFunction(7)" id="button7" class="button button_stin">Detská Timea</button></p>
      <p><button onclick="myFunction(8)" id="button8" class="button button_stin">Detská Filip</button></p>
      <p><button onclick="myFunction(9)" id="button9" class="button button_stin">Kúpeľňa</button></p>
      <p><button onclick="myFunction(10)" id="button10" class="button button_stin">Garáž</button></p>
      <p><button onclick="myFunction(100)" id="param1" class="button button_stin">Návrat na hlavnú stránku</button></p>
    </div>
  </div>
</body>

</html>
)rawliteral";

const char Bla1[] = R"rawliteral(
<html>

<head>
  <meta charset="utf-8">
  <title>Untitled Page</title>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
</head>

<style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }

  h1 {
    font-size: 1.8rem;
    color: white;
  }

  h2 {
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }

  body {
    margin: 0;
  }

  .content {
    padding: 30px;
    max-width: 620px;
    margin: 0 auto;
  }

  div#container {
    width: 970px;
    position: relative;
    margin: 0 auto 0 auto;
    text-align: left;
  }


  .card {
    background-color: #F8F7F9;
    ;
    box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    padding-top: 20px;
    padding-bottom: 20px;
  }

  .Timebox {
    border: 1px solid #CCCCCC;
    border-radius: 4px;
    background-color: #FFFFFF;
    background-image: none;
    color: #000000;
    font-family: Arial;
    font-weight: normal;
    font-style: normal;
    font-size: 32px;
    line-height: 36px;
    padding: 4px 4px 4px 4px;
    margin: 0;
    text-align: center;
  }

  .Timebox:focus {
    border-color: #66AFE9;
    box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.075), 0px 0px 8px rgba(102, 175, 233, 0.60);
    outline: 0;
  }

  .tBox_temp {
    border: 1px solid #CCCCCC;
    border-radius: 4px;
    background-color: #FFFFFF;
    background-image: none;
    color: #000000;
    font-family: Arial;
    font-weight: normal;
    font-style: normal;
    font-size: 32px;
    line-height: 36px;
    padding: 4px 4px 4px 4px;
    margin: 0;
    text-align: center;
    width: 120px;
    height: 70px;
  }

  .tBox_temp:focus {
    border-color: #66AFE9;
    box-shadow: inset 0px 1px 1px rgba(0, 0, 0, 0.075), 0px 0px 8px rgba(102, 175, 233, 0.60);
    outline: 0;
  }

  .button {
    display: inline-block;
    padding: 9px 10px;
    font-size: 43px;
    cursor: pointer;
    text-align: center;
    text-decoration: none;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 15px;
    box-shadow: 0 9px #999;
    width: 60%;
    height: 125px;
  }

  .button:hover {
    background-color: #3e8e41
  }

  .button:active {
    background-color: #0f8b8d;
    box-shadow: 0 5px #666;
    transform: translateY(4px);
  }

  .button1 {
    box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  }
</style>


<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<script>

  window.addEventListener('load', onLoad);


  function onLoad(event) {
    console.log('Casovy harmos  - funkcia onLoad a nastavuje ');
  }

  function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
      currentDate = Date.now();
    } while (currentDate - date < milliseconds);
  }

  function myFunction(rt) {
    console.log("Button dva vola myFuncion a param funkcie je :" + rt);

    if (rt == 1) {
      var va1 = document.getElementById("Editbox01").value;
      var va2 = document.getElementById("Editbox02").value;
      var va3 = document.getElementById("Editbox03").value;
      var va4 = document.getElementById("Editbox04").value;
      var va5 = document.getElementById("Editbox05").value;
      var va6 = document.getElementById("Editbox06").value;
      var va7 = document.getElementById("Editbox07").value;
      var va8 = document.getElementById("Editbox08").value;
      var va9 = document.getElementById("Editbox09").value;
      var va10 = document.getElementById("tB_nazovMiesti").value;

      if (va1 > 30 || va1 < 10) { alert("Hodnota 1 musi byt v rozsahu 10 až 30 stupňov"); return; }

      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "/get?param1=" + va1 + "&param2=" + va2 + "&param3=" + va3 + "&param4=" + va4 + "&param5=" + va5 + "&param6=" + va6 + "&param7=" + va7 + "&param8=" + va8 + "&param9=" + va9 + "&param10=" + va10, true);
      xhttp.send();
    }
    else if (rt == 2) {
      window.location.assign("/vytapeni");
      /*
    var x = document.getElementById("spinerek");
      if (window.getComputedStyle(x).display === "none") { x.style.display = "block";}
      setInterval(function() { window.location.assign("/bla");  },1000) ;
  */
    }
  }
</script>

<body>
  <div class="topnav">
    <h1>{{LEDIO}}</h1>
  </div>
  <div class="content">


    <div id="Text1" style="font-size:30px;position:absolute;left:350px;top:90px;"><span>Harmonogram Od - Do</span></div>

    <div id="Text2" style="position:absolute;left:100px;top:220px;width:312px;height:30px;">
      <span style="font-size:30px;">Teplota (&deg;C) cez časový harmonogram</span>
    </div>
    <div id="Text3" style="position:absolute;left:570px;top:220px;width:315px;height:30px;">
      <span style="font-size:30px;">Teplota (&deg;C) mino časový harmonogram</span>
    </div>



    <div id="Tx4" style="font-size:30px;position:absolute;left:160px;top:340px;"><span>Pondelok</span></div>
    <div id="Tx5" style="font-size:30px;position:absolute;left:160px;top:420px;"><span>Útorok</span></div>
    <div id="Tx6" style="font-size:30px;position:absolute;left:160px;top:500px;"><span>Streda</span></div>
    <div id="Tx7" style="font-size:30px;position:absolute;left:160px;top:580px;"><span>Štvrtok</span></div>
    <div id="Tx8" style="font-size:30px;position:absolute;left:160px;top:660px;"><span>Piatok</span></div>
    <div id="Tx9" style="font-size:30px;position:absolute;left:160px;top:740px;"><span>Sobota</span></div>
    <div id="Tx10" style="font-size:30px;position:absolute;left:160px;top:820px;"><span>Nedeľa</span></div>


    <input type="time" id="Editbox8" class="Timebox" style="position:absolute;left:270px;top:130px;" value="01:12">
    <input type="time" id="Editbox8" class="Timebox" style="position:absolute;left:510px;top:130px;" Value="01:12">


    <input type="number" id="Editbox01" class="tBox_temp" style="position:absolute;left:315px;top:320px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox02" class="tBox_temp" style="position:absolute;left:315px;top:400px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox03" class="tBox_temp" style="position:absolute;left:315px;top:480px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox04" class="tBox_temp" style="position:absolute;left:315px;top:560px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox05" class="tBox_temp" style="position:absolute;left:315px;top:640px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox06" class="tBox_temp" style="position:absolute;left:315px;top:720px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox07" class="tBox_temp" style="position:absolute;left:315px;top:800px;" value=11.2
      min="10" max="30" step="0.1">

    <div style="position:absolute;left:490px;top:200px; border-left: 4px solid #143642;height: 700px;"></div>
    <div style="position:absolute;left:80px;top:200px; border-left: 800px solid #143642;height: 4px;"></div>
    <div style="position:absolute;left:80px;top:300px; border-left: 800px solid #143642;height: 4px;"></div>

    <input type="number" id="Editbox08" class="tBox_temp" style="position:absolute;left:550px;top:320px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox09" class="tBox_temp" style="position:absolute;left:550px;top:400px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox10" class="tBox_temp" style="position:absolute;left:550px;top:480px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox11" class="tBox_temp" style="position:absolute;left:550px;top:560px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox12" class="tBox_temp" style="position:absolute;left:550px;top:640px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox13" class="tBox_temp" style="position:absolute;left:550px;top:720px;" value=11.2
      min="10" max="30" step="0.1">
    <input type="number" id="Editbox14" class="tBox_temp" style="position:absolute;left:550px;top:800px;" value=11.2
      min="10" max="30" step="0.1">

    <div id="Tx11" style="position:absolute;left:50px;top:920px;">
      <span style="font-size:25px;">Názov miestnosti (max 25 znakov):</span>
    </div>
    <input type="text" id="tB_nazovMiesti"
      style="font-size:40px; position:absolute;left:50px;top:950px;width:600px;height:70px;" value="{{LEDIO}}"
      maxlength="25">

    <p><button onclick="myFunction(1)" class="button button1" id="param1"
        style="position:absolute;left:50px;top:1100px;">Uložiť nastavenie</button></p>
    <p><button onclick="myFunction(2)" class="button button1" id="param2"
        style="position:absolute;left:50px;top:1300px;">Návrat zpět na vytápění</button></p>
    <i id="spinerek" class="fa fa-spinner fa-spin"
      style="display:none; position:absolute;left:320px;top:600px;width:300px;height:400px; font-size:200px;"></i>

  </div>
</body>

</html>
)rawliteral";

const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
</head>

<style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }

  h1 {
    font-size: 1.8rem;
    color: white;
  }

  h2 {
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }

  body {
    margin: 0;
  }

  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }

  .card {
    background-color: #F8F7F9;
    ;
    box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    padding-top: 10px;
    padding-bottom: 20px;
  }

  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
  }

  /*.button:hover {background-color: #0f8b8d}*/
  .button:active {
    background-color: #0f8b8d;
    box-shadow: 2 2px #CDCDCD;
    transform: translateY(2px);
  }

  .state {
    font-size: 1.5rem;
    color: #8c8c8c;
    font-weight: bold;
  }
</style>

<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 2</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
  <script>
    var gateway = `ws://${window.location.hostname}/ws`;
    var websocket;
    window.addEventListener('load', onLoad);
    function initWebSocket() {
      console.log('Trying to open a WebSocket connection...');
      websocket = new WebSocket(gateway);
      websocket.onopen = onOpen;
      websocket.onclose = onClose;
      websocket.onmessage = onMessage; // <-- add this line
    }
    function onOpen(event) {
      console.log('Connection opened');
    }
    function onClose(event) {
      console.log('Connection closed');
      setTimeout(initWebSocket, 2000);
    }
    function onMessage(event) {
      var state;
      if (event.data == "1") {
        state = "ON";
      }
      else {
        state = "OFF";
      }
      document.getElementById('state').innerHTML = state;
    }
    function onLoad(event) {
      initWebSocket();
      initButton();
    }
    function initButton() {
      document.getElementById('button').addEventListener('click', toggle);
    }
    function toggle() {
      websocket.send('toggle');
    }
  </script>
</body>

</html>
)rawliteral";


const char DebugLog_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
  <meta charset="utf-8">
  <title>aDum Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.9.0/css/all.css">
  </link>
</head>

<style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }

  h1 {
    font-size: 1.8rem;
    color: white;
  }

  h2 {
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }

  .topnav {
    overflow: hidden;
    background-color: #143642;
  }

  body {
    margin: 0;
  }

  .content {
    padding: 30px;
    max-width: 950px;
    margin: 0 auto;
  }

  .card {
    background-color: #F8F7F9;
    ;
    box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
    padding-top: 10px;
    padding-bottom: 20px;
  }

  .button {
    padding: 15px 50px;
    font-size: 20px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 7px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
    word-wrap: break-word;
  }

  /*.button:hover {background-color: #0f8b8d}*/
  .button:hover,
  .button:focus,
  .button:active {
    background-color: #0f8b8d;
    box-shadow: 2 2px #CDCDCD;
    transform: translateY(10px);
    background: olive;
  }

  .button1 {
    box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  }

  .state {
    font-size: 1.5rem;
    color: #8c8c8c;
    font-weight: bold;
  }
</style>

<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var LogEnable = true;

  setInterval(function () {
    console.log('Timer loop...');
  }, 10000);


  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;   // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
    websocket.send('VratMiCas');   //vypytam si cas
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }

  function onLoad(event) {
    console.log('PageNastevVentil - funkcia onLoad a nastavuje ');
    initWebSocket();
    //document.getElementById("ElemTime").value = VentilJson.CasOn;
  }

  //toto sa vykona ked dojde sprava z ESP 
  function onMessage(event) {
    var state;
    var obj = JSON.parse(event.data);
    console.log("Event data dosli:" + event.data);

    //if (event.data == "1") { state = "ON";}
    if (obj.DebugMsg) {
      if (LogEnable == true) {
        document.getElementById("LogWindow").value += obj.DebugMsg;
        logTa = document.getElementById("LogWindow")
        logTa.scrollTop = logTa.scrollHeight;
      }
    }
  }


  function myFunction(rt) {
    console.log('Button vola myFuncion a param funkcie je :' + rt);

    if (rt == 1) {
      document.getElementById("LogWindow").value = "";
    }
    else if (rt == 2) {
      if (LogEnable == false) {
        console.log('Log enble davam TRUE');
        LogEnable = true;
        document.getElementById("LogWindow").value += "[Interna hlaska stranek] Povolujem  vypis logu.....:-)\r\n";
      }
      else {
        console.log('Log enble davam FALSE');
        LogEnable = false;
        document.getElementById("LogWindow").value += "[Interna hlaska stranek] Zastavujem vypis logu........!!!!\r\n";
      }
    }
  }
</script>

<body>
  <div class="topnav">
    <h1>aDum LOG výpise</h1>
  </div>
  <div class="content">
    <div id="myCard" class="card">
      <!-- <p class="state">state: <span id="state">%STATE%</span></p> -->

      <textarea id="LogWindow" name="LogWindow" rows="50" cols="80">Tu sa bude vypispvat debug log texty.</textarea>


      <p><button onclick="myFunction(1)" id="ClearText" class="button button1">Vymaž okno</button></p>
      <p><button onclick="myFunction(2)" id="Stoplog" class="button button1">Stop/Start log</button></p>
      <!-- <p><button onclick="myFunction(272)" id="param1" class="button button1">Pokoj pro hosty</p><p><i class="fa fa-thermometer-half"></i> 23.5&deg;C &nbsp;<i class="fa fa-tint"></i> 58%</button></p>
           <p></><input type="number" id="Hood" name="hodnota" style="font-size:35px; width:85px;" value=23 min="-10" max="30000" step="0.1"></p> -->
    </div>
  </div>


</body>

</html>
)rawliteral";