
void indexRoot(){
              if ( serverhttp.hasArg("envoi")) 
                {      
                  send_data();
                } 
              if (serverhttp.hasArg("commande"))
                {
                   consigne=serverhttp.arg("commande")[0];
                }
              
               serverhttp.send ( 200, "text/html", indexPage() );   // envoi de la page
               }
  


String indexPage(){
                    String page =F("<!DOCTYPE html> <html lang=fr-FR> <head> <meta charset='utf-8'><link rel='stylesheet'type='text/css'href='style.css'><title>Capteur Wifi</title></head>");
                           page +=F("<body>");
                           page +=F("<nav> <ul id='menu'><li><a href='index.html'> Accueil </a></li><li><a href='reglage.html'> Réglages </a></li><li><a href='mail.html'> Mails </a> </li></ul></nav>");
                           page +=F("<div id='page'>");
                           page +=F("<header><h1>Centrale Qualité </h1></header>");
                           page +=F("<div id='corp'>");
                           page +=F("<section id='datedujour'><h2>");
                           page +=NTP.getDateStr();
                           page +=F("</h2><h3>");
                           page +=NTP.getTimeStr();
                           page +=F("</h3>");
                           page +=F("<ul><li>Serveur:");
                           page +=ip_server;
                           page +=F("</li><li> Port serveur:");
                           page +=port_server;
                           page +=F("</li><li> Adresse MAC:");
                           page +=local_mac;
                           page +=F("</li><li> Version:");
                           page +=version_module;
                           page +=F("</li></ul></section>");
                           page +=F("<section id='capteur1'><h2> Capteur CO2 </h2><ul><li>Température:");
                           page +=temperature;
                           page +=F("</li><li>Humidité:");
                           page +=humidity;
                           page +=F("</li><li> CO2:");
                           page +=co2;
                           page +=F("</li><li>Délai d'envoi:");
                           page +=millisecondes/1000;
                           page +=F("</li><li> Validation envoi:");
                           page +=enableServer;
                           page +=F("</li><li><form method='get'><input type='hidden' name='envoi' value='1'/><input type='submit' value='envoi'/>");                      
                           page +=F("</form></li></ul></section>");
                           page +=F("<section id='Calibration'><h2> Clalibration CO2 </h2><ul><li>z pour calibrer le zéro");
                           page +=F("</li><li>i pour passer en auto calibration");
                           page +=F("</li><li>c pour passer en calibration manuel");
                           page +=F("</li><li>m pour effectuer une mesure ");
                           page +=F("</li><li></li><li><form method='get'>");
                           page +=F("<input type='text' name='commande'/>");  
                           page +=F("</li><li><input type='hidden' name='envoi' value='1'/></li><li><input type='submit' value='envoi'/>");                      
                           page +=F("</form></li></ul></section>");
                           page +=F("</div>");
                           page +=F("<footer><a href='http://www.Innogreentech.com'>InnoGreenTech</a><a href='mailto: fabricebaudin@outlook.fr'>Contactez moi</a></footer>");
                           page +=F("</div></body></html>");                  
                           return page;
                          }
