#include <Arduino.h>
#include <IRremote.h> //INCLUSÃO DE BIBLIOTECA 
#include <ESP8266WiFi.h>

//config Rede
const char* ssid     = "PROF.RAFAEL"; //Roteador
const char* password = ""; 
IPAddress ip(192,168,1,171); //ip do dispositivo 
IPAddress gateway(192,168,1,1); // gateway
IPAddress subnet(255,255,255,0); //mascara



WiFiServer server(80);


String header;

String output5State = "off";

String Sumidificar = "Umidificar";

String Svelocidade = "Velocidade";

String Soscilacao = "Oscilação";

String Sresfriar = "Resfriar";

String Stimer = "Timer";


//porta digital
const int output5 = 5; //D1



unsigned long currentTime = millis();

unsigned long previousTime = 0; 

const long timeoutTime = 2000;

 
int RECV_PIN = 2; //PINO DIGITAL UTILIZADO PELO FOTORRECEPTOR KY-022
 
IRrecv irrecv(RECV_PIN); //PASSA O PARÂMETRO PARA A FUNÇÃO irrecv
 
decode_results results; //VARIÁVEL QUE ARMAZENA OS RESULTADOS (SINAL IR RECEBIDO)
//Pro controle climatizador
String ligar = "1FE48B7";
String umidificar = "1FE20DF";
String velocidade = "1FEA05F";
String oscilacao = "1FE609F";
String resfriar = "E50FD9A5";
String timer = "1FE";

//Pro controle do alexandre




 
void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  irrecv.enableIRIn(); //INICIALIZA A RECEPÇÃO DE SINAIS IR

  //habilitar porta
  pinMode(output5, OUTPUT);


  //O estado da porta
  digitalWrite(output5, HIGH);
 

  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
 
void loop(){
  //CAPTURA O SINAL IR
  //Serial.println("Esperando");
  if (irrecv.decode(&results)) {
    Serial.print("Código HEX: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(results.value, HEX); //IMPRIME NO MONITOR SERIAL O CÓDIGO IR EM FORMATO HEXADECIMAL
    Serial.print("Código DEC: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(results.value); //IMPRIME NO MONITOR SERIAL O CÓDIGO IR EM FORMATO DECIMAL
    Serial.println(""); //QUEBRA DE LINHA NA SERIAL
    irrecv.resume(); //AGUARDA O RECEBIMENTO DE UM NOVO SINAL IR
  }
  delay(200); //INTERVALO DE 100 MILISSEGUNDOS

 WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
         
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
           
            if (header.indexOf("GET /AR/Ligado") >= 0) {
              Serial.println("on");
              output5State = "Ligado";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /AR/Desligado") >= 0) {
              Serial.println("off");
              output5State = "Desligado";
              digitalWrite(output5, HIGH);
            }
            
          
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
          
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color:  #ffcbdb; border: none; color: black; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color:  #ffcbdb;}</style></head>");
           


            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<body><h2>Projeto Ar Condicionado</h2>");
            
             
            client.println("<p>Ar Condicionado " + output5State + "</p>");
              
            if (output5State=="Desligado") {
              client.println("<p><a href=\"/AR/Ligado\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/AR/Desligado\"><button class=\"button button2\">OFF</button></a></p>");
            } 
             client.println("<p>Ar Condicionado " + Sumidificar + "</p>");
              
            if (output5State=="Desligado") {
              client.println("<p><a href=\"/Sumidificar/Ligado\"><button class=\"button\">Umidificar</button></a></p>");
            } else {
              client.println("<p><a href=\"/Sumidificar/Desligado\"><button class=\"button button2\">Umidificar</button></a></p>");
            } 
            client.println("<p>Ar Condicionado " + Svelocidade + "</p>");
             if (output5State=="Desligado") {
              client.println("<p><a href=\"/Svelocidade/Ligado\"><button class=\"button\">Velocidade</button></a></p>");
            } else {
              client.println("<p><a href=\"/Svelocidade/Desligado\"><button class=\"button button2\">Velocidade</button></a></p>");
            }
            client.println("<p>Ar Condicionado " + Soscilacao + "</p>"); 
            if (output5State=="Desligado") {
              client.println("<p><a href=\"/Soscilacao/Ligado\"><button class=\"button\">Oscilar</button></a></p>");
            } else {
              client.println("<p><a href=\"Soscilacao/Desligado\"><button class=\"button button2\">Oscilar</button></a></p>");
            }
            client.println("<p>Ar Condicionado " + Sresfriar + "</p>");
            if (output5State=="Desligado") {
              client.println("<p><a href=\"/Sresfriar/Ligado\"><button class=\"button\">Resfriar</button></a></p>");
            } else {
              client.println("<p><a href=\"Sresfriar/Desligado\"><button class=\"button button2\">Resfriar</button></a></p>");
            }
            client.println("<p>Ar Condicionado " + Stimer + "</p>");
            if (output5State=="Desligado") {
              client.println("<p><a href=\"/Stimer/Ligado\"><button class=\"button\">Timer</button></a></p>");
            } else {
              client.println("<p><a href=\"Stimer/Desligado\"><button class=\"button button2\">Timer</button></a></p>");
            }

            client.println("</body></html>");
            
   
            client.println();
       
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
