#include <ESP8266WiFi.h> // Used for the soft AP
#include <WiFiUdp.h> // used for UDP comms.
#include <Servo.h>

WiFiUDP Udp;
Servo servoLeftRight;          // Define left servo
Servo servoUpDown;         // Define right servo
//*** Soft Ap variables ***
const char *APssid = "Microsek-MCU";
const char *APpassword = "123456789"; // No password for the AP
IPAddress APlocal_IP(192, 168, 4, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);

//***UDP Variables***
unsigned int localUdpPort = 4210;
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
const int PACKET_SIZE = 256;
//***General Variables***
#define LEDpin  2
#define ServoLR_pin  5
#define ServoUP_pin  4
#define Pump_pin  14

int prev_S=128;

void setup() 
{
  
 Serial.begin(115200); //fire up the serial port
 pinMode(LEDpin,OUTPUT);
 pinMode(Pump_pin,OUTPUT);
 servoLeftRight.attach(ServoLR_pin);  // Set left servo to digital pin 10
 servoUpDown.attach(ServoUP_pin);  // Set right servo to digital pin 9
 
 digitalWrite(LEDpin,HIGH);
 digitalWrite(Pump_pin,HIGH);
 
 WiFi.mode(WIFI_AP);

  // Setup the UDP port
 Udp.begin(localUdpPort);
 
 digitalWrite(LEDpin,LOW);

}

void loop() 
{
  
  int rlen, Relay=0,UpDown=128,RL=128;

  while (1) 
  {
    rlen = Udp.parsePacket();
    if(rlen<10) 
    {
      delay(1);
      continue;
    }
    
    Udp.read(packetBuffer, (rlen > PACKET_SIZE) ? PACKET_SIZE : rlen);
    
    Relay=packetBuffer[1];//
    UpDown=packetBuffer[2];
    RL=packetBuffer[3];
    
    UpDown = map(UpDown, 0, 255, 170, 10);
    RL = map(RL, 0, 255, 170, 10);

    servoLeftRight.write(RL);
    servoUpDown.write(UpDown);

    if(Relay==64)
    {
      Serial.print("Pump ON");
      digitalWrite(Pump_pin,LOW);
    }
    else
    {
      Serial.print("Pump OFF");
      digitalWrite(Pump_pin,HIGH);
    }
   

  }
}
