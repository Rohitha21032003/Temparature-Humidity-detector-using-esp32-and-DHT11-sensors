#include<WiFi.h>
#include<Wire.h>
#include <DHT.h>
// included libraries
#define DHTTYPE DHT11
uint8_t DHTPin = 4;
// required lines to sense using DHT11 sensor
DHT dht(DHTPin, DHTTYPE);
float temperature; // variable for temperature
float humidity; // variable for humidity

//ssid and password
const char* ssid = "qqqqqqqq";
const char* password = "qqqqqqqq";

WiFiServer server(80); // server

String header;
// setting up wifi
void setup()
{
  Serial.begin(115200);
  pinMode(DHTPin, INPUT);
  dht.begin();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// sensing and displaying temperature and humidity using DHT11 on serial monitor and a web page.
void loop()
{
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  WiFiClient client = server.available();
  if(client)
  {
    // html and css code for making a webpage appealing lookwise and displaying our results.
    String request = client.readStringUntil('\r');
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection:close");
    client.println();

    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel\"icon\" href=\"data:,\">");

    client.println("<style>body {text-align: center; font-family: Garamond, serif;font-size: x-large;}");
    client.println("table { border-collapse: collapse; width:40%; margin-left:auto; margin-right:auto;border-spacing: 2px;background-color: white; border: 4px solid blue; }");
    client.println("th { padding: 20px; background-color: #0000FF; color: white}");
    client.println("tr { border: 5px solid blue; padding: 2px; }");
    client.println("tr: hover {background-color:yellow; }");
    client.println("td { border: 4px; padding: 12px; }");
    client.println(".sensor {color:black; font-weight: bold; background-color: white; padding: 1px; }");

  
    client.println("</style></head></body><h1>ESP32 Web Server Reading sensor value</h1>");
    client.println("<h2>DHT11</h2>");
    client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
    client.println("<tr><td><b>Temp. Celsius</b></td><td><span class=\"sensor\">");
    client.println(dht.readTemperature());
    Serial.println("Temperature Now :");
    Serial.println(dht.readTemperature());
    client.println("<tr><td><b>Humidity</b></td><td><span class=\"sensor\">");
    client.println(dht.readHumidity());
    Serial.println("Humidity Now :");
    Serial.println(dht.readHumidity());
    client.println(" %</span></td></tr>");
    client.println("</body></html>");
   Serial.println("Client disconnected.");
  }
}
