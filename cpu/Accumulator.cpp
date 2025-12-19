class Accumulator {
    double count = 0;
public:
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
    }
    void reset() {
        trigger = false;
        count = 0;
    }
};