#include <iostream>

int main() {
    std::cout << "C++ Standard: " << __cplusplus << std::endl;
    
    #ifdef __GNUC__
        std::cout << "GCC version: " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
    #endif
    
    #ifdef _MSC_VER
        std::cout << "MSVC version: " << _MSC_VER << std::endl;
    #endif
    
    return 0;
}