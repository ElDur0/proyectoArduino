#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN  2        // Pin donde está conectado el sensor DHT11
#define DHTTYPE DHT11   // Tipo de sensor DHT11
#define sensor  A2       //Pin del sensor de tierra   
#define mojado  50
#define seco    70


DHT dht(DHTPIN, DHT11);
const char* ssid = "";          // Reemplaza con el SSID de tu red
const char* password = "";  // Reemplaza con la contraseña de tu red
const char* server = "104.41.25.26";  // IP pública o nombre de dominio de tu servidor

int status = WL_IDLE_STATUS;
WiFiClient client;  // Crear cliente WiFi

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("Iniciando el sensor DHT11...");
  while (!Serial) {
    ; // Esperar a que el puerto serie esté listo
  }

  // Conectarse a la red WiFi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  status = WiFi.begin(ssid, password);

  // Esperar hasta conectarse a la red WiFi
  while (status != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    status = WiFi.status();
  }

  Serial.println("\nConectado a la red WiFi");
}

void loop() {
  // Leer humedad
  float h = dht.readHumidity();
  // Leer temperatura en grados Celsius
  float t = dht.readTemperature();
  //leer la lectura dle suelo;
  int humedadSuelo = analogRead(sensor);
  int lectura = map(humedadSuelo, mojado, seco , 100, 0);
  
  // Comprobar si alguna lectura ha fallado
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT11.");
  } else {
    Serial.println("Sensor de humedad y temperatura");
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
    
  }
  Serial.print("lectura de suelo: ");
  Serial.println(humedadSuelo);
  Serial.println(" porcentaje de humedad en la tierra");
  Serial.print(lectura);
  Serial.println("%");

  if (client.connect(server, 80)) {  // Conectar al servidor en el puerto 5000
    Serial.println("Conectado al servidor");

    // Crear datos en formato JSON
    String jsonData = "{\"sensor_temperatura\": " + String(t, 2) + 
                      ", \"sensor_humedad\": " + String(h, 2) +
                       ", \"sensor_humedad_tierra\":" +String(lectura) +"}";

    Serial.println(jsonData);

    // Enviar solicitud HTTP POST
    client.println("POST /datos HTTP/1.1");
    client.println("Host: 104.41.25.26");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonData.length());
    client.println();
    client.println(jsonData);

    // Esperar la respuesta del servidor
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    client.stop();  // Cerrar la conexión
    Serial.println("Datos enviados con éxito");
  } else {
    Serial.println("Error al conectarse al servidor");
  }

  delay(5000);  // Enviar datos cada 5 segundos
}
