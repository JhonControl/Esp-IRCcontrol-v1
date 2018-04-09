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
//     ADDITIONAL IRC FUNCTIONS               FUNCIONES IRC ADICIONALES
// ========================================================================================================================================================





// ========================================================================================================================================================
/// print_nick captura el nickname de los usuarios que envian mensajes al canal/grupo
/// print_nick captures the nickname of users who send messages to the channel / group
// ========================================================================================================================================================

////void print_nick
String print_nick(char buffer[]) {
  String nick;
  for (int i = 0; i < IRC_BUFSIZE - 1; i++) {
    if (buffer[i] == '!') {
      break;
    }
    //  Serial.print(buffer[i]);
    nick += String(buffer[i]);

  }
  return nick;
}


// ========================================================================================================================================================
/// read_until take the characters and store them in a byte array
/// read_until tomar los caracteres y los almacena en un array byte(buffer)
// ========================================================================================================================================================

int read_until(char abort_c, char buffer[]) {
  int bytes_read = 0;
  memset(buffer, 0, sizeof(buffer));
  for (int i = 0; i < IRC_BUFSIZE - 1; i++) {
    if (client.available()) {
      char c = client.read();
      bytes_read++;
      if (c == abort_c) {
        return bytes_read;
      }
      else if (c == '\n') {
        return bytes_read;
      }
      buffer[i] = c;
    }
  }
  ignore_until(abort_c);
  return bytes_read;
}
// ========================================================================================================================================================
/// ignore_until:  reads characters from the connection until it hits the given character.
/// ignore_until:  lee caracteres de la conexión hasta que alcanza el carácter dado.
// ========================================================================================================================================================
void ignore_until(char c) {
  while (true) {
    if (client.available()) {
      char curr_c = client.read();
      if (curr_c == c) {
        return;
      }
    }
  }
}

// ========================================================================================================================================================
// print_until : tomar los caracteres utilies y retorna el mensaje
// print_until : take the useful characters and return the message
// ========================================================================================================================================================
String print_until(char c) {
  String msg;
  while (true) {
    if (client.available()) {
      char curr_c = client.read();
      msg += String(curr_c);
      if (curr_c == c) {
        //   Serial.println(msg);
        //  Serial.println("");
        return msg;
      }
      // Serial.print(curr_c);
    }
  }
}

// ========================================================================================================================================================
// IRCsendMsg : utilizando "PRIVMSG" Funcion que envia mensajes al canal IRC, parametros String channel y String newMsg
// IRCsendMsg : using "PRIVMSG" Function that sends messages to the IRC channel, parameters String channel and String newMsg
// ========================================================================================================================================================
// send mesagge IRC / reenvia mensaje IRC
void IRCsendMsg(String channel, String newMsg) {
  Serial.print("enviado/send: ");
  Serial.println("PRIVMSG " + channel + " :" + newMsg);
  client.print("PRIVMSG " + channel + " :" + newMsg + " \r\n");
  return;
}

String interactionMSGsensors[] = {
  "Que buen clima :)",
  "Que calor esta haciendo!!!"
};

// ========================================================================================================================================================
// IRCcommand (Received IRC, Parameter / Command, Answer)
/// ------------------------------------------------ ----------------------------
/// FUNCTION IRCcommand (String 1, String 2, String 3) return Bool
///
/// Compare String1 with String2 and send Message to channel irc on String3
/// String InCommand:  Messages received on the IRC channel
/// String OutCommand: Parameters or Command to Compare
/// String AddMessage: Return Message or response to IRC Channel
/// Return Bool: 1 = Equal Strings
///
///Includes GPIO'S control function
//
// ========================================================================================================================================================
//   IRCcommand (Recibido IRC ,Parametro/Comando,Respuesta)
// ----------------------------------------------------------------------------
// FUNTION IRCcommand (String 1 ,String 2 ,String 3 ) return Bool
//
// Compara String1 con String2  y envia Mensaje a canal irc el String3
// String InCommand: Mensajes Recibidos en el canal IRC
// String OutCommand: Parametros o comando a Comparar
// String AddMessage: Retorna Mensaje o respuesta hacia Canal IRC
// Return Bool:   1 = Strings Iguales
//
// Incluye funcion de control de GPIO'S
//
// ========================================================================================================================================================

