#include <rxduino.h>
#include <stdlib.h>
#include "tsurai.h"

//class Motor

Motor::Motor(const short FWD, const short BCK, const short PWM): PIN_FWD(FWD), PIN_BCK(BCK), PIN_PWM(PWM){
    pinMode(PIN_FWD, OUTPUT);
    pinMode(PIN_BCK, OUTPUT);
    pinMode(PIN_PWM, OUTPUT);
    breakMotor();
}
    
void Motor::driveMotor(int speed){
    if(nowSpeed == speed){
        return;
    }
    if(speed > 0 && (!digitalRead(PIN_FWD) || digitalRead(PIN_BCK))){
        analogWrite(PIN_PWM, 0);
        delay(1);
        digitalWrite(PIN_BCK, LOW);
        digitalWrite(PIN_FWD, HIGH);
        delay(1);
    }else if(speed < 0 && (digitalRead(PIN_FWD) || !digitalRead(PIN_BCK))){
        analogWrite(PIN_PWM, 0);
        delay(1);
        digitalWrite(PIN_FWD, LOW);
        digitalWrite(PIN_BCK, HIGH);
        delay(1);
    }
    analogWrite(PIN_PWM, abs(speed));
    nowSpeed = speed;
}

void Motor::breakMotor(){
    analogWrite(PIN_PWM, 0);
    delay(1);
    digitalWrite(PIN_FWD, LOW);
    digitalWrite(PIN_BCK, LOW);
    nowSpeed = 0;
}


// class Sensor

Sensor::Sensor(const short* PIN, const int num): PIN_SEN(PIN), sensorNum(num){
    if(sensorNum > 8){
        sensorNum = 8;
    }
    for(int i = 0; i < sensorNum; i++){
        pinMode(PIN_SEN[i], INPUT);
    }
}

byte Sensor::read(){
    byte state = 0;
    
    for(int i = 0; i < sensorNum; i++){
        int high = 0;
        for(int j = 0; j < 10; j++){
            if(digitalRead(PIN_SEN[i]) == HIGH){
                high++;
            }
        }
        if(high > 5){
            state += (0x01 << i);
        }
    }
    
    return 31 - state;
}

