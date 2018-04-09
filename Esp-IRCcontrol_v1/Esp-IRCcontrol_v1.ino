// ========================================================================================================================================================
/*--------------------------------------------------------------
  Program:      IRCControl + Interactive ESP8266 12E 
  Version:      v 1.0

  Description:  Implementation of IRC client for ESP8266 measurement and remote control from the Internet without complications or intermediary applications.                     
                Implementacion de cliente IRC para ESP8266 medicion y control remoto desde Internet sin complicaciones o aplicaciones intermediarias.  
                
  Hardware:     ESP8266 12E NodeMCU Lolin .
  Sensor        DHT11  GPIO14           
                DS18B20 OneWire GPIO2
                 
  Software:     Arduino IDE v1.8.3
  
  Date:         2018-04-08

  Enter and test the control from IRC Client:  http://pdacontrolen.com/irc/
  Complete Tutorial English:                   http://pdacontrolen.com/esp8266-irccontrol-from-internet-interaction-iot-v1-0-part-1/ 

  Ingresa y prueba el control desde Cliente:   http://pdacontroles.com/irc/
  Tutorial Completo Español                    http://pdacontroles.com/esp8266-irccontrol-interactivo-iot-v1-0-part-1/  

  Based:        On work done by Uwe Kamper  ArduinoIRC https://github.com/uwekamper/arduinoirc
     
  Modified or Created:       PDAControl   http://pdacontroles.com   http://pdacontrolen.com
--------------------------------------------------------------*/
// ========================================================================================================================================================
//      RUTINA PRINCIPAL    MAIN ROUITNE
// ========================================================================================================================================================

/*
        Requirements:
        SSID & PASSWORD
        Nickname IRC max 10 characters
        IRC server freenode "chat.freenode.net" port 6667
        Channel IRC optional
                
        Functionalities:
        Control: Activation and depletion GPIO's
        Monitoring: Read ADC0 value
        Monitoring: Read RSSI signal value
        Monitoring: Read Temperature Sensor DHT11
        Monitoring: Read Relative Humidity Sensor DHT11
        Monitoring: Read Sensor Temperature DS18B20
        Menu of available parameters
        Interaction: Answers correct messages
******************************************************************************
        Requerimientos
        SSID & PASSWORD
        Nickname IRC    max 10 caracteres
        Servidor IRC      freenode "chat.freenode.net"   port 6667
        Channel IRC      optional
       
        Funcionalidades:
        Control: Activacion y descativacion GPIO's
        Monitoreo: Leer valor  ADC0
        Monitoreo: Leer valor señal RSSI
        Monitoreo: Leer Sensor Temperatura DHT11
        Monitoreo: Leer Sensor Humedad Relativa DHT11
        Monitoreo: Leer Sensor Temperatura DS18B20        
        Menu de parametros disponibles        
        Interaccion: Respuestas a ciertos mensajes
 */

#include <ESP8266WiFi.h>

/// 1 = Habilita lectura sensor DHT11   // 1 = Enables DHT11 sensor reading
#define ENABLE_DHT11 1  

/// 1 = Habilita lectura sensor DS18B20   // 1 = Enables DS18B20 sensor reading
#define ENABLE_DS18B20 1  

// ========================================================================================================================================================
///  1  ESP8266 could respond or send some messages automatically, with pre-loaded answers //// 1 ESP8266 podria responder o enviar algunos mensajes automaticamente, con  respuestas pre cargadas
///  0  ESP8266 only responds to commands received                                         //// 0 ESP8266 solo responde a comandos recibidos 
#define ENABLE_INTERACTIVE  1

// ========================================================================================================================================================
/// 1 = Only receive commands from the users entered in the Array nick_users    /// 1 = Solo recibe comandos de los usuarios ingresados en el Array nick_users
/// 0 = Only receive commands from any user on the same channel                /// 0 = Solo recibe comandos de cualquier usuario en el mismo canal
#define ENABLE_USERPERMITTED  0

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t users=2;                                            // Number of users Allowed to control ESP     // Cantidad de usuarios Permitidos para controlar ESP
String nick_users[]={"PDAUSER|49192", "PDAUSER|xxxx"};       //Allowed users to control ESP8266           // Usuarios permitidos para controlar ESP8266
// ========================================================================================================================================================

