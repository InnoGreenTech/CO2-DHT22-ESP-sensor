void config_probe(){

      String id_probe="";


      if ( serverhttp.hasArg("id_prob") ) 
              {      
              reception=serverhttp.arg("id_prob");
              id_probe=reception.toInt();
              }
      else {return;}         

            if (id_probe=="p1")
            { 
              if ( serverhttp.hasArg("off_set") )
              {
              reception = serverhttp.arg("off_set");
              c_offset=reception.toInt();
              int b=t_offset;
              for (int a=0;a<2;a++){EEPROM.write(TEMPERATURE_OFFSET+a ,b);b=b>>8;}
              } 

            
              if ( serverhttp.hasArg("max") )
              {
              reception = serverhttp.arg("max");
              c_max=reception.toInt();
              int b=c_max;
              for (int a=0;a<2;a++){EEPROM.write(CO2_MAX+a ,b);b=b>>8;}
              }   

              if ( serverhttp.hasArg("min") )
              {
              reception = serverhttp.arg("min");
              t_min=reception.toInt();
              int b=c_min;
              for (int a=0;a<2;a++){EEPROM.write(CO2_MIN+a ,b);b=b>>8;} 
              } 

              if ( serverhttp.hasArg("alarm") )
              {
              reception=serverhttp.arg("alarm");
              if (reception.equals("1")) {bitSet(activation_alarmes, ALARME_C);}  // Active alarm
              else {bitClear(activation_alarmes, ALARME_C);}                       
              EEPROM.write(ALARME_EEPROM, activation_alarmes); 
              }
            }  

      if (id_probe=="p2")
            { 
              if ( serverhttp.hasArg("off_set") )
              {
              reception = serverhttp.arg("off_set");
              t_offset=reception.toInt();
              int b=t_offset;
              for (int a=0;a<2;a++){EEPROM.write(TEMPERATURE_OFFSET+a ,b);b=b>>8;}
              } 

            
              if ( serverhttp.hasArg("max") )
              {
              reception = serverhttp.arg("max");
              t_max=reception.toInt();
              int b=t_max;
              for (int a=0;a<2;a++){EEPROM.write(TEMPERATURE_MAX+a ,b);b=b>>8;}
              }   

              if ( serverhttp.hasArg("min") )
              {
              reception = serverhttp.arg("min");
              t_min=reception.toInt();
              int b=t_min;
              for (int a=0;a<2;a++){EEPROM.write(TEMPERATURE_MIN+a ,b);b=b>>8;} 
              } 

              if ( serverhttp.hasArg("alarm") )
              {
              reception=serverhttp.arg("alarm");
              if (reception.equals("1")) {bitSet(activation_alarmes, ALARME_T);}  // Active alarm
              else {bitClear(activation_alarmes, ALARME_T);}                       
              EEPROM.write(ALARME_EEPROM, activation_alarmes); 
              }
            }    
       if (id_probe=="p3")
            { 
              if ( serverhttp.hasArg("off_set") )
              {
              reception = serverhttp.arg("off_set");
              h_offset=reception.toInt();
              int b=h_offset;
              for (int a=0;a<2;a++){EEPROM.write(HUMIDITY_OFFSET+a ,b);b=b>>8;}
              } 

            
              if ( serverhttp.hasArg("max") )
              {
              reception = serverhttp.arg("max");
              h_max=reception.toInt();
              int b=h_max;
              for (int a=0;a<2;a++){EEPROM.write(HUMIDITY_MAX+a ,b);b=b>>8;}
              }   

              if ( serverhttp.hasArg("min") )
              {
              reception = serverhttp.arg("min");
              h_min=reception.toInt();
              int b=h_min;
              for (int a=0;a<2;a++){EEPROM.write(HUMIDITY_MIN+a ,b);b=b>>8;} 
              } 

              if ( serverhttp.hasArg("alarm") )
              {
              reception=serverhttp.arg("alarm");
              if (reception.equals("1")) {bitSet(activation_alarmes, ALARME_H);}  // Active alarm
              else {bitClear(activation_alarmes, ALARME_H);}                       
              EEPROM.write(ALARME_EEPROM, activation_alarmes); 
              }
            }      
              
           
           EEPROM.commit();
           serverhttp.send ( 200, "text/csv", answer_config_probe() );   // envoi de la page  
        }

String answer_config_probe()
        {
         String page ="Configuration reussie ";
         
         return page;
      }
