class encoder{
    private:
        InterruptIn _channelA;
        InterruptIn _channelB;
        int count;
    public:
        encoder (PinName channelA, PinName channelB): _channelA(channelA), _channelB(channelB) {
            count = 0;
            _channelA.rise(callback(this, &motor::increment));
            _channelA.fall(callback(this, &motor::increment));
            _channelB.rise(callback(this, &motor::increment));
            _channelB.fall(callback(this, &motor::increment));
        }
    
        void increment(void){count++;}
        void setCount(int value){count = value;}
        int getcount(void){return count;}
}