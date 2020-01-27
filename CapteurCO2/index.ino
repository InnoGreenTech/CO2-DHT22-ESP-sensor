
void indexRoot(){
              if ( serverhttp.hasArg("envoi")) 
              {      
                send_data();
              } 
               serverhttp.send ( 200, "text/html", indexPage() );   // envoi de la page
                }
  


String indexPage(){
                    String page =F("<!DOCTYPE html> <html lang=fr-FR> <head> <meta charset='utf-8'><link rel='stylesheet'type='text/css'href='style.css'><title>Capteur Wifi</title></head>");
                           page +=F("<body>");
                           page +=F("<nav> <ul id='menu'><li><a href='index.html'> Accueil </a></li><li><a href='reglage.html'> Réglages </a></li><li><a href='mail.html'> Mails </a> </li></ul></nav>");
                           page +=F("<div id='page'>");
                           page +=F("<header><h1>Centrale MINIATURE </h1></header>");
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
                           page +=F("</li><li> Delai envoi:");
                           page +=millisecondes;
                           page +=F("</li><li> Validation envoi:");
                           page +=enableServer;
                           page +=F("</li><li><form method='get'><input type='hidden' name='envoi' value='1'/><input type='submit' value='envoi'/>");                      
                           page +=F("</form></li></ul></section>");
                           page +=F("</div>");
                           page +=F("<footer><a href='http://Innogreentech.fr'>InnoGreenTech</a><a href='mailto: info@innogreentech.fr'>Contactez moi</a></footer>");
                           page +=F("</div></body></html>");                  
                           return page;
                          }
