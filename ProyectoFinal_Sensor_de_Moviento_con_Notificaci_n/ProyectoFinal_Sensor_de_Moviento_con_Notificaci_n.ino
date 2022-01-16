#include <HTTPClient.h>
#include <dummy.h>
#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

// Reemplazar con los datos de tu red wifi
const char* ssid = "AINSHLEY";
const char* password = "Ainshley0809.O.T.";
String apiKey = "926599";              //Agregue su número de Token que el bot le ha enviado en WhatsApp messenger
String phone_number = "+593968176536"; // Agregue su número de teléfono registrado en la aplicación WhatsApp (el mismo número que el bot le envió en la URL)
String url;                            //url String se usará para almacenar la URL final generada
WiFiClientSecure client;
const int sensorPIR = 23; // Pin donde está conectada la entrada del sensor PIR
const int luz = 4;// Pin para led
int conteo = 0;
String mensaje = "";
String conteoString = "";
int PIR = 0;

////Setup////
void setup() {
  Serial.begin(115200);
  // Activamos el resistor de PULLUP para la entrada sensor PIR
  pinMode(sensorPIR, INPUT_PULLUP);
  pinMode(luz, OUTPUT);
  // Intenta conectarse a la red Wifi:
  Serial.print("Conectando a la red wifi... ");
  Serial.println(ssid);
  //Seteo de la red Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Conectado a la red wifi!!!");
  Serial.print("Dirección ip: ");
  Serial.println(WiFi.localIP());//Imprimimos la direción ip local
  // usa la función message_to_whatsapp para enviar tu propio mensaje
  message_to_whatsapp("Hola Erick, el Sistema está preparado!");  // envía su propio mensaje.
}
/////loop//////
void loop() {
   PIR = digitalRead(sensorPIR);//Leemos el estado del sensor PIR
   //Si es igual a "1" se cumple la condición
  if(PIR == 1){
    digitalWrite(luz, HIGH);//Encendemos el led
    Serial.println("Luz Encendida!");
    message_to_whatsapp("Movimiento Detectado!");  // envía su propio mensaje.
    delay(5000);//mantenemos la luz encendida este tiempo
    digitalWrite(luz, LOW);//Luego la apagamos
    Serial.println("Luz apagada!");
    PIR = 0;//Asignamos el valor "0" a la variable PIR para que deje de cumplirse la condición
  }
}


void  message_to_whatsapp(String message)       // función definida por el usuario para enviar mensajes a la aplicación WhatsApp
{
  // agregando todos los números, su clave api, su mensaje en una url completa
  url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);

  postData(); // llamando a postData para ejecutar la URL generada anteriormente una vez para que reciba un mensaje.
}

void postData()     // función de definición de usuario utilizada para llamar a api (datos POST)
{
  int httpCode;     // variable utilizada para obtener el código http de respuesta después de llamar a api
  HTTPClient http;  // Declarar objeto de clase HTTPClient
  http.begin(url);  // comienza el objeto HTTPClient con la url generada
  httpCode = http.POST(url);// Finalmente, publique la URL con esta función y almacenará el código http
  if (httpCode == 200)      // Comprobar si el código http de respuesta es 200
  {
    Serial.println("Mensaje enviado"); // imprimir mensaje enviado ok mensaje
  }
  else                      // si el código HTTP de respuesta no es 200 significa que hay algún error.
  {
    Serial.println("Error."); // imprimir mensaje de error.
  }
  http.end();          // Después de llamar a la API finaliza el objeto de cliente HTTP.
}

String urlencode(String str)  // Función utilizada para codificar la url
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
}
