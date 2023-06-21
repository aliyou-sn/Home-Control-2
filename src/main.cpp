#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "Lights/light1.h"
#include "Lights/light2.h"
#include "Lights/light3.h"
#include "Lights/light4.h"
// #include "Lights/light5.h"
// #include "Lights/light6.h"
#include "Sirens/Siren1.h"
#include "weathers/Room1.h"
const char* ssid = "Aliyu";
const char* password = "macbooker";
#define timeSeconds 10

AsyncWebServer server(80);

void light1();
void light2();
void light3();
void light4();
// void light5();
// void light6();


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>NITHUB IOT ECOSYSTEM</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(h1);
  }
  else if(var == "HUMIDITY"){
    return String(t1);
  }
  return String();
}


unsigned long now = millis();
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  dht1.begin();
  pinMode(pir1, INPUT_PULLUP);
  pinMode(pir2, INPUT_PULLUP);
  pinMode(pir3, INPUT_PULLUP);
  pinMode(pir4, INPUT_PULLUP);
  // pinMode(pir5, INPUT_PULLUP);
  // pinMode(pir6, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pir1), detectsMovement1, RISING);
  attachInterrupt(digitalPinToInterrupt(pir2), detectsMovement2, RISING);
  attachInterrupt(digitalPinToInterrupt(pir3), detectsMovement3, RISING);
  attachInterrupt(digitalPinToInterrupt(pir4), detectsMovement4, RISING);
  // attachInterrupt(digitalPinToInterrupt(pir5), detectsMovement5, RISING);
  // attachInterrupt(digitalPinToInterrupt(pir6), detectsMovement6, RISING);
  xTaskCreate(readweather1,"Room 1",5000,NULL,4,NULL);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  // pinMode(L5, OUTPUT);
  // pinMode(L6, OUTPUT);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L3, LOW);
  digitalWrite(L4, LOW);
  // digitalWrite(L5, LOW);
  // digitalWrite(L6, LOW);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t1).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h1).c_str());
  });

  // Start server
  server.begin();
}

void loop() {
  now = millis();
  light1();
  light2();
  light3();
  light4();
  // light5();
  // light6();
  Siren1();
}
  //Light 1
  void light1(){
  if((digitalRead(L1) == HIGH) && (motion1 == false)) {
    motion1 = true;
  }
  if(startTimer1 && (now - lastTrigger1 > (timeSeconds * 1000))) {
    digitalWrite(L1, LOW);
    startTimer1 = false;
    motion1 = false;
  }
  }
  //Light 2
  void light2(){
  if((digitalRead(L2) == HIGH) && (motion2 == false)) {
    motion2 = true;
  }
  if(startTimer2 && (now - lastTrigger2 > (timeSeconds * 1000))) {
    digitalWrite(L2, LOW);
    startTimer2 = false;
    motion2 = false;
  }
  }

  //Light 3
  void light3(){
  if((digitalRead(L3) == HIGH) && (motion3 == false)) {
    motion3 = true;
  }
  if(startTimer3 && (now - lastTrigger3 > (timeSeconds * 1000))) {
    digitalWrite(L3, LOW);
    startTimer3 = false;
    motion3 = false;
  }
  }

  //Light 4
  void light4(){
  if((digitalRead(L4) == HIGH) && (motion4 == false)) {
    motion4 = true;
  }
  if(startTimer4 && (now - lastTrigger4 > (timeSeconds * 1000))) {
    digitalWrite(L4, LOW);
    startTimer4 = false;
    motion4 = false;
  }
  }

  // //Light 5
  // void light5(){
  // if((digitalRead(L5) == HIGH) && (motion5 == false)) {
  //   motion5 = true;
  // }
  // if(startTimer5 && (now - lastTrigger5 > (timeSeconds * 1000))) {
  //   digitalWrite(L5, LOW);
  //   startTimer5 = false;
  //   motion5 = false;
  // }
  // }

  // //Light 6
  // void light6(){
  // if((digitalRead(L6) == HIGH) && (motion6 == false)) {
  //   motion6 = true;
  // }
  // if(startTimer6 && (now - lastTrigger6 > (timeSeconds * 1000))) {
  //   digitalWrite(L6, LOW);
  //   startTimer6 = false;
  //   motion6 = false;
  // }
  // }
 