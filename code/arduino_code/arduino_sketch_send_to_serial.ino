
// use this sketch with this R script:
//  /path_to_file/R_code_receive_serial_plot.R

//**** ads1115 hookup: vcc to 5V, gnd to gnd, A0 to white wire from sliding resistor
// addr to gnd (0x48) or addr to vcc (0x49), scl to A5, sda to A4

//**** sliding variable resistor hookup (added additonal resistor to reduce total voltage range to about 2V (100K on pin1 and 12K on pin3)
// 5v to pin 1 on sliding resistor **via 100K ohm resistor**  12/1/2022 -- using 10K resistors on both gnd and vcc connections
// gnd to pin 3 on sliding resistor **via 12K ohm resistor**  12/1/2022 -- using 10K resistors on both gnd and vcc connections
// A0 on ADS1115 to pin 2 on sliding resistor


#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads_sensor_1(0x48);
// Adafruit_ADS1115 ads_sensor_2(0x49);

float volt_conversion = 0.000125;  // conversion values are given at bottom of sketch

// for recieved signal from R
char receivedChar;


void setup(void) {
  Wire.begin();
  Serial.begin(9600);
  ads_sensor_1.begin();
  // set gain
  ads_sensor_1.setGain(GAIN_ONE);
}


void loop(void) {

  // check to see if any data is recieved from R. If data is recieved newData is set to true
  recvOneChar();  // run this "read input from R" fxn (code at bottom)

  if(isAlpha(receivedChar)==true) {

    // sample voltage from sliding resistors
    // loop thought voltage read many times to get a stable mean value

    // using ads_sensor_1 ADC for sensors 1 to 4
    // do sensor 1
    float store_mean1 = ads_sensor_1.readADC_SingleEnded(0);
    for (int i=1; i<10; i++) {
      float foo1_sensor = ads_sensor_1.readADC_SingleEnded(0); // read sensor on pin A0
      store_mean1 = store_mean1 + foo1_sensor;
    }; //delay(1000);

    // do sensor 2
    float store_mean2 = ads_sensor_1.readADC_SingleEnded(1);
    for (int i=1; i<10; i++) {
      float foo2_sensor = ads_sensor_1.readADC_SingleEnded(1); // read sensor on pin A0
      store_mean2 = store_mean2 + foo2_sensor;
    }; //delay(1000);

    // do sensor 3
    float store_mean3 = ads_sensor_1.readADC_SingleEnded(2);
    for (int i=1; i<10; i++) {
      float foo3_sensor = ads_sensor_1.readADC_SingleEnded(2); // read sensor on pin A0
      store_mean3 = store_mean3 + foo3_sensor;
    }; //delay(1000);

    // do sensor 4
    float store_mean4 = ads_sensor_1.readADC_SingleEnded(3);
    for (int i=1; i<10; i++) {
      float foo4_sensor = ads_sensor_1.readADC_SingleEnded(3); // read sensor on pin A0
      store_mean4 = store_mean4 + foo4_sensor;
    }; //delay(1000);

    // calculate means and voltages for all sensors
    store_mean1 = store_mean1 / 10; // get mean value of all measurements in for loop (use "x" in i<x above)
    store_mean2 = store_mean2 / 10;
    store_mean3 = store_mean3 / 10;
    store_mean4 = store_mean4 / 10;

    // convert to voltate
    store_mean1 = (store_mean1 * volt_conversion);
    store_mean2 = (store_mean2 * volt_conversion);
    store_mean3 = (store_mean3 * volt_conversion);
    store_mean4 = (store_mean4 * volt_conversion);

    //_____ print date and time to serial
    // add voltage data
    // Serial.print(" ");
    Serial.print(store_mean1, 7);
    Serial.print(" ");
    Serial.print(store_mean2, 7);
    Serial.print(" ");
    Serial.print(store_mean3, 7);
    Serial.print(" ");
    Serial.println(store_mean4, 7);

    receivedChar = 1;  // set char to numeric (signal sent from R will change to a letter when recieved)
  }
//   delay(2000);
}


// this reads input from R (write.serialConnection fxn)
void recvOneChar() {
  if (Serial.available() > 0) {
     receivedChar = Serial.read();
  }
}

  /*
  //int16_t adc0;  // we read from the ADC, we have a sixteen bit integer as a result
  //[sean] uncomment one of the lines below, based on gain set above and expected voltage range
  adc0 = ads.readADC_SingleEnded(0);
  Voltage = (adc0 * 0.0000625);      // for GAIN_TWOTHIRDS,     +/- 6.144V
  // Voltage = (adc0 * 0.000125)       // for GAIN_ONE,        +/- 4.096V
  // Voltage = (adc0 * 0.0000625)      // for GAIN_TWO,        +/- 2.048V
  // Voltage = (adc0 * 0.00003125)     // for GAIN_FOUR,       +/- 1.024V
  // Voltage = (adc0 * 0.000015625)    // for GAIN_EIGHT,      +/- 0.512V
  // Voltage = (adc0 * 0.00000078125)  // for GAIN_SIXTEEN,    +/- 0.256V
  */
