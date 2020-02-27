#include "mbed.h"

class motor{
    private:
        DigitalOut bipol, direction;
        PwmOut speed;

    public:
        motor (PinName JP1A_1_4, PinName JP1A_2_5, PinName JP1A_3_6) 
                : bipol(JP1A_1_4), direction(JP1A_2_5), speed(JP1A_3_6) 
        {
            speed.period_us(50);//set PWM frequency to ~ 20kHz
            off();
        }
    
        void setBipol(int value){bipol = value;}//H-bridge
        int getBipol(){return bipol;}
    
        void setDirection(int value){direction = value;}
        int getDirection(){return direction;}
    
        void setSpeed(float dutycycle){speed.write(dutycycle);}//set duty cycle
        float getSpeed(){return speed.read();}
    
        void clockwise(float dutycycle){
            setBipol(0);//unipolar
            setDirection(1);
            setSpeed(dutycycle);
        }
    
        void counterclockwise(float dutycycle){
            setBipol(0);//unipolar
            setDirection(0);
            setSpeed(dutycycle);
        }
    
        void off(void){
            setBipol(0);//unipolar
            setDirection(1);
            setSpeed(1);
        }
};


