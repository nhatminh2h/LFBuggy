#include "Motors.cpp"
#include "encoder.cpp"

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

        void turnon(void){enable = 1;}
        void turnoff(void){enable = 0;}
        
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
            left -> clockwise(PWM);
            right -> counterclockwise(PWM);
            
            drive::turnon();
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
        
};