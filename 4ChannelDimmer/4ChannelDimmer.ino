#include <RBDdimmer.h> // https://github.com/RobotDynOfficial/RBDDimmer

/*
    ? == you can choose the number
    connect the sync pin of the circuit to the arduino's D2 pin         sync -> D2
    connect 4 wires from potentiometer to arduino pins                  potentiometer -> D?  * 4
    connect 4 wires from the ch1-ch4 of the circuit to arduino pins     ch? -> D?  * 4
*/

const byte POTENTIOIMETER_PINS[] = {A1, A2, A3, A4}; // numbers and amount of potentiometers changeable
const byte DIMMER_PINS[] = {6, 9, 10, 11};           // numbers and amount of channels changeable

const byte NUM_OF_LIGHTS = 4; // changeable
const byte MIN_LIGHT = 55;    // the minimum percentage of power that goes to the lamps    changeable
const byte MAX_LIGHT = 80;    // the maximum percentage of power that goes to the lamps    changeable

dimmerLamp dimmers[] = {dimmerLamp(DIMMER_PINS[0]), dimmerLamp(DIMMER_PINS[1]), dimmerLamp(DIMMER_PINS[2]), dimmerLamp(DIMMER_PINS[3])}; // numbers and amount of lamps changeable
double values[4];                                                                                                                        // percentage of power [0,100]

// the builtin map() function does not return a float, so this function is the same as the map() however it returns a double
double double_map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (double)(in_max - in_min) + out_min;
}

// promijeni ovu funkciju u svoju
void get_values()
{
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        values[i] = double_map(analogRead(POTENTIOIMETER_PINS[i]), 1, 1024, MAX_LIGHT, MIN_LIGHT);

        // debugging
        Serial.print(double_map(analogRead(POTENTIOIMETER_PINS[i]), 1, 1024, MAX_LIGHT, MIN_LIGHT));
        Serial.print("\t");
    }
    Serial.println();
}

void setup()
{
    // debugging
    Serial.begin(9600);

    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        // setup dimmers
        dimmers[i].begin(NORMAL_MODE, ON);
        pinMode(DIMMER_PINS[i], OUTPUT);

        // setup potentiometer pins
        pinMode(POTENTIOIMETER_PINS[i], INPUT);
    }
}

void loop()
{
    // get the values
    get_values();

    // set the values
    for (byte i = 0; i < NUM_OF_LIGHTS; i++)
    {
        dimmers[i].setPower(values[i]);
    }
}
