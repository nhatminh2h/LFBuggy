#include "mbed.h"


class motor{
    private:
    InterruptIn encoder;
    DigitalOut bipol, direction;
    PwmOut speed;

    public:
    motor (PinName bipol, PinName direction, PinName speed, PinName encoder) 
            : bipol(bipol), direction(direction), speed(speed), encoder(encoder) 
    {
        //make interrupt
    }

    void setBipol(int value){bipol = value;}//H-bridge
    int getBipol(){return bipol;}

    void setDirection(int value){direction = value;}
    int getDirection(){return direction;}

    void setSpeed(float PWM){speed.write(PWM);}//set duty cycle
    float getSpeed(){return speed.read();}

    void forward(int PWM){
        setBipol(1);
        setDirection(0);
        setSpeed(PWM);
    }

    void backward(int PWM){
        setBipol(0);
        setDirection(1);
        setSpeed(PWM);
    }

    void off(void){
        setBipol(0);
        setDirection(0);
    }
};

class drive{
    private:
        motor *left;
        motor *right;
    public:
        drive(motor *left, motor *right): left(left), right(right){}

        void forward(int PWM){ 
            left -> forward(PWM);
            right -> forward(PWM);
        }

        void reverse(int PWM){ 
            left -> backward(PWM);
            right -> backward(PWM);
        }

        void turnright(int PWM){ 
            left -> forward(PWM);
            right -> off();
        }

        void turnleft(int PWM){ 
            left -> off();
            right -> forward(PWM);
        }

        void turn180(int PWM){ 
            left -> forward(PWM);
            right -> backward(PWM);
        }

};

// main() runs in its own thread in the OS

int main(){
    motor left(PA_5, PA_6, PA_7, PB_6);
    motor right(PC_7, PA_9, PA_8, PB_10);
    drive buggy(&left, &right);


    while(1){
        //run buggy here
    }



}

