#include <RBDdimmer.h> // https://github.com/RobotDynOfficial/RBDDimmer

/*
    ? == you can choose the number
    connect the sync pin of the circuit to the esp32's GPIO pin       sync -> GPIO?
    connect 4 wires from potentiometer to esp32 pins                  potentiometer -> GPIO?  * 4
    connect 4 wires from the ch1-ch4 of the circuit to esp32 pins     ch? -> GPIO?  * 4
*/

const byte POTENTIOIMETER_PINS[] = {13, 12, 27, 14}; // numbers and amount of potentiometers changeable
const byte DIMMER_PINS[] = {5, 18, 19, 21};          // numbers and amount of channels changeable
const byte INTERRUPT_PIN = 34;

const byte NUM_OF_LIGHTS = 4; // changeable
const byte MIN_LIGHT = 55;    // the minimum percentage of power that goes to the lamps    changeable
const byte MAX_LIGHT = 80;    // the maximum percentage of power that goes to the lamps    changeable

dimmerLamp dimmers[] = {
    dimmerLamp(DIMMER_PINS[0], INTERRUPT_PIN),
    dimmerLamp(DIMMER_PINS[1], INTERRUPT_PIN),
    dimmerLamp(DIMMER_PINS[2], INTERRUPT_PIN),
    dimmerLamp(DIMMER_PINS[3], INTERRUPT_PIN)}; // numbers and amount of lamps changeable

double values[4]; // percentage of power [0,100]

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
        Serial.print(analogRead(POTENTIOIMETER_PINS[i]));
        Serial.print(" = ");
        Serial.print(double_map(analogRead(POTENTIOIMETER_PINS[i]), 1, 1024, MAX_LIGHT, MIN_LIGHT));
        Serial.print(" is ");
        Serial.print(dimmers[i].getPower());
        Serial.println();
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
