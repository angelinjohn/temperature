/*
make the connections without tx and rx first,then upload the code connect tx,rx and the reset arduino
Wiring - 

 LM35             Arduino
 Pin 1            5V
 Pin 2            A0
 Pin 3            GND
 
 ESP8266          Arduino
 CH_PD,VCC        3.3V
 GND              GND
 TX               RX (Arduino)
 RX               TX (Ardino)


 Written By
 Angelin John

 Last Update - 
 January 19, 2017
 
*/

int value=0;
float temp=0;

#define SSID "BSNL_bb" // type in your SSID
#define PASS "bsnl4924" // type in your password
#define IP "184.106.153.149"//Thingspeak IP Address,need not be changed
String GET = "GET /update?key=F51VSF3MUOE6UYHM&field1="; // instead of F51VSF3MUOE6UYHM, add the write API key of your channel.
int wifir=0;

void setup()
{
  Serial.begin(115200); //baud rate for ESP8266 and Serial Monitor
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    Serial.println("OK");
    connectWiFi();
    delay(1000);
  }
}

void loop(){
  
  //Get Temperature value and convert it to String
  
  value=analogRead(A0);
  temp=value*.48828125; // value*(5*100/1024) 
  String tempC=String(temp);// converting float value to String


  //Upload temperature value to thingspeak.com api
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";//set up TCP connection
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(1000);
    if(Serial.find("Error")){
      Serial.println("AT+CIPSTART Error");
      return;
    }
 
  cmd = GET;
  cmd += tempC;
  cmd += "\r\n\r\n";
  Serial.print("AT+CIPSEND=");//send TCP/IP data
  Serial.println(cmd.length());
  delay(1000);
    if(Serial.find(">")){ 
      Serial.print(cmd); 
    }
    else
      Serial.println("AT+CIPSEND error");
  delay(16000); // Thingspeak can update the value only at an interval of 15 secs.
}


//Connect to WiFi when Arduino starts up. One time run called during void setup().
int connectWiFi(){
  Serial.println("AT+CWMODE=3");//wifi mode
  delay(2000);
  String cmd="AT+CWJAP=\"";//join access point
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(15000); //it takes some time to connect to WiFi and get an IP address
}
