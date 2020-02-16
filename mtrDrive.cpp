class drive{
    private:
        motor *left;
        motor *right;
        DigitalOut enable;
    public:
        drive(PinName JP1A_7, motor *left, motor *right)
        : enable(JP1A_7), left(left), right(right){
            drive::off();
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
                leftcount = left -> getcount();
                rightcount = right -> getcount();
            }
            drive::off();
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