int readCO2() {
  // D'après le code original de | From original code https://github.com/jehy/arduino-esp8266-mh-z19-serial
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // command to ask for data
  byte response[9]; // for answer

  co2Serial.write(cmd, 9); //request PPM CO2

  // The serial stream can get out of sync. The response starts with 0xff, try to resync.
  while (co2Serial.available() > 0 && (unsigned char)co2Serial.peek() != 0xFF) {
    co2Serial.read();
  }

  memset(response, 0, 9);
  co2Serial.readBytes(response, 9);

  if (response[1] != 0x86)
  {
    Serial.println("Invalid response from co2 sensor!");
    co2Serial.flush();
   co2Serial.end();
   co2Serial.begin(9600);
    
    return co2;
  }

  byte crc = 0;
  for (int i = 1; i < 8; i++) {
    crc += response[i];
  }
  crc = 255 - crc + 1;

  if (response[8] == crc) {
    int responseHigh = (int) response[2];
    int responseLow = (int) response[3];
    //int ppm = (256 * responseHigh) + responseLow;
    int ppm = response[2] << 8 | response[3];
    return ppm;
  } else {
    Serial.println("CRC error!");
    return -1;
  }
}
 /** ini co2 probe with munual configuration */
  void ini_CO2()

      {     
                  
          uint8_t cmd[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00};

          uint8_t sum = 0x00;
           for (int i = 1; i < 8; i++)
          {
            sum += cmd[i];
          }
          sum = 0xff - sum + 0x01;

          cmd[8]= sum;
          co2Serial.write(cmd, 9); //request PPM CO2

          delay(3000);
      } 

      /** Zero calibration */
      void zero_CO2()
         
        {
          uint8_t cmd[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
          
          uint8_t sum = 0x00;
          for (int i = 1; i < 8; i++)
          {
            sum += cmd[i];
          }
          sum = 0xff - sum + 0x01;

          cmd[8]= sum;
    
          co2Serial.write(cmd, 9); //request PPM CO2*/

          delay (3000);
        }

       /** calibration manuel */

       void manu_cali()


          {                           
              uint8_t cmd[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
              uint8_t sum = 0x00;
               for (int i = 1; i < 8; i++)
              {
                sum += cmd[i];
              }
              sum = 0xff - sum + 0x01;
    
              cmd[8]= sum;
              co2Serial.write(cmd, 9); //request PPM CO2
    
              delay(3000);
          }        
      
