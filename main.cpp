#include "mbed.h"
#include "C12832.h"

class encoder{
    private:
        InterruptIn _channelA;
        InterruptIn _channelB;
        volatile int count;
    public:
        encoder (PinName channelA, PinName channelB): _channelA(channelA), _channelB(channelB) {
            count = 0;
            _channelA.rise(callback(this, &encoder::increment));
            _channelA.fall(callback(this, &encoder::increment));
            _channelB.rise(callback(this, &encoder::increment));
            _channelB.fall(callback(this, &encoder::increment));
        }
    
        void increment(void){count++;}
        void setCount(int value){count = value;}
        int getCount(void){return count;}
};

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

class drive{
    private:
        DigitalOut enable;
        motor *left;
        motor *right;
        encoder *leftEn;
        encoder *rightEn;
    public:
        drive(PinName JP1A_7, motor *left, motor *right, encoder *leftEn, encoder *rightEn)
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
            int leftcount = 0, rightcount = 0;
            
            
            while (leftcount < turn){
                leftcount = leftEn -> getCount();
                rightcount = rightEn -> getCount();
                left -> clockwise(PWM);
                right -> counterclockwise(PWM);
                
                drive::turnon();
            }
            drive::turnoff();
            leftEn -> setCount(0);
            rightEn -> setCount(0);
        }

        void turnleft(float PWM){
            left -> counterclockwise(PWM);
            right -> clockwise(PWM);
            drive:turnon();
        }

        void turn180(float PWM){ 
            left -> clockwise(PWM);
            right -> counterclockwise(PWM);
        }
        
        void turnoff(void){enable = 0;}
};

class display{
  private:
    Ticker t;
    C12832 *lcd;
    encoder *leftEn;
    encoder *rightEn;
  public:  
    display(C12832 *lcd, encoder *leftEn, encoder *rightEn)
    {
           t.attach(callback(this, &display::update), 0.001f);
    }
    void update(void){
        lcd -> locate(3,0);
        lcd -> printf("Left Encoder ticks: %d ", leftEn -> getCount());

        lcd -> locate(3,15);
        lcd -> printf("Right Encoder ticks: %d ", rightEn -> getCount());
    }
};


// main() runs in its own thread in the OS

void update(void);

C12832 lcd(D11, D13, D12, D7, D10);
encoder En_Left(PB_14, PB_13);
encoder En_Right(PC_10, PC_12);

int main(){
    motor left(PB_2, PB_1, PB_15);
    motor right(PA_13, PA_14, PA_15);
    drive buggy(PC_3, &left, &right, &En_Left, &En_Right);
    
    int count = 0;
    /*Ticker t;
    t.attach(&update, 0.001f);*/
    
    while(count < 1150){
        buggy.forward(0.7f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    wait(2);
    En_Right.setCount(0);
    count = 0;
    while(count < 180){
        buggy.turnleft(0.3f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    En_Right.setCount(0);
    count=0;
    wait(2);
    while(count < 1150){
        buggy.forward(0.7f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    wait(2);
    En_Right.setCount(0);
    count = 0;
    while(count < 165){
        buggy.turnleft(0.3f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    En_Right.setCount(0);
    count=0;
    wait(2);
    while(count < 1150){
        buggy.forward(0.7f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    wait(2);
    En_Right.setCount(0);
    count = 0;
    while(count < 180){
        buggy.turnleft(0.3f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    En_Right.setCount(0);
    count=0;
    wait(2);    
    while(count < 1150){
        buggy.forward(0.7f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    buggy.turnoff();
    wait(2);
    En_Right.setCount(0);
    count = 0;
    while(count < 590){
        buggy.turnleft(0.3f);
        count = En_Right.getCount();
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());
        
        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
    }
    count = 0;
    buggy.turnoff();
}

/*void update(void){
        lcd.locate(3,0);
        lcd.printf("Left Encoder ticks: %d ", En_Left.getCount());

        lcd.locate(3,15);
        lcd.printf("Right Encoder ticks: %d ", En_Right.getCount());
}*/
