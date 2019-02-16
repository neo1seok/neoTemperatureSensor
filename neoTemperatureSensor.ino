#include <LiquidCrystal_I2C.h>

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

const float voltageRef = 5.000;       //Set reference voltage,you need test your IOREF voltage. 
//const float voltageRef = 3.300;    
int HighTemperaturePin = A0;	//Setting pin
DFRobotHighTemperature PT100 = DFRobotHighTemperature(voltageRef); //Define an PT100 object
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup(void) {
  Serial.begin(9600);

    // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  lcd.print("Temp!!!!");
  
  PT100.get_max_value();
  PT100. get_voltageRef();
     
}
#define STR_FLOAT(a_) String(PT100.a_(),3).c_str()
void loop(void) {
  
  char buffer[512];




  
  int temperature = PT100.readTemperature(HighTemperaturePin);  //Get temperature
  sprintf(buffer , "%d ^C", temperature);
  lcd.setCursor(0, 1);
  lcd.print(buffer);

  sprintf(buffer , 
        "get_max_value:%s  get_voltageRef:%s  get_cur_voltage:%s  get_cur_value:%d get_cur_res:%s  ^C ", 
        STR_FLOAT(get_max_value),STR_FLOAT(get_voltageRef),
        STR_FLOAT(get_cur_voltage) ,PT100.get_cur_value(), STR_FLOAT(get_cur_res ) );
  Serial.println(buffer);

  sprintf(buffer , "temperature: %d ^C n", temperature);
  Serial.println(buffer);
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
 
  delay(1000); //just here to slow down the output so it is easier to read
}
