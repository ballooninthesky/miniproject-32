#include <Arduino.h>
#include <ESP32Servo.h>

int RAIN_SENSOR_PIN= 19;    //D19
int LDR_PIN=32;             //D32
int SERVO_PIN=4;            //D4
int LIGHT_THRESHOLD =300;

Servo myservo;

const int IDLE = 0;
const int RAINING = 1;
const int SUNSHINE = 2;
int state = IDLE;
int servoPosition = 0;  // 0 = closed, 1 = open

void setup() {
    pinMode(RAIN_SENSOR_PIN, INPUT);
    pinMode(LDR_PIN, INPUT);
    myservo.attach(SERVO_PIN);
    myservo.write(0);
    servoPosition = 0;
    Serial.begin(115200);
}

void loop() {
    int rainDetected = digitalRead(RAIN_SENSOR_PIN);
    int lightValue = analogRead(LDR_PIN);

    if (state == IDLE) {
        Serial.println("IDLE MODE");
        Serial.print("lightValue = ");
        Serial.println(LDR_PIN);
        delay(1000);

        if (servoPosition == 0) {
            Serial.println("Servo is CLOSED");
            delay(1000);
        } else {
            Serial.println("Servo is OPEN");
            delay(1000);
        }

        if (rainDetected == HIGH) {
            state = RAINING;
        } 
        else if (lightValue > LIGHT_THRESHOLD && rainDetected == HIGH) {
            state = SUNSHINE;
        }
    }

    else if (state == RAINING) {
        Serial.println("RAINING MODE");
        delay(1000);

        if (servoPosition != 0) {
            myservo.write(0);
            servoPosition = 0;
            Serial.println("Servo is now CLOSED due to rain");
            delay(1000);
        }

        if (rainDetected == LOW) {
            state = IDLE;
        }
    }

    else if (state == SUNSHINE) {
        Serial.println("SUNSHINE MODE");
        delay(1000);

        if (servoPosition != 1) {
            myservo.write(90);
            servoPosition = 1;
            Serial.println("Servo is now OPEN due to sunshine");
            delay(1000);
        }

        if (rainDetected == LOW) {
            state = IDLE;
        }
    }
}
