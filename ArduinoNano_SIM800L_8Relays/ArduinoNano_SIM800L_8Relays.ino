
 
#include <SoftwareSerial.h> //Incluir libreria <SoftwareSerial.h>
SoftwareSerial Sim800l(12, 11); // rx tx oficial
// configuracion de los reles.................................................................................
int reles[8] = {3,4,5,6,7,8,9,10};

char comando = 0; //Variable que guarda los caracteres que envia el SIM
String mensaje = ""; //Mensajes que se muestran en el monitor serial
String estado="";
int msj_sim800=0;
int segundos=0; 
int timeout=0;
long ciclos=0;

// Numero master *****************************************************************************

String numero_master="04112221122"; // numero del cliente a enviar el mensaje **********************************************************************
String pw="1234"; // password de seguridad

void setup() {
    Serial.begin(9600);
    pinMode(13, OUTPUT); 
    iniciando_modulo_reles();
    iniciando_modulo_Sim800l();


}

void iniciando_modulo_reles() {

   for (int i=0 ; i < 9 ; i++)
  {
     pinMode(reles[i], OUTPUT); // configurando rele como salida
     digitalWrite(reles[i], HIGH); // colocando el rele desactivado por defecto
  }

  
}

void iniciando_modulo_Sim800l() {
  delay(30000);
  Sim800l.begin(9600);
  Sim800l.println("AT");
  delay(500);
  Sim800l.println("AT");
  delay(1500);
  Sim800l.println("AT+CREG=1\r"); // Activacion de registro
  delay(1000);
  Sim800l.println("AT+CLIP=1\r"); // Activa la identificacion de llamada
  delay(1000);
  Sim800l.println("AT+CMGF=1\r"); //Configura el modo texto para enviar o recibir mensajes
  delay(1000);
  Sim800l.println("AT+CNMI=2,2,0,0,0\r"); // Saca el contenido del SMS por el puerto serie del modulo
  delay(1000);
  Serial.println("Modulo Sim800l inicializado");
  delay(1000);
  Sim800l.print("AT+CMGS=\"");  
  Sim800l.print(numero_master);  
  Sim800l.println("\"");  
  delay(1000);
  Sim800l.print("Se Ha Encendido El Sistema");  // Texto del SMS

  Sim800l.println((char)26); //Comando de finalización ^Z
  delay(100);
  Sim800l.println();

}

void loop() {

   timeout++;
   ciclos++;
   if (Sim800l.available() > 0) 
   {
        comando = Sim800l.read(); //Guardamos los caracteres
        mensaje = mensaje + comando; //Añadimos los caracteres leidos al mensaje
        timeout=0;
        msj_sim800=1;
   }

    if(timeout == 200) // tiempo de espera para veriricar el mensaje recibido
    {
       if(msj_sim800==1)
       {
          identifica_mensajes();
          mensaje="";
          msj_sim800=0;
       }

    }

   if(ciclos==1000)
   {   
          digitalWrite(13, !digitalRead(13)); 
          ciclos=0;
         
   }
  
 }

