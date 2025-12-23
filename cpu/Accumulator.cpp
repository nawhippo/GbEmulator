class Accumulator {
    double count = 0;
public:
    //this is a huge number of instruction it has to be scaled according to masterclock/ subdivision * pace
    int threshold;
    char id;
    bool trigger = false;
    Accumulator(char id, int threshold = 0)
        : threshold(threshold), id(id) {
    }
    
    void tick(double amount){
        count += amount;
        if (count >= threshold){
            trigger = true;
            count = 0;
        }
        if (count < 0){
            count = 0;
    }
    }
    void reset() {
        trigger = false;
        count = 0;
    }
};