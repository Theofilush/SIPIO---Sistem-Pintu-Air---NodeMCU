#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>  
#include <ThingSpeak.h>;
#include <ESP8266HTTPClient.h>
#define DestinationIP "184.106.153.149" // ip web thingspeak.com
#define TS_Key "G49O6A65K0OUZXHV" // api key dari thingspeak.com buat chanel untuk mendapatkanya
//#include <LiquidCrystal.h>
//#include <WiFiManager.h>
// Setting LCD RS E D4 D5 D6 D7
//LiquidCrystal lcd(D3, D3, D4, D5, D6, D6);
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
const int trigPin = D0;
const int echoPin = D1;
 
const char *ssid = "cahayaDev"; //ganti nama wifi
const char *pass = "gamegame";//ganti password
const char* host = "http://35.184.2.246";
String get_host = "http://35.184.2.246";

long durasi;
int jarak ;
int jarakku ;
int i;
int LEDbiru = D8;//lampu led merah
int LEDmerah = D7;//lampu led kuning
String cmd;
char c;
Servo myservo;
Servo myservo2;
WiFiServer server(80);  // open port 80 for server connection
WiFiClient client;

unsigned long myChannelNumber = 774760; //Your Channel Number (Without Brackets) 
const char * myWriteAPIKey = "G49O6A65K0OUZXHV"; //Your Write API Key

void setup() {
  myservo.attach(D2);
  myservo.write(0);
  myservo2.attach(D5);
  myservo2.write(0);
  delay(100);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDbiru, OUTPUT);
  pinMode(LEDmerah, OUTPUT);
  pinMode(LEDbiru, OUTPUT);
  //Serial.begin(9600);
   Wire.begin(D3,D4);
   lcd.begin();
   lcd.print("Tinggi Air:");
 
//WiFiManager wifiManager;
   Serial.begin(9600);
    delay(10);
   
    Serial.print(" Menghubungkan ke : ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    //wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");
 server.begin();
   
    while (WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print("….");
    }
    Serial.print("\n");
    Serial.print("IP address : ");
    Serial.print(WiFi.localIP());
    Serial.print("\n");
    Serial.print("MAC : ");
    Serial.println(WiFi.macAddress());
    Serial.println("");
    Serial.print("Terhubung dengan : ");
    Serial.println(ssid);
    ThingSpeak.begin(client);
}
 
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  durasi = pulseIn(echoPin, HIGH);
  jarak = durasi*0.034/2;
 
  lcd.setCursor(14, 0);
  lcd.print(" M");
  lcd.setCursor(13, 0);
  lcd.print(18-jarak);
 
  lcd.setCursor(2, 1);
  lcd.print("SIPIO-TIF '16");
        
        WiFiClient client = server.available();
        HTTPClient http;
        String url = get_host+"/sipio/get_status.php";        
        http.begin(url);
        int httpCode = http.GET();
        String status_db = http.getString();
        if(status_db=="terbuka"){
              digitalWrite(LEDbiru, LOW);          
              myservo2.write(180);
              for(i=1;i<=2;i++){
                digitalWrite(LEDmerah, HIGH);
                delay(250);
                digitalWrite(LEDmerah, LOW);
                delay(250);
              } 
        }
        else if(status_db=="tertutup"){ 
          digitalWrite(LEDmerah, LOW);
          digitalWrite(LEDbiru, HIGH);      
          myservo2.write(0); 
        }
        http.end();
  if(jarak <= 10){ //buka pintu
          digitalWrite(LEDbiru, LOW);          
          myservo.write(180);
              for(i=1;i<=2;i++){
                digitalWrite(LEDmerah, HIGH);
                delay(250);
                digitalWrite(LEDmerah, LOW);
                delay(250);
                }                    
    }
    else if(jarak >= 12 ){//tutup pintu
      digitalWrite(LEDmerah, LOW);
      digitalWrite(LEDbiru, HIGH);      
      myservo.write(0);          
     }
     jarakku=18-jarak;
   //kirim_thingspeak();
   ThingSpeak.writeField(myChannelNumber, 1,jarakku, myWriteAPIKey);//Update in ThingSpeak
}
/*void kirim_thingspeak(){
    cmd = "AT+CIPSTART=\"TCP","";
    cmd += DestinationIP ;
    cmd += "\",80";
    Serial.println(cmd);
    Serial.println(cmd);
    if(Serial.find("Error")){ 
      Serial.println("Koneksi error.");
      return;
    }
    
    cmd = "GET /update?key=";
    cmd += TS_Key;
    cmd +="&field1="; 
    cmd += jarak;
    cmd += "rn";        // jangan lupa, setiap perintah selalu diakhiri dengan CR+LF
    Serial.print("AT+CIPSEND=");
    Serial.println(cmd.length());
    Serial.println(cmd);
    if(Serial.find(">")){
      Serial.print(">");
    }
    else{
      Serial.println("AT+CIPCLOSE");
      Serial.println("koneksi timeout");
      delay(1000);
      return;
    }
    Serial.print(cmd);
    delay(2000);
    while (Serial.available()){
      c = Serial.read();
      Serial.write(c);
      if(c=='r') Serial.print('n');
    }
    Serial.println("——end");
    delay(10000);
  }
  */
