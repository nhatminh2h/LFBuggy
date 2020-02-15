#include "mbed.h"


class motor{
    private:
    InterruptIn _encoder;
    DigitalOut bipol, direction;
    PwmOut speed;
    int count;

    public:
    motor (PinName JP1A_1_4, PinName JP1A_2_5, PinName JP1A_3_6, PinName encoder) 
            : bipol(JP1A_1_4), direction(JP1A_2_5), speed(JP1A_3_6), _encoder(encoder) 
    {
        count = 0;
        _encoder.rise(callback(this, &motor::increment));
        speed.period_us(300);//set PWM frequency to ~3 kHz
    }

    void setBipol(int value){bipol = value;}//H-bridge
    int getBipol(){return bipol;}

    void setDirection(int value){direction = value;}
    int getDirection(){return direction;}

    void setSpeed(float dutycycle){speed.write(dutycycle);}//set duty cycle
    float getSpeed(){return speed.read();}

    void increment(void){count++;}
    int getcount(void){return count;}

    void clockwise(float dutycycle){
        setBipol(0);//unipolar
        setDirection(1);
        setSpeed(dutycycle);
    }

    void counterclockwise(float PWM){
        setBipol(0);//unipolar
        setDirection(0);
        setSpeed(PWM);
    }

    void off(void){
        setBipol(0);//unipolar
        setDirection(0);
        setSpeed(0);
    }
};

class drive{
    private:
        motor *left;
        motor *right;
        DigitalOut enable;
    public:
        drive(PinName JP1A_7, motor *left, motor *right)
        : enable(JP1A_7), left(left), right(right){
            enable = 1;
        }

        void forward(float PWM){ 
            left -> clockwise(PWM);
            right -> counterclockwise(PWM);
        }

        void reverse(float PWM){ 
            left -> counterclockwise(PWM);
            right -> clockwise(PWM);
        }

        void turnright(float PWM){ 
            left -> clockwise(PWM);
            right -> off();
        }

        void turnleft(float PWM){ 
            left -> off();
            right -> counterclockwise(PWM);
        }

        void turn180(float PWM){ 
            left -> clockwise(PWM);
            right -> counterclockwise(PWM);
        }

};

// main() runs in its own thread in the OS

int main(){
    motor left(PA_5, PA_6, PA_7, PB_6);
    motor right(PC_7, PA_9, PA_8, PB_10);
    drive buggy(PA_12, &left, &right);


    while(1){
        buggy.forward(0.5f);
    }
}