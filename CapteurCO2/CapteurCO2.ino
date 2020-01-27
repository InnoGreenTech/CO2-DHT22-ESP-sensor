/* library system*/
#include <SPI.h>
#include <stdio.h>
#include <WString.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DHT.h>

/* use to enable sleep mode */
#include <ArduinoJson.h>
#include <Wire.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <wchar.h>

extern "C" {
#include "user_interface.h"
}

#include <Modbus.h>
#include <ModbusIP_ESP8266.h>

//Modbus Registers Offsets (0-9999)
const int HIGH_WORD = 101;
const int LOW_WORD = 100;
//ModbusIP object
ModbusIP mb;

long ts;

/* configuration eeprom memory */

#define NAME_MODULE     0
#define ADRESS_SERVER   20
#define PORT_SERVER     40
#define PERIOD          44
#define MEM_SSID        46
#define MEM_PASSWORD    76
#define ALARME_EEPROM  100


#define TEMPERATURE_MAX     102    //Temperature maximum
#define TEMPERATURE_MIN     104    //Température minimum
#define TEMPERATURE_OFFSET  106

#define HUMIDITY_MAX     108    //humidity maximum
#define HUMIDITY_MIN     110    //humidity minimum
#define HUMIDITY_OFFSET  112

#define CO2_MAX     116   //humidity maximum
#define CO2_MIN     118    //humidity minimum
#define CO2_OFFSET  120

/* Define variables for the software */

float         temperature;           // in °C
int           humidity;              // in %
int unsigned  co2;                   // ppm
int           period;                // period to sent datas
char          name_module[20];       // name of module
String        ref_module;            // référence du module
String        version_module;        // version of module
char          ip_server[15];         // adress ip of the module
int unsigned  port_server;           // port of server
String        local_mac;             // adress mac of the module
String        local_ip;              // adress ip of module
byte          enableServer = 1 ;     // Use to control if the parameters are good

unsigned long refreshScreen=0;       //permet de d'actualiser les lectures de températures et l'écran
#define REFRESHSCREEN 5000


String        reception;             // To read information
int           code;                  // Variable code pour l'échange d'information
int           consigne='m';
String        ssid = "";
String        password = "";
int           config_mode = 0;
unsigned long delay_reboot;         // delay during the page of connection is open befor reboot

#define DELAY_REBOOT 120000

ESP8266WebServer serverhttp(80);

/* Configuration of screen */

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/* config update server*/

//const char* host = "esp8266-webupdate";
const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "admin";

ESP8266HTTPUpdateServer httpUpdater;


/* DHT temperature and humidity confuguration */

#define DHTPIN D5    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22,  DHT21, DHT11

DHT dht(DHTPIN, DHTTYPE);

#include <SoftwareSerial.h>
#define MH_Z19_RX D7
#define MH_Z19_TX D6

SoftwareSerial co2Serial(MH_Z19_RX, MH_Z19_TX);


/* alarm */

byte activation_alarmes; // Mot utilisé pour l'activation des alarmes
byte alarmes_actives;     // Drapeau d'une alarme active
int nbenvois = 0;         //nombre d'envoi de mail

#define ALARME_G  0  // Bit d'activation de la surveillance d'alarme
#define ALARME_C  1  // Activate CO2 alarm
#define ALARME_T  2  // Activate Temperature alarm
#define ALARME_H  3  // Activate Humidity alarm

int c_max;    // Set max CO2
int c_min;    // Set min CO2
int c_offset; // offset CO2

int t_max;    // Set max temperature
int t_min;    // Set min temperature
int t_offset; // Set offset temperature

int h_max;    // Set max humidity
int h_min;    // Set min humidity
int h_offset; // Set offset humidity

unsigned long  lastmessage;         // heures d'envoi des derniers relevés
unsigned long  millisecondes;       // Intervalle des relevés en millisecondes
int nb_cycle_lost_wifi=0;  

#define NB_TRYING_CONNECT 4  

