#include "mbed.h"

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