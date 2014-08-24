#ifndef TSURAI_H__
#define TSURAI_H__

class Motor{
    public:
        Motor(const short FWD, const short BCK, const short PWM);
        void driveMotor(const int speed);
        void breakMotor();
    private:
        const short PIN_FWD, PIN_BCK, PIN_PWM;
        int nowSpeed;
};

class Sensor{
    public:
        Sensor(const short* PIN, const int num);
        byte read();
    private:
        const short *PIN_SEN;
        int sensorNum;
};

#endif

