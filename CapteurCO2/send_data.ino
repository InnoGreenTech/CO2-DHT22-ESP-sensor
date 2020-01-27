void send_data(){


    
    if ( WiFi.status()!= WL_CONNECTED)
    
        {
        if (nb_cycle_lost_wifi<NB_TRYING_CONNECT)                            // test the wifi connection  if lost it, reset the module
                      {
                      nb_cycle_lost_wifi++;
                      lastmessage = lastmessage+30000;
                      return;              
                      } 

        else {ESP.reset();}  // if the lost of wifi
        }
      
 /*   WiFiClient client;                                          // Connection in mod

      if (!client.connect(ip_server,port_server))  
         {
        if (nb_cycle_lost_wifi<NB_TRYING_CONNECT)                // test the wifi connection  if lost it, reset the module
                      {
                      nb_cycle_lost_wifi++;
                      lastmessage = lastmessage+30000;
                      return;              
                      } 

        else {ESP.reset();}  // if the lost of wifi
         }      


      
      client.print("GET ");
      client.print("/InnoGreenTech/controller/get_probe.php");
      Serial.print("/InnoGreenTech/controller/get_probe.php");
      client.print("?mac_adress=");
      Serial.print("?mac_adress=");
      client.print(WiFi.macAddress());
      Serial.print(WiFi.macAddress());
      
      client.print("&ip_adress=");
      Serial.print("&ip_adress=");
      client.print(WiFi.localIP()); 
      Serial.print(WiFi.localIP()); 
         
      client.print("&probe1=");
      Serial.print("&probe1=");
      client.print("p1");
      Serial.print("p1");
      client.print("&value1=");
      Serial.print("&value1=");
      client.print(temperature);
      Serial.println(temperature);
      
      
      client.println(" HTTP/1.1");
      client.print("Host:");
      client.println(name_module);   
      client.println("Connection: close");
      client.println();                             //fin de la requet
      client.stop();      //déconnexion*/

      String ip=WiFi.localIP().toString().c_str();
      String data;

      const size_t bufferSize =JSON_OBJECT_SIZE(5);
      //DynamicJsonBuffer jsonBuffer(bufferSize);
      DynamicJsonDocument root(bufferSize);            
                  
      //JsonObject& root = jsonBuffer.createObject();
      root["mac_adress"]=WiFi.macAddress();
      root["ip_adress"]=ip; 
      root["p1"] =  co2;
      root["p2"] =  temperature;
      root["p3"]=   humidity;
      
     serializeJson(root,data); 
     //root.printTo(data);
     String adresse="http://";
     adresse+=ip_server;
     adresse+="/InnoGreenTech/controller/json_probe.php";
     String data1= "data=";
     data1 +=data;
     //Serial.println(data1);

     HTTPClient http;
     int httpCode = 0;
     http.begin(adresse);
     http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //http.addHeader("Content-Type", "application/json");
     httpCode = http.POST(data1);
     String payload = http.getString();
     //Serial.println(httpCode);   //Print HTTP return code
     //Serial.println(payload);
     http.end();  //Close connection
      
      /* Création des données json */      
      nb_cycle_lost_wifi=0;      
      lastmessage= millis();
  
}
