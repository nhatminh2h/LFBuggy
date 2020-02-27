#include "mbed.h"
#include "C12832.h"
#include "mtrDrive.cpp"

// main() runs in its own thread in the OS

int main(){
    motor left(PB_2, PB_1, PB_15);
    motor right(PA_13, PA_14, PA_15);
    encoder En_Left(PB_14, PB_13);
    encoder En_Right(PC_10, PC_12);
    drive buggy(PC_3, &left, &right, &En_Left, &En_Right);
    //C12832 lcd(D11, D13, D12, D7, D10);
    //display disp(lcd, En_Left, En_Right);

}