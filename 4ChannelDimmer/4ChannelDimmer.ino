#include <RBDdimmer.h> // https://github.com/RobotDynOfficial/RBDDimmer

/*
    ? == you can choose the number
    connect the sync pin of the circuit to the arduino's D2 pin         sync -> D2
    connect 4 wires from potentiometer to arduino pins                  potentiometer -> D?  * 4
    connect 4 wires from the ch1-ch4 of the circuit to arduino pins     ch? -> D?  * 4
*/

const byte POTENTIOIMETER_PINS[] = {A1, A2, A3, A4}; // numbers and amount of potentiometers changeable 
unsigned short potentiometer_values[4];

const byte DIMMER_PINS[] = {6,9,10,11}; //numbers and amount of channels changeable 
dimmerLamp dimmers[] = {dimmerLamp(DIMMER_PINS[0]), dimmerLamp(DIMMER_PINS[1]), dimmerLamp(DIMMER_PINS[2]), dimmerLamp(DIMMER_PINS[3])}; // numbers and amount of lamps changeable 

const byte NUM_OF_LIGHTS = 4; // changeable
const byte MIN_LIGHT = 55; // the minimum percentage of power that goes to the lamps    changeable
const byte MAX_LIGHT = 80; // the maximum percentage of power that goes to the lamps    changeable

// the builtin map() function does not return a float, so this function is the same as the map() however it returns a double
double double_map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (double)(in_max - in_min) + out_min;
}

void setup()
    {
    // debugging
    Serial.begin(9600);

    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        // setup dimmers
        dimmers[i].begin(NORMAL_MODE, ON);
        // setup potentiometer pins
        pinMode(POTENTIOIMETER_PINS[i], INPUT);
    }
    pinMode(6,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
}

void loop()
{
    // get the values
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        potentiometer_values[i] = double_map(analogRead(POTENTIOIMETER_PINS[i]), 1, 1024, MAX_LIGHT, MIN_LIGHT);
        Serial.print(double_map(analogRead(POTENTIOIMETER_PINS[i]), 1, 1024, MAX_LIGHT, MIN_LIGHT));
        Serial.print("\t");
    }
    Serial.println();
    // set the values
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        dimmers[i].setPower(potentiometer_values[i]);
    }
}