void setup()
{
  Serial.begin(115200);

  co2Serial.begin(9600);
  //ini_CO2();
   

  /* read memory EEPROM */

  EEPROM.begin(512);

  activation_alarmes = EEPROM.read(ALARME_EEPROM);

  char carac = '1'; // Name of module
  int a = 0;
  while (!(carac == '\0') && a < 20) {
    carac = char (EEPROM.read(NAME_MODULE + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 20) {
    reception = F("InnoGreenTech");
  }
  int c = reception.length() + 1;                               // longueur de la chaîne de caractéres
  reception.toCharArray(name_module, c);

  reception = "";  // read memory ip adress server
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 20) {
    carac = char (EEPROM.read(ADRESS_SERVER + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 20) {
    reception = F("InnoGreenTech");
    enableServer = 0;
  }
  c = reception.length() + 1;
  reception.toCharArray(ip_server, c);

  byte mem[2];
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(PORT_SERVER + a); // load the port server
  }
  port_server = mem[0] | mem[1] << 8;

  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(PERIOD + a); // load the time period
  }
  period = mem[0] | mem[1] << 8;
  millisecondes = period * 1000;

  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(TEMPERATURE_MAX + a); // Récupération de la température Max
  }
  t_max = mem[0] | mem[1] << 8;
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(TEMPERATURE_MIN + a); // Récupération de la température Minimum
  }
  t_min = mem[0] | mem[1] << 8;
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(TEMPERATURE_OFFSET + a);
  }
  t_offset = mem[0] | mem[1] << 8;

  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(HUMIDITY_MAX + a); // Récupération de la température Max
  }
  t_max = mem[0] | mem[1] << 8;
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(HUMIDITY_MIN + a); // Récupération de la température Minimum
  }
  t_min = mem[0] | mem[1] << 8;
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(HUMIDITY_OFFSET + a);
  }
  t_offset = mem[0] | mem[1] << 8;

  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(CO2_MAX + a); // Récupération de la température Max
  }
  t_max = mem[0] | mem[1] << 8;
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(CO2_MIN + a); // Récupération de la température Minimum
  }
  t_min = mem[0] | mem[1] << 8;
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(CO2_OFFSET + a);
  }
  t_offset = mem[0] | mem[1] << 8;

  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 100) {
    carac = char (EEPROM.read(MEM_SSID + a));  //récupération du lien d'information
    if (!(carac == '\0')) {
      ssid += carac;
    } a++;
  }
  if (ssid == '\0' || a > 50) {
    ssid = F("");
  }

  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 100) {
    carac = char (EEPROM.read(MEM_PASSWORD + a));  //récupération du lien d'information
    if (!(carac == '\0')) {
      password += carac;
    } a++;
  }
  if (password == '\0' || a > 50) {
    password = F("");
  }

  /*Connexion wifi*/

  WiFi.mode(WIFI_STA);

  c = ssid.length() + 1;
  char char_ssid[50];
  ssid.toCharArray(char_ssid, c);

  c = password.length() + 1;
  char char_password[50];
  password.toCharArray(char_password, c);

  WiFi.begin(char_ssid, char_password);

  delay(20000);

  if ( WiFi.status() != WL_CONNECTED) // Portail de connexion
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    config_mode = 1;
    delay(200);
    WiFi.softAP("InnoGreenTech", "innogreentech");
    serverhttp.on ( "/index.html", handleRoot );    // go to set up wifi page
    delay(100);
    serverhttp.on ("/style.css", cssRoot);     // envoi au fichier de configuration de style
    delay(100);
    serverhttp.begin();
    delay_reboot = millis();

  }

  else

  {

    /* Configuration serial port */

    Serial.begin(115200);
    Serial.println("");
    Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );


    /*Configuration de la synchronisation de l'heure */

    NTP.begin("pool.ntp.org", 1, true);  //configuration de la récupération de la date,  Serveur+1 heure, Heure été/hivers
    NTP.setInterval(3600);                // Toute les heures

    delay(2000);


    /*informations du module*/

    local_ip = WiFi.localIP().toString().c_str();
    local_mac = WiFi.macAddress(); //.toString().c_str();
    ref_module = "module_oled";
    version_module = "V1";

    Serial.print(F("référence:")); Serial.println(ref_module);
    Serial.print(F("Version:")); Serial.println(version_module);
    Serial.print(F("Adresse IP:")); Serial.println(local_ip);
    Serial.print(F("Adresse MAC:")); Serial.println(local_mac);




    /* Configuration du serveur Web */

    serverhttp.on ( "/index.html", indexRoot );    // Renvoi à la page de index
    delay(100);
    serverhttp.on ( "/", indexRoot );    // Renvoi à la page de index
    delay(100);
    serverhttp.on ( "/add_module.csv", add_module );    // Page for add or modify  module by php server
    delay(100);
    serverhttp.on ( "/config_probe.htlm", config_probe );    // Page for do a configuration of probes by php server
    delay(100);
    serverhttp.on ("/style.css", cssRoot);     // envoi au fichier de configuration de style
    delay(100);
    httpUpdater.setup(&serverhttp, update_path, update_username, update_password);

  }

  /*listen on the server*/

    serverhttp.begin();

  /*Config Modbus IP*/
    mb.config("InnoGreenTech", "innogreentech");
    mb.addIreg(HIGH_WORD);
    mb.addIreg(LOW_WORD);

    dht.begin();
    //ini_CO2();
  
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

    display.clearDisplay();
    display.display();
}

void loop() {
  /* server control */
  serverhttp.handleClient();
  mb.task();

  /* read data write it on screen */

  if (millis()> refreshScreen + REFRESHSCREEN )
      {
        display.clearDisplay(); 
        display.setTextSize(1);
        display.setTextColor(BLACK, WHITE);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(WiFi.localIP());
        temperature =float(dht.readTemperature());
        humidity = dht.readHumidity();
        //co2 = readCO2();
        display.print(temperature);
        display.println(" C");
        display.print(humidity);
        display.println(" %");
        display.print(co2);
        display.println(" ppm"); 
        display.display();
        refreshScreen=millis();      
        Serial.print(temperature);
        

       //convert float in Byte

        char* c = (char*)(&temperature);
        
        int low_temperature= c[0] | c[1] <<8;
        int high_temperature= c[2]  | c[3] <<8;

        mb.Ireg(HIGH_WORD, high_temperature);
        mb.Ireg(LOW_WORD, low_temperature);
        
        Serial.println(" C");
        Serial.print(humidity);
        Serial.println(" %");
        Serial.print(co2);
        Serial.println(" ppm"); 
          
        /* use to test and configure CO2 probeµ*/
        if (Serial.available()>0){consigne=Serial.read();}

        

        if (consigne=='m'){co2 = readCO2();Serial.println("Mesure CO2");consigne='m';}
        if (consigne=='i'){ini_CO2();Serial.println("Init auto calibaration");consigne='m';}
        if (consigne=='z'){zero_CO2();Serial.println("Zero calibration"),consigne='m';}
        if (consigne=='c') {manu_cali();Serial.println("Calibration manuel"),consigne='m';}        
        
            
      }
  

}
