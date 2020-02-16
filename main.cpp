#include "mbed.h"
#include "C12832.h"

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
            speed.period_us(50);//set PWM frequency to ~ 20kHz
            off();
        }
    
        void setBipol(int value){bipol = value;}//H-bridge
        int getBipol(){return bipol;}
    
        void setDirection(int value){direction = value;}
        int getDirection(){return direction;}
    
        void setSpeed(float dutycycle){speed.write(dutycycle);}//set duty cycle
        float getSpeed(){return speed.read();}
    
        void increment(void){count++;}
        void setCount(int value){count = value;}
        int getCount(void){return count;}
    
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

class drive{
    private:
        motor *left;
        motor *right;
        DigitalOut enable;
    public:
        drive(PinName JP1A_7, motor *left, motor *right)
        : enable(JP1A_7), left(left), right(right){
            drive::turnoff();
        }

        void turnon(void){enable=1;}
        
        void forward(float PWM){ 
            left -> clockwise(PWM);
            right -> clockwise(PWM);
            drive::turnon();
        }

        void reverse(float PWM){
            left -> counterclockwise(PWM);
            right -> counterclockwise(PWM);
            drive::turnon();
        }

        void turnright(float PWM, int turn){
            drive::turnon();
            int leftcount = 0, rightcount = 0;
            while (leftcount < turn || rightcount < turn){
                left -> clockwise(PWM);
                right -> counterclockwise(PWM);
                leftcount = left -> getCount();
                rightcount = right -> getCount();
            }
            drive::turnoff();
            left -> setCount(0);
            left -> setCount(0);
        }

        void turnleft(float PWM){
            left -> off();
            right -> clockwise(PWM);
        }

        void turn180(float PWM){ 
            left -> clockwise(PWM);
            right -> counterclockwise(PWM);
        }
        
        void turnoff(void){enable = 0;}
};

// main() runs in its own thread in the OS

int main(){
    C12832 lcd(D11, D13, D12, D7, D10);
    motor left(PB_9, PB_2, PB_15, PC_9);
    motor right(PB_12, PB_8, PB_7, PC_8);
    drive buggy(PB_1, &left, &right);
    
    buggy.forward(0.5f);
    
    while(1){
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", right.getCount());
    }
}