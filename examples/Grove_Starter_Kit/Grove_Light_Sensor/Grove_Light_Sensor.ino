// demo of Grove - Light Sensor

const int pinLight = A0;


void setup()
{
    Serial.begin(9600);
    //wait for serial monitor to start
    while(!Serial)
        delay(100);
}

void loop()
{
    int value = analogRead(pinLight);
    Serial.println(value);
    delay(100);
}