bool IRCcommand (String InCommand, String OutCommand, String AddMessage) {

  String comandorec;
  String valor;

  // ========================================================================================================================================================
  //   FIRST FILTER & VALIDACION -- PRIMER FILTRO & VALIDACION
  // ========================================================================================================================================================
  for (int i = 0; i < InCommand.length() + 1; i++) {  //Compara mensaje IRC con comando ---- Compare IRC message with command

    /////     Separa comando GPIO y numero de PIN -- Separate GPIO command and PIN number  ////

    if (i < OutCommand.length() ) {         // Captura comando  -  Capture Command
      comandorec += InCommand[i];
    }
    else {                               // captura valor para habilitar gpio -  Capture Value for gpio enable
      valor += InCommand[i];
    }
  }

  comandorec.toLowerCase();     /// Convert strings to a lowercase letter /// Convertir cadenas a letra minuscula
  OutCommand.toLowerCase();     /// Convert strings to a lowercase letter /// Convertir cadenas a letra minuscula

  // ========================================================================================================================================================
  //   SECOND  VALIDACION -- SEGUNDA VALIDACION
  // ========================================================================================================================================================

  ///////////// Revalida comando entrante con comando en lista - Revalidate incoming command with command in list ///////////////

  if (comandorec.equals(OutCommand))  {

          //msgOriginal="";
      
          Serial.println("command OK");     ///////////COMANDO APROBADO -----------------  APPROVED COMMAND  /////////////////////
          ///-----------------------------------------------------------------------------------------////
          ///----------------------- GPIO CONTROL---&---COMMAND CONTROL ------------------------------////
          ///-----------------------------------------------------------------------------------------////
      
        
          if (comandorec.equals("high gpio") && valor.toInt()>0 ) {                                         //Command = Turn ON GPIO and GPIO  IS NUMBER
                 
              if(digitalRead(valor.toInt())==1){  //  verifica estado actual del GPIO   // check current status of the GPIO              
                    IRCsendMsg(channel, "Genius is already on   :D "); 
                    IRCsendMsg(channel, "Ya esta encendido Genio:D");            
                }else{
                    IRCsendMsg(channel, AddMessage + " : ON GPIO" + valor);     
                    digitalWrite(valor.toInt(), HIGH);
                }
                   
          }
          else if (comandorec.equals("low gpio") && valor.toInt()>0 ) {                                    //Command = Turn OFF GPIO and GPIO  IS NUMBER
                        
                if(digitalRead(valor.toInt())==0){  //  verifica estado actual del GPIO  // check current status of the GPIO              
                    IRCsendMsg(channel, "Genius is already off :D ");
                    IRCsendMsg(channel, "Ya esta apagado Genio :D");             
                }else{
                    IRCsendMsg(channel, AddMessage + " : OFF GPIO" + valor);
                    digitalWrite(valor.toInt(), LOW);            
                }
                      
          }
          else if ((comandorec.equals("low gpio") && valor.toInt()==0) || (comandorec.equals("high gpio") && valor.toInt()==0)) {    //Command  GPIO and GPIO NOT IS NUMBER
                        
                 /// Can not activate GPIO 0 /// No se puede activar el GPIO 0           
                 /// Mensaje si el comando de GPIO no tiene un numero pin, en caso de recibir otro carater retornara
                 /// Message if the GPIO command does not have a pin number, in case of receiving another carater it will return
          
                IRCsendMsg(channel, "Error: GPIO command without number :/ , Try: HIGH GPIO12 or LOW GPIO12");
                IRCsendMsg(channel, "Error: Comando GPIO sin numero :/ , Pruebe:  HIGH GPIO12  o LOW GPIO12");
                
          }
          else {  //******/   Aqui  Permite cualquier de los comandos ya conocidos ejemplo: "READ ADC0" "ESP??"  "ESP Temp" etc   //  Allow any of the commands already known example: "READ ADC0" "ESP??"  "ESP Temp" etc  ////****////
      
                ///-----------------------------------------------------------------------------------------////
                /// Aqui retorna el mensaje de todos los otros parametros ya conocidos... 
                /// Here returns the message of all the other known parameters ...
                ///-----------------------------------------------------------------------------------------////
      
                 IRCsendMsg(channel, AddMessage);                                      // SEND:  String AddMessage: Return Message or response to IRC Channel
                        
          }
      
          return true;                                                                 //COMPARACION DE COMANDO OK!!! :) -- COMMAND COMPARISON OK !!! :)
  }
  else {
    return false;                                                                      // NO COMMAND!!!!!!!!!!
  }

}

// ========================================================SECURITY CONTROL================================================================================
//ENABLE_USERPERMITTED=1
//
//Del mensaje recibido toma el "nickname" y lo compara con los usuarios permitidos almacenados en el array  "nick_users"
//From the received message take the "nickname" and compare it with the allowed users stored in the array  "nick_users"
//
//ENABLE_USERPERMITTED=0
//
//ESP8266 recibe comandos de cualquier usuario aunque el usuario no este enarray "nick_users"
//ESP8266 receives commands from any user even if the user is not in array "nick_users"
// ========================================================================================================================================================

bool userPermitted () {

  bool permitted = 0;

  for (int i = 0; i < users - 1 ; i++) {       ////  busca en  Array "nick_users" usuarios permitidos //// search in Array "nick_users" allowed users
    if (Nick.equals(nick_users[i])) {
      permitted = 1;
    }
  }

#if ENABLE_USERPERMITTED==0     ///permite control al todos los usuarios en el mismo grupo/canal //// allows control to all users in the same group / channel
  permitted = 1;
#endif
  return permitted;

}