// ========================================================================================================================================================
#if ENABLE_DHT11==1  
#include "DHT.h" 
#define DHTPIN 14                                   /// GPIO14 data
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
#endif
// ========================================================================================================================================================

// ========================================================================================================================================================
#if ENABLE_DS18B20==1  
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2                                // GPIO 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
#endif
// ========================================================================================================================================================
bool first_bit=0;
// ========================================================================================================================================================
//  SSID y contraseña desde su propia red WiFi             // SSID and password from your own WiFi network    
const char* ssid     = "************";
const char* password = "************";
// ========================================================================================================================================================
//  Parameters IRC Connection
const char* host = "chat.freenode.net";        // Servidor IRC // IRC Server  /// https://es.wikipedia.org/wiki/Freenode
uint32_t portirc = 6667;                       // IRC Port 
String nickname = "ESP8266IRC";     /// MAX 10 Characters           // Nickname module ESP on IRC
String channel =   "#PDAChannel";   //  example "#TestIRCready"; 
// ========================================================================================================================================================

String Msg;
String Nick;
String Mac;
String NickComplete;
uint32_t counterInteractions=0;

/// Buffers IRC /////////
#define IRC_BUFSIZE  32     
char from[IRC_BUFSIZE];
char type[IRC_BUFSIZE];
char to[IRC_BUFSIZE];
////////////////////////

WiFiClient client;
   
void setup() {                              // Tutorial http://pdacontrolen.com/irc/

  pinMode(12, OUTPUT);  //Led pin
  Serial.begin(115200);

   #if ENABLE_DHT11==1
   dht.begin();
   #endif

   #if ENABLE_DS18B20==1  
   sensors.begin();
   #endif
  
  delay(10);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
 
   // ========================================================================================================================================================  
   //-----------------------------------------------------------------------------------------------------------------------------------------------------------
   // we take the last 6 characters of the MAC and add to the Nickname for differences the ESP8266
   // tomamos los ultimos 6 caracteres de la MAC y adicionamos al Nickname para diferencias los ESP8266 
   Mac = String(WiFi.macAddress());
  /// Serial.println(Mac);     /// 
   Mac.remove(0, 9);
   Mac.replace(":", "");
   //Serial.println(Mac);      
   NickComplete = nickname + Mac;   
   Serial.println("nickname esp8266 = "+NickComplete); 
   // ========================================================================================================================================================
}


void loop()
{ 
  if (!client.connected()) {
    Serial.println("connecting ...");
    if (client.connect(host, portirc)) {
      // ========================================================================================================================================================
      // Ingresando al Canal   // Entering the Channel      
      Serial.println("connected");
      delay(500);
      client.println("USER  uk 8 *  : Uwe user\r");
      delay(500);
      client.println("NICK "+nickname+Mac+"\r");     
      delay(250);
      Serial.println("Ingresando a Canal  ...");
      delay(250);
      Serial.println(channel);
      client.println("JOIN "+channel+"\r");
      // ========================================================================================================================================================
      
      handle_irc_connection();                 //  Manejador de conexion Irc  //  Irc Connection Manager
  } 
    else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed...........");
      delay(1000);
    }
  } 
}  /// end void loop()

