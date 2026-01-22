class Accumulator {
   
public:
    int threshold;
    char id;
    bool trigger = false;
    double count = 0;
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