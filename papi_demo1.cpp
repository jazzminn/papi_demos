#include <iostream>

#include <papi.h>

double calculate() {
    double x;
    for(long i=0; i<100000; i++) {
        // do nothing
        x+=(double)i*2.1;
    }
    return x;
}

int main() {
    std::cout << "PAPI Demo v1" << std::endl;
    long long counter = 0;

    PAPI_library_init(PAPI_VER_CURRENT);

    int eventSet = PAPI_NULL;
    int rv = PAPI_create_eventset(&eventSet);
    if ( rv != PAPI_OK ) {
        std::cerr << "Failed to create event set: " << rv << std::endl;
        exit(EXIT_FAILURE);
    }

    rv = PAPI_add_event(eventSet, PAPI_TOT_CYC);
    if ( rv != PAPI_OK ) {
        std::cerr << "Failed to add event PAPI_TOT_CYC: " << rv << std::endl;
        exit(EXIT_FAILURE);
    }

    rv = PAPI_start(eventSet);
    if ( rv != PAPI_OK ) {
        std::cerr << "Failed to start PAPI measurement: " << rv << std::endl;
        exit(EXIT_FAILURE);
    }

    calculate();

    rv = PAPI_stop(eventSet, &counter);
    if ( rv != PAPI_OK ) {
        std::cerr << "Failed to start PAPI measurement: " << rv << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Measured PAPI_TOT_CYC: " << counter << std::endl;

    return EXIT_SUCCESS;
}
