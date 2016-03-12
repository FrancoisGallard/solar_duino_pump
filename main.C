/* Solar panel pump controller
 * Starts a pump using a Relay when the temperature sensor
 * of the panel measures a temperature higher than the bottom water tank
 * temperature sensor
 * 4x Temperature Sensors DS18B20
 * 1x Relays (control pump)
 * 1x LCD display 16 characters x 2 rows (display temperatures and statuses)
 * 1x LED (when pump ON)
 * Francois Gallard
 */

/*-----( Import needed libraries )-----*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

/*-----( Declare Constants )-----*/
#define WIRE_BUS_TEMP 7 /*-(Connect to Pin 7 )-*/
#define RELAY_PIN1  8
#define N_TEMP_SENSORS 2
#define ON_PUMP_TRESHOLD 5. /* test tresholds */
#define OFF_PUMP_TRESHOLD 2./* test tresholds */

/*-----( Declare objects )-----*/
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire wireTemp(WIRE_BUS_TEMP);
/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature tempSensors(&wireTemp);
/*Initialize lcd screen */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*-----( Declare Variables )-----*/
/* temperatures array*/
float temperatures[4] = { 0., 0., 0., 0. }; 
String info = "";
bool pumpOn = false;

void setup() /*----( SETUP: RUNS ONCE )----*/
{
Serial.begin(9600);
/*-( Start up the DallasTemperature library )-*/
tempSensors.begin();
/* Sets the pin for the relay */
pinMode(RELAY_PIN1, OUTPUT);
/* set up the LCD's number of columns and rows:*/
lcd.begin(16, 2);
}/*--(end setup )---*/


void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{
/* Send the command to get temperatures*/
tempSensors.requestTemperatures(); 
for(int i=0;i<N_TEMP_SENSORS;i++){
  temperatures[i]=tempSensors.getTempCByIndex(i);
}
/* Build the concatrnated string containing temp infos*/
info="";
for(int i=0;i<N_TEMP_SENSORS;i++){
  info=info+"T"+i+"="+temperatures[i]+" ";
}
Serial.println(info);
//Col 0 line 0
lcd.setCursor(0, 0);
/* Displays info on LCD */
lcd.print(info);

/*Test if we should activate or deactivate the pump*/
if(!pumpOn && temperatures[1]>temperatures[0]+ON_PUMP_TRESHOLD){
  pumpOn=true;
}

if(pumpOn && temperatures[1]<temperatures[0]+OFF_PUMP_TRESHOLD){
  pumpOn=false;
}

/*Write info on pump status*/
if(pumpOn){
  Serial.println("PUMP is ON");
  /*Activate relay*/
  digitalWrite(RELAY_PIN1,0);
  //Col 0 line 1
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("PUMP is ON      ");

}else{
  //Col 0 line 1
  lcd.setCursor(0, 1);
  /*Deactivate relay*/
  lcd.print("PUMP is OFF     ");
  Serial.println("PUMP is OFF     ");
  digitalWrite(RELAY_PIN1,1);
}

}/* --(end main loop )-- */

/* ( THE END ) */
