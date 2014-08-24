#include <rxduino.h>
#include "tsurai.h"

//#define DEBUG__
#define Speak_Enable__

const int masterSpeed = 60;
double command[64][3] = {{0, 0, 0}};
int stopline = 0;
const char speakSent[][50] = {
"anna'iwohajimeruyo.",
"kokodayo.",
"yoi'syo,ko'rasyo.",
"po'ntaha,bo'kuno/tomodati.",
"oyama'shi/tyu-o-/tosyokanwa,suki?",
"toki'doki'/po'ntato/asobuyo.",
"bo'kuno/namaewa,u'satan/dayo-.",
"oyamako'-senwo/yorosikune.",
"kyo'uwa/iite'nkidane."
"huna'ssi'-.",
"nya'npasu'-.",
"arinomamade."
};

const int sensor[6] = {8, 9, 10, 11, 12, 13};

Motor motorL(2, 3, 4);
Motor motorR(5, 6, 7);

void setup(){
#ifdef DEBUG__
    Serial.begin(9600, SCI_USB0);
#endif
#ifdef Speak_Enable__
    Serial1.begin(9600,SCI_SCI2A);
#endif
    pinMode(PIN_LED0,OUTPUT);
    pinMode(PIN_LED1,OUTPUT);
    pinMode(PIN_LED2,OUTPUT);
    pinMode(PIN_LED3,OUTPUT);
    pinMode(PIN_SW, INPUT);
    pinMode(sensor[0], INPUT);
    pinMode(sensor[1], INPUT);
    pinMode(sensor[2], INPUT);
    pinMode(sensor[3], INPUT);
    pinMode(sensor[4], INPUT);
    pinMode(sensor[5], INPUT);
    analogWriteFrequency(4000);
    motorL.breakMotor();
    motorR.breakMotor();
    delay(200);
//#ifdef Speak_Enable__
    Serial1.print('?');
    while(Serial1.read() != '>'){
#ifdef DEBUG__
        Serial.println("waiting speakInit...");
        delay(200);
#endif
    }
//#endif

    while(1){
        if(!digitalRead(PIN_SW)){
            unsigned long s_time = millis();
            while(!digitalRead(PIN_SW));
            unsigned long e_time = millis();
            if(e_time - s_time < 1000){
                stopline++;
                for(int i = 0; i < stopline; i++){
                    digitalWrite(PIN_LED0, HIGH);
                    delay(100);
                    digitalWrite(PIN_LED0, LOW);
                    delay(100);
                }
            }else{
                break;
            }
        }
    }
            

command[0x3f][0] = 1;
command[0x3f][1] = 0.00;
command[0x3f][2] = 0.00;

command[0x3e][0] = 1;
command[0x3e][1] = 0.00;
command[0x3e][2] = 1.00;

command[0x3c][0] = 1;
command[0x3c][1] = 0.00;
command[0x3c][2] = 1.00;

command[0x3d][0] = 1;
command[0x3d][1] = 0.00;
command[0x3d][2] = 1.00;

command[0x39][0] = 1;
command[0x39][1] = 0.00;
command[0x39][2] = 1.00;

command[0x3b][0] = 1;
command[0x3b][1] = 0.00;
command[0x3b][2] = 1.00;

command[0x33][0] = 1;
command[0x33][1] = 1.00;
command[0x33][2] = 1.00;

command[0x37][0] = 1;
command[0x37][1] = 1.00;
command[0x37][2] = 0.00;

command[0x27][0] = 1;
command[0x27][1] = 1.00;
command[0x27][2] = 0.00;

command[0x2f][0] = 1;
command[0x2f][1] = 1.00;
command[0x2f][2] = 0.00;

command[0x0f][0] = 1;
command[0x0f][1] = 1.00;
command[0x0f][2] = 0.00;

command[0x1f][0] = 1;
command[0x1f][1] = 1.00;
command[0x1f][2] = 0.00;

command[0x00][0] = 2;
command[0x00][1] = 1;
command[0x00][2] = 0;

    digitalWrite(PIN_LED1, HIGH);
    
#ifdef Speak_Enable__
    Serial1.print("konnitiwa");
    Serial1.write(0x0d);
    digitalWrite(PIN_LED3, HIGH);
    while(Serial1.read() != '>');
    digitalWrite(PIN_LED3, LOW);
#endif
}

bool onlineflag = false;
int count = 0;
bool state = false, old_state = false;
unsigned long distance = 0;
int speaknum = 0, oldspeaknum = 0;

void loop(){
    unsigned int val = 0;
    
    for(int i = 5; i >= 0; i--){
        val = (val<<1) + !digitalRead(sensor[i]);
    }
    if(val != 0x00){
        onlineflag = false;
    }
#ifdef DEBUG__
    Serial.print("val(HEX):");
    Serial.println(val, HEX);
#endif
    
    switch((int)command[val][0]){
        case 1:
            if((int)command[val][1] == 0){
                motorL.breakMotor();
            }else{
                motorL.driveMotor((int)((double)masterSpeed * command[val][1]));
            }
            if((int)command[val][2] == 0){
                motorR.breakMotor();
            }else{
                motorR.driveMotor((int)((double)masterSpeed * command[val][2]));
            }
#ifdef DEBUG__
            char temp_deb[100];
            sprintf(temp_deb, "MotorL:%3d MotorR:%3d", (int)((double)masterSpeed * command[val][1]), (int)((double)masterSpeed * command[val][2]));
            Serial.println(temp_deb);
#endif
            if(((int)((double)masterSpeed * command[val][1]) || (int)((double)masterSpeed * command[val][2])) && state == false){
                Serial1.print(speakSent[0]);
                Serial1.write(0x0d);
                state = true;
            }
            if(millis() - distance > 5000 && ((int)((double)masterSpeed * command[val][1]) || (int)((double)masterSpeed * command[val][2]))){
                randomSeed((unsigned int)(millis() % 0xff));
                while(speaknum == oldspeaknum){
                    speaknum = 2 + random() % (sizeof(speakSent)/sizeof(speakSent[0]) - 2);
                }
                oldspeaknum = speaknum;
                Serial1.print(speakSent[speaknum]);
                Serial1.write(0x0d);
                digitalWrite(PIN_LED3, HIGH);
                distance = millis();
            }
            break;
            
#ifdef Speak_Enable__
        case 2:
            if((int)command[val][1] == 1){
                if(onlineflag == false){
                    count++;
                    onlineflag = true;
                }
                if(count == stopline || count == 4){
                    if(command[val][2] == 0){
                        Serial1.print(speakSent[1]);
                        Serial1.write(0x0D);
                        command[val][2] = 1;
                    }
                    motorL.breakMotor();
                    motorR.breakMotor();
                }
            }
            break;
#endif
    }

#ifdef Speak_Enable__
    if(Serial1.read() == '>'){
        digitalWrite(PIN_LED3, LOW);
    }
#endif
    delay(10);
}

