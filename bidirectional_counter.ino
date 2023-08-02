#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NodeMCU2";  // Enter SSID here
const char* password = "12345678";  // Enter Password here


/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int irPin1 = D6;
int irPin2 = D7;
int count = 0;
boolean state1 = true;
boolean state2 = true;
boolean insideState = false;
boolean outsideIr = false;
boolean isPeopleExiting = false;
int i = 1;

void setup() {
  Serial.begin(115200);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);


  // Add a new route handler
  server.on("/", handle_OnConnect);
  server.on("/getcount", handle_GetCount);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (!digitalRead(irPin1) && i == 1 && state1) {
    outsideIr = true;
    delay(100);
    i++;
    state1 = false;
  }

  if (!digitalRead(irPin2) && i == 2 && state2) {
    Serial.println("Entering into room");
    outsideIr = true;
    delay(100);
    i = 1;
    count++;
    Serial.print("No of persons inside the room: ");
    Serial.println(count);
    state2 = false;
  }

  if (!digitalRead(irPin2) && i == 1 && state2) {
    outsideIr = true;
    delay(100);
    i = 2;
    state2 = false;
  }

  if (!digitalRead(irPin1) && i == 2 && state1) {
    Serial.println("Exiting from room");
    outsideIr = true;
    delay(100);
    count--;
    Serial.print("No of persons inside the room: ");
    Serial.println(count);
    i = 1;
    state1 = false;
  }

  if (digitalRead(irPin1)) {
    state1 = true;
  }

  if (digitalRead(irPin2)) {
    state2 = true;
  }
}

void handle_GetCount() {
  String response = String(count);
  server.send(200, "text/plain", response);
}

void handle_OnConnect() {
  String html = SendHTML(count);
  server.send(200, "text/html", html);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(int count) {
    String html = "<html>\n";
  html += "<head>\n";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  html += "<title>People Counter</title>\n";
  html += "<style>\n";
  html += "body {\n";
  html += "  font-family: Arial, Helvetica, sans-serif;\n";
  html += "  background: linear-gradient(to bottom, #e6f7ff, #c2e3f3);\n";
  html += "}\n";
  html += "h1 {\n";
  html += "  text-align: center;\n";
  html += "  padding: 10px;\n";
  html += "  background-color: #f0f0f0;\n";
  html += "  border: 2px solid #333;\n";
  html += "  border-radius: 10px;\n";
  html += "  box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);\n";
  html += "  color: #333;\n";
  html += "  display: inline-block;\n";
  html += "}\n";
  html += ".count {\n";
  html += "  font-size: 48px;\n";
  html += "  text-align: center;\n";
  html += "}\n";
  html += ".description {\n";
  html += "  margin: 20px;\n";
  html += "  text-align: center;\n";
  html += "}\n";
  html += ".contact-info {\n";
  html += "  margin-top: 50px;\n";
  html += "  text-align: center;\n";
  html += "  margin-bottom: 20px;\n";
  html += "}\n";
  html += "</style>\n";
  html += "<script>\n";
  html += "function getCount() {\n";
  html += "  var xhttp = new XMLHttpRequest();\n";
  html += "  xhttp.onreadystatechange = function() {\n";
  html += "    if (this.readyState == 4 && this.status == 200) {\n";
  html += "      document.getElementById('count').innerHTML = this.responseText;\n";
  html += "    }\n";
  html += "  };\n";
  html += "  xhttp.open('GET', '/getcount', true);\n";
  html += "  xhttp.send();\n";
  html += "}\n";
  html += "setInterval(getCount, 1000);\n";
  html += "</script>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h1>People Counter</h1>\n";
  html += "<div class='count' id='count'></div>\n";
  html += "<div class='description'>\n";
  html += "<p>A bidirectional counter is used to count the number of people entering a room. The counter increments when someone enters the room and decrements when someone exits. This helps keep track of the number of people currently inside the room.</p>\n";
  html += "<p>Project created by:</p>\n";
  html += "<p>Sarthak Gupta (Registration Number: 21BIT0179)</p>\n";
  html += "<p>Sameekhshya Pati (Registration Number: 21BIT0226)</p>\n";
  html += "<p>Priyanshi Dixit (Registration Number: 21BIT0228)</p>\n";
  html += "</div>\n";
  html += "<div class='contact-info'>\n";
  html += "<h2>Contact Information</h2>\n";
  html += "<p>Email: info@example.com</p>\n";
  html += "<p>Phone: +1234567890</p>\n";
  html += "</div>\n";
  html += "</body>\n";
  html += "</html>\n";
  return html;
}
