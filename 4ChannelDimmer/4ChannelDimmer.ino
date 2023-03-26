#include <RBDdimmer.h>

/*
    ? == you can choose the number
    connect the sync pin of the circuit to the arduino's D2 pin         sync -> D2
    connect 4 wires from potentiometer to arduino pins                  potentiometer -> D?  * 4
    connect 4 wires from the ch1-ch4 of the circuit to arduino pins     ch? -> D?  * 4
*/

const byte POTENTIOIMETER_PINS[] = {7, 8, 9, 10}; // changeable numbers and amount of potentiometers
unsigned short potentiometer_values[4];

dimmerLamp dimmers[] = {dimmerLamp(3), dimmerLamp(4), dimmerLamp(5), dimmerLamp(6)}; // changeable numbers and amount of lamps

const byte NUM_OF_LIGHTS = 4;

void setup()
{
    // debugging
    Serial.begin(9600);

    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        // setup dimmers
        dimmers[i].begin(NORMAL_MODE, ON);
        // setup potentiometer pins
        pinMode(POTENTIOIMETER_PINS[i], OUTPUT);
    }
}

void loop()
{
    // get the values
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        potentiometer_values[i] = map(analogRead(POTENTIOIMETER_PINS[i]), 1, 1024, 100, 0);
    }
    // set the values
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        dimmers[i].setPower(potentiometer_values[i]);
    }
}