#include <iostream>
#include "PapiMetrics.hpp"

void test() {
    for(int i=0; i<2000; i++) {
        for(int j=0; j<1000000; j++) {}
        usleep(1000);
    }
}

int main() {
    PapiMetrics papiMetrics;
    
    if ( papiMetrics.error.happened() ) {
        std::cout << "Failed to initialize PAPI metrics: " << papiMetrics.error.description << " - " << papiMetrics.error.code << std::endl;
        return EXIT_FAILURE;
    }
    
    auto eventSet = papiMetrics.createEventSet();
    eventSet.add(PAPI_TOT_CYC, "Cycles");
    eventSet.add(PAPI_TOT_INS, "Instr.");
    eventSet.add(PAPI_L1_ICH, "L1 hits");

    auto measurements = papiMetrics.measure(eventSet, test);
    
    std::cout << "Measurements:" << std::endl;
    std::cout << "Time\t";
    for(auto event: eventSet.events) {
        std::cout << event.name << "\t";        
    }
    std::cout << std::endl;

    std::cout << measurements[0].ts << "\t";
    for(auto m : measurements) {
        std::cout << m.value << "\t";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
