
/***************************************************
 This example reads HighTemperature Sensor.
 
 Created 2016-1-13
 By berinie Chen <bernie.chen@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here  https://www.dfrobot.com/wiki/index.php?title=HighTemperatureSensor_SKU:SEN0198
 2.This code is tested on Arduino Uno.
 ****************************************************/
#include<DFRobotHighTemperatureSensor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//----- WiFi settings
const char *ssid = "neo1seok";
const char *password = "wonseokhome";
const char* mqtt_server = "192.168.0.27";
const char* mqtt_topic = "neo_high_temp_check";
char* mqtt_message = "Hello world #%ld";


const float voltageRef = 5.000;       //Set reference voltage,you need test your IOREF voltage. 
//const float voltageRef = 3.300;    
int HighTemperaturePin = A0;	//Setting pin
DFRobotHighTemperature PT100 = DFRobotHighTemperature(voltageRef,11); //Define an PT100 object
//LiquidCrystal_I2C //lcd(0x27, 16, 2);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup(void) {
  Serial.begin(9600);

    // initialize the LCD
  //lcd.begin();

  // Turn on the blacklight and print a message.
  //lcd.backlight();

  //lcd.print("Temp!!!!");
  
  PT100.get_max_value();
  PT100. get_voltageRef();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
     
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

#define STR_FLOAT(a_) String(PT100.a_(),3).c_str()
void loop(void) {
  
  char buffer[512];


  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    
    Serial.print("Publish message: ");
    Serial.println(msg);
    int temperature = PT100.readTemperature(HighTemperaturePin);  //Get temperature
  sprintf(buffer , "%d ^C", temperature);
  //lcd.setCursor(0, 1);
  //lcd.print(buffer);

  sprintf(buffer , 
        "get_max_value:%s  get_voltageRef:%s  get_cur_voltage:%s  get_cur_value:%d get_cur_res:%s  ^C ", 
        STR_FLOAT(get_max_value),STR_FLOAT(get_voltageRef),
        STR_FLOAT(get_cur_voltage) ,PT100.get_cur_value(), STR_FLOAT(get_cur_res ) );
  Serial.println(buffer);

  sprintf(buffer , "temperature: %d ^C n", temperature);
  Serial.println(buffer);
  snprintf (msg, 50, "hello world #%ld", value);
    client.publish(mqtt_topic, buffer);
  }

/*
 *  Serial.print("get_max_value:");
  Serial.println(PT100.get_max_value());

  Serial.print("get_voltageRef:");
  Serial.println(PT100.get_voltageRef());
     Serial.print("get_cur_voltage:");
  Serial.print(PT100.get_cur_voltage());
  Serial.print("  ");
  
     Serial.print("get_cur_value:");
  Serial.print(PT100.get_cur_value());
  Serial.print("  ");

  
     Serial.print("get_cur_res:");
  Serial.print(PT100.get_cur_res());
Serial.print("  ");
  
  
  Serial.print("temperature:  ");
  Serial.print(temperature);*/
 
  //delay(1000); //just here to slow down the output so it is easier to read
}
