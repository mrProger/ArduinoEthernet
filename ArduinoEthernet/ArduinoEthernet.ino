#include <SPI.h>
#include <Ethernet.h>
 
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  IPAddress ip(192, 168, 100, 20);
  IPAddress myDns(192, 168, 100, 1);
  EthernetServer server(80);
 
void setup() { 
  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("ERROR > Failed dhcp");

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("ERROR > Ethernet shield not found");  
    }
    else {
      Serial.println("INFO > Ethernet shiled founded");
    }

    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("ERROR > Ethernet cable is not connected");  
    }
    else {
      Serial.println("INFO > Ethernet cable is connected");
    }

    Ethernet.begin(mac, ip, myDns);
  }
  else {
    Serial.print("INFO > DHCP assigned IP: ");
    Serial.println(Ethernet.localIP());
  }
}
 
 
void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean ok = true;
 
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
 
        if (c == '\n' && ok) {
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5"); // время обновления страницы 
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><meta charset='UTF-8'>");
 
          client.println("<h1>Привет МИР!!!</h1>");
 
          client.println("</html>");
 
          break;
        }
        if (c == '\n'){ok = true;}else if(c != '\r'){ok = false;}
      }
    }
 
    delay(1);
    client.stop();
    Serial.println("client disconnected");
  }
}
