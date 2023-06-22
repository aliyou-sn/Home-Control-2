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

#define timeSeconds 10
#define NUM_RELAYS  4
#define RELAY_NO    false


const char* ssid = "Now";
const char* password = "123456789";
const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";

AsyncWebServer server(80);

int relayGPIOs[NUM_RELAYS] = {L1, L2, L3, L4};



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
  <link href="https://fonts.googleapis.com/css?family=Varela Round" rel="stylesheet">
  <title>Temperature and Humidity</title>
  <link href="https://zippy-pie-5f40f4.netlify.app/style.css" rel="stylesheet">
</head>
<body class="colorLight">
  <img class ="logo" src="https://nithub.unilag.edu.ng/wp-content/uploads/2022/01/Nithub_Nithub-Dark-Blue.png" height="40px" width="112px"/>
  <h2>NITHUB IOT <strong>ECOSYSTEM</strong></h2>
  <div class="temp">
    <div class="icon-div">
      <i class="fas fa-thermometer-half" style="color:rgba(0, 0, 200, 0.6);"></i>
      <span class="dht-labels">Temperature</span> 
    </div>
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </div>
  <div class="humi">
    <div class="icon-div">
      <i class="fas fa-tint" style="color:rgba(0, 0, 200, 0.6);"></i> 
      <span class="dht-labels">Humidity</span>
    </div>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </div>
  <div class = "circle"></div>
  <button class="buttonLight" type="button" class ="btn"></button>
</body>
<script>
  let theme = true;
  const body = document.querySelector('body');
  const icon = document.querySelectorAll('i');
  const image = document.querySelector('img');
  function dark(){
    body.classList.add('colorDark');
    body.classList.remove('colorLight');
    icon[0].style.color = "rgba(78, 27, 196, 0.8)";
    icon[1].style.color = "rgba(78, 27, 196, 0.8)";
    image.src = "https://nithub.unilag.edu.ng/wp-content/uploads/2022/01/Nithub_Nithub-White-300x108.png";
  }
  function light(){
    body.classList.remove('colorDark');
    body.classList.add('colorLight');
    icon[0].style.color = "rgba(0, 0, 200, 0.6)";
    icon[1].style.color = "rgba(0, 0, 200, 0.6)";
    image.src = "https://nithub.unilag.edu.ng/wp-content/uploads/2022/01/Nithub_Nithub-Dark-Blue.png";
  }
  
  document.querySelector("button").addEventListener('click', function(){
    document.querySelector("button").classList.toggle('buttonDark');
    document.querySelector("button").classList.toggle('buttonLight');
    if(theme){
      dark();
      theme = false;
    }else{
      light();
      theme = true;
    }
  })

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
</html>
)rawliteral";

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}
// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(h1);
  }
  else if(var == "HUMIDITY"){
    return String(t1);
  }
  else if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    for(int i=1; i<=NUM_RELAYS; i++){
      String relayStateValue = relayState(i);
      buttons+= "<h4> Light " + String(i) + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
    }
    return buttons;
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

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    // Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
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
 