// ========================================================================================================================================================
//   MANEJADOR IRC   IRC HANDLER
// ========================================================================================================================================================  
void handle_irc_connection() {
  char c;
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while(true) {
    if (!client.connected()) {       
      return;      
    }
 // ========================================================================================================================================================
 //  captura los caracteres recibidos  // capture the received characters
    
    if(client.available()) {       
      c = client.read();      
    }
   // ========================================================================================================================================================
   
    else {    
      continue;
    }
    // ========================================================================================================================================================
    // El servidor IRC envia un mensaje muy largo, aqui se corta y se extraen los mensajes.  
    // The IRC server sends a very long message, here it is cut and the messages are extracted.
    // ========================================================================================================================================================
    if(c == ':') {
      memset(from, 0, sizeof(from));
      memset(type, 0, sizeof(type));
      memset(to, 0, sizeof(to));
      
      read_until(' ', from);
      read_until(' ', type);
      read_until(' ', to);   
   
// ========================================================================================================================================================
/// PRIVMSG es para enviar un mensaje a un usuario o un canal
//  PRIVMSG is for sending a message to a user or a channel
// ========================================================================================================================================================        
      if(strcmp(type, "PRIVMSG") == 0) {    
    
   // ========================================================================================================================================================
   //    Reception and display of all messages sent to the Channel  --------------    Recepcion y visualizacion de todos los mensajes enviados al Canal 
          Nick = print_nick(from);
          ignore_until(':');      
          Msg = print_until('\r'); 
////-----------------------------------------------------------------------------------------------------------------------------------------------------------------//////
           
            ///  Greeting of ESP8266 when entering channel     ///  Saludo de ESP8266 al ingresar a canal 
            if(first_bit==0){         
                       IRCsendMsg(channel,"Hello everyone :), I am "+ NickComplete);   
                       first_bit=1; 
              }
          
          Serial.println("<"+Nick+"> :"+String(Msg));   /// Print serial terminal - user + Msg     /// visualizacion de mensajes // print message display     

////-----------------------------------------------------------------------------------------------------------------------------------------------------------------//////
////-----------------------------AGREGAR SU PROPIO CODIGO DESDE AQUI--------------------  ADD YOUR OWN CODE FROM HERE    --------------------------------------------//////
////-----------------------------------------------------------------------------------------------------------------------------------------------------------------//////

   
   // ==============================================SECURITY==========================================================================================================
   // ENABLE_USERPERMITTED  1   only allows user commands in the Array "nick_users"     ///   solo permite comandos de los usuarios en el Array "nick_users"
   // ENABLE_USERPERMITTED  0   Allows commands from any user on the same IRC channel  /// Permite comandos de cualquier usuario en el mismo canal IRC
   // ========================================================================================================================================================
         
           if(userPermitted ()==1) {    /// ZONE Secure  --Zona Segura   if ENABLE_USERPERMITTED  1
        
    
         //  FUNTION bool IRCcommand (Messages received from IRC , Parameter / Command, Answer)
         //  Funcion bool IRCcommand (Mensajes recibidos desde IRC ,Parametro/Comando,Respuesta)  

         // -------------------------------Controla Cualquier salida del ESP8266  -----Control Any Exit of ESP8266---------------------------------------------
         //Sending parameter:  "HIGH GPIO#"  Example: "HIGH GPIO12"  will turn in the gpio12 or any gpio initialized in void setup() 
         //Enviando parametro: "HIGH GPIO#"  Ejemplo: "HIGH GPIO12"  encendera el gpio12   o cualquier gpio inicializado en  void setup()  
         // ----------------------------------------------------------------------------   
          IRCcommand(Msg,"HIGH GPIO","Turn ON");
               
         // ----------------------------------------------------------------------------
         //Sending parameter:  "LOW GPIO#"  Example: "LOW GPIO12"  will turn off the gpio12  or any gpio initialized in void setup() 
         //Enviando parametro: "LOW GPIO#"  Ejemplo: "LOW GPIO12"  apagaran el gpio12  o cualquier gpio inicializado en void setup() 
         // ----------------------------------------------------------------------------                           
         IRCcommand(Msg,"LOW GPIO","Turn OFF"); 
         // ----------------------------------------------------------------------------------------------------------------------------------------------------

        ///--------------------------Leer Variables -- Read Variables -------------------////
        /// Parameter  "READ ADC0" 
        IRCcommand(Msg,"READ ADC0","ADC0 esp8266 = "+String(A0)); 

        /// Parameter "ESP SIGNAL"
        IRCcommand(Msg,"ESP SIGNAL?","Signal RSSI ESP8266 = "+String(WiFi.RSSI( ))+"dB");
       
       ////-------------------------------Sensors-----------------------------------------////
       
       #if ENABLE_DHT11==1                 
                     float humedad = dht.readHumidity();
                     float temperatura = dht.readTemperature();
                     IRCcommand(Msg,"ESP Temp?","DHT11 Temperature Now = "+String(temperatura)+"°C");                      
                     IRCcommand(Msg,"ESP HR?","DHT11 Humidity Relative Now = "+String(humedad)+"%" );              
           #endif

        #if ENABLE_DS18B20==1          
                    sensors.requestTemperatures();
                   float celsius = sensors.getTempCByIndex(0);
                   IRCcommand(Msg,"ESP Temp2?","DS18B20 Onewire Temperature Now = "+String(celsius)+"°C");
         #endif
    
         //-----------------------------Help-------------------------------------------------///              
         /// Enviando parametro: "ESP8266IRCXXXXXX Help"  Menu Se visualizaran los comandos 
         /// Sending parameter:  "ESP8266IRCXXXXXX Help"  Menu Commands will be displayed
         /// ----------------------------------------------------------------------------                                                              
         if(IRCcommand(Msg,NickComplete+" help","*------ Comandos Disponibles / Available Commands -------*" ))  {

               // funcion sin retorno para IRCsendMsg enviar mensajes, requiere, canal y String de mensaje 
               //function without return IRCsendMsg to send messages, requires, channel and message String
                      IRCsendMsg(channel,"         HIGH GPIO#");   ///
                      IRCsendMsg(channel,"         LOW GPIO#");  
                      IRCsendMsg(channel,"         READ ADC0"); 
                      IRCsendMsg(channel,"         ESP Temp?");                      
                      IRCsendMsg(channel,"         ESP Temp2?"); 
                      IRCsendMsg(channel,"         ESP HR?");
                      IRCsendMsg(channel,"         ESP SIGNAL?"); 
                      IRCsendMsg(channel,"*--------------------------------------------------------*");                                             
                  
          } 
                                 
       }     /// end if // userPermitted ==1
      
         // ----------------------------------------------------------------------------
         //  Allows commands from any user on the same IRC channel  /// Permite comandos de cualquier usuario en el mismo canal IRC
         // ----------------------------------------------------------------------------
        
          ///Interacion al ver un :(   ///   Interaction to see a :(
          IRCcommand(Msg,":(","Why are you sad? "+Nick );  
          IRCcommand(Msg,":(","Por que estas triste? "+Nick );  
        
         // ----------------------------------------------------------------------------
         //  Commands can be created, sending, "ESP?? the esp8266 will respond
         //  Se pueden crear comandos, enviando ,"ESP?? el esp8266 respondera
         // ----------------------------------------------------------------------------                              
         IRCcommand(Msg,"ESP?????","Hi, I'm here. I'm " + NickComplete +" :P  space on the chip is "+String(ESP.getFreeHeap())+" and how are you?? "+Nick);    


//  END Reception and display of all messages sent to the Channel  --------------  FIN  Recepcion y visualizacion de todos los mensajes enviados al Canal 
////-----------------------------------------------------------------------------------------------------------------------------------------------------------------//////
////-----------------------------AGREGAR SU PROPIO CODIGO HASTA AQUI--------------------  ADD YOUR OWN CODE TO HERE    ----------------------------------------------//////
////-----------------------------------------------------------------------------------------------------------------------------------------------------------------//////
      }
      else {       
      
        ignore_until('\r');
      } 
      
    }  
  // ========================================================================================================================================================    
  // El servidor IRC constantemente realiza una solitud al ESP8266, un "PING" para verificar que esta conectado o activo? y el esp8266 responde "PONG" cada 3min aprox
  // The server IRC constantly makes a request to ESP8266, a "PING" to verify that it is connected or active and esp8266 responds "PONG" every 3 min APROX
  // ========================================================================================================================================================
 
    else if (c == 'P') {   
      char buf[5];
      memset(buf, 0, sizeof(buf));
      buf[0] = c;
      for(int i = 1; i < 4; i++) {
        c = client.read();
        buf[i] = c;
      }
      ignore_until('\r');
      
      if(strcmp(buf, "PING") == 0) {           // RECEIVE "PING"           RECIBE "PING"
        client.println("PONG\r");              //ESP8266 RESPONSE "PONG"   ESP8266 RESPONDE "PONG"
       /// Serial.println("PING->PONG");   
                  
           #if ENABLE_INTERACTIVE==1
               counterInteractions = counterInteractions +1; // Incrementa cada vez que hay PING, cada 3 minutos aproximadamente /// Increase every time there is PING, every 3 minutes approximately       
            #endif     
            // =====================================================================================================================================================     
              if(counterInteractions>= 15)  {  // every 12 min aprox esp8266 send message to char IRC
                  Serial.println("interaction");        
                  IRCsendMsg(channel,"More info Project PDAControl: pdacontrolen.com/irc  ;)"); //Envia un mensaja al chat    // send message to chat
                  delay(100);
                  IRCsendMsg(channel,"Mas info Proyecto PDAControl: pdacontroles.com/irc  ;)"); //Envia un mensaja al chat    // send message to chat
                  counterInteractions=0;
               }
             
      }
    }
  }   // end while
   
}    //end  handle_irc_connection