void identifica_mensajes() {
  int pass =  mensaje.indexOf(pw);  
  if( pass>=0)
  {

      int resetear = mensaje.indexOf("Rs");
      int encender = mensaje.indexOf("On");
      int apagar = mensaje.indexOf("Of");
      int encender_todos =  mensaje.indexOf("Ontodos");
      int apagar_todos =  mensaje.indexOf("Oftodos");
      int gsmok =  mensaje.indexOf("Gsmok");
      int rele_seleccionado =0;


                  if( resetear>=0)
                  {
                        String nro_de_rele = mensaje.substring(resetear+2,resetear+4);
                        rele_seleccionado = nro_de_rele.toInt(); 
                        if((rele_seleccionado >0) && (rele_seleccionado <9)) // aseguramos que solo se ejecuten acciones sobre los reles 1 al 8
                        {  
                            digitalWrite(reles[rele_seleccionado-1], LOW); // restamos 1 porque va de 0 a 8
                            delay(10000);
                            digitalWrite(reles[rele_seleccionado-1], HIGH); // restamos 1 porque va de 0 a 8
                            delay(5000);
                            enviar_sms("Reset realizado");
                           
                        }
                  }
   
          
                //*********************************************************************************************************************************
                    if( encender>=0)
                    {
                        String nro_de_rele = mensaje.substring(encender+2,encender+4);
                        rele_seleccionado = nro_de_rele.toInt(); 
                        if((rele_seleccionado >0) && (rele_seleccionado <9)) // aseguramos que solo se ejecuten acciones sobre los reles 1 al 8
                        {  
                            
                            digitalWrite(reles[rele_seleccionado-1], LOW); // restamos 1 porque va de 0 a 8
                            delay(5000);
                            enviar_sms("On realizado");
                        }
                    }
                  
                    if( apagar>=0)
                    {
                        String nro_de_rele = mensaje.substring(apagar+2,apagar+4);
                        rele_seleccionado = nro_de_rele.toInt() ;
                        if((rele_seleccionado >0) && (rele_seleccionado <9)) // aseguramos que solo se ejecuten acciones sobre los reles 1 al 15
                        {  
                            digitalWrite(reles[rele_seleccionado-1], HIGH);  // restamos 1 porque va de 0 a 15
                            delay(5000);
                            enviar_sms("OFF realizado");
                        }     
                    }
                  
                    if( encender_todos>=0)
                    {
                            encender_todos_secuencial();
                            delay(5000);
                            enviar_sms("Todos ON");
                    }
                  
                    if( apagar_todos>=0)
                    {
                            apagar_todos_secuencial();
                            delay(5000);
                            enviar_sms("Todos OFF");
                    }
                  
                  
                    if (gsmok >=0)        // r1:on,r2:of,
                    {

                        Sim800l.println("AT");
                        delay(1000); 
                        Sim800l.print("AT+CMGS=\"");  
                        Sim800l.print(numero_master);  
                        Sim800l.println("\""); 
                        delay(1000);
                        Sim800l.print("Sistema GSM funcionando ");  // Texto del SMS
                  
                        // revisamos el estado de cada uno de las salidas digitales asiganadas a los reles
                        
                        for (int i=0 ; i < 8 ; i++)
                        {
                            Sim800l.print(" R");
                            Sim800l.print(i+1); // escribe el nro del rele
                            Sim800l.print(": ");
                            if(digitalRead(reles[i])) // evalua el estado de la salida digital
                            {
                                Sim800l.print("OFF ,");
                            }
                            else
                            { 
                               Sim800l.print("ON ,");
                            } 
                            delay(1000);
                        }
                        
                        delay(100);
                        Sim800l.println((char)26); //Comando de finalización ^Z
                        delay(100);
                        Sim800l.println();
                    }
                          
  } // if es sms master
  
  
}
void encender_todos_secuencial()
{
   for (int i=0 ; i < 9 ; i++)
  {
     pinMode(reles[i], OUTPUT); // configurando rele como salida
     digitalWrite(reles[i], LOW); // colocando el rele desactivado por defecto
     delay(500);
  }
}

void apagar_todos_secuencial()
{
   for (int i=0 ; i < 9 ; i++)
  {
     pinMode(reles[i], OUTPUT); // configurando rele como salida
     digitalWrite(reles[i], HIGH); // colocando el rele desactivado por defecto
     delay(500);
  }
}



void enviar_sms( String texto)
{

      Sim800l.println("AT");
      delay(1000);
      Sim800l.print("AT+CMGS=\"");  
      Sim800l.print(numero_master); 
      Sim800l.println("\""); 
      delay(1000);
      Sim800l.print(texto);  // Texto del SMS
      delay(100);
      Sim800l.println((char)26); //Comando de finalización ^Z
      delay(100);
      Sim800l.println();
   
  
}
