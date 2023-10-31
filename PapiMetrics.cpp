#include "PapiMetrics.hpp"

#include <thread>

PapiMetrics::PapiMetrics() {
    int rv = PAPI_library_init(PAPI_VER_CURRENT);
    if ( rv < PAPI_OK ) {
        error.code = rv;
        error.description = "Failed to initialize library";
    }
}

PapiMetrics::~PapiMetrics() {
    //TODO free resources here
}

PapiMetrics::EventSet::EventSet(PapiMetrics& metrics) : metrics{metrics} {
    int rv = PAPI_create_eventset(&eventSet);
    if ( rv != PAPI_OK ) {
        metrics.error.code = rv;
        metrics.error.description = "Failed to create event set";
    }
}

PapiMetrics::EventSet::~EventSet() {
    if ( eventSet != PAPI_NULL ) {
        PAPI_cleanup_eventset(eventSet);
        PAPI_destroy_eventset(&eventSet);
    }
}

PapiMetrics::EventSet PapiMetrics::createEventSet() {
    return EventSet( *this);
}

void PapiMetrics::EventSet::add(int event, const std::string& name) {
    int rv = PAPI_add_event(eventSet, event);
    if ( rv != PAPI_OK ) {
        metrics.error.code = rv;
        metrics.error.description = "Failed to create event set";
    }

    events.push_back(PapiEvent{event, name});
    counters.push_back(0);
}

void PapiMetrics::EventSet::start() {
    int rv = PAPI_start(eventSet);
    if ( rv != PAPI_OK ) {
        metrics.error.code = rv;
        metrics.error.description = "Failed to start event set";
    }
}

void PapiMetrics::EventSet::stop() {
    auto values = counters.data();
    int rv = PAPI_stop(eventSet, values);
    if ( rv != PAPI_OK ) {
        metrics.error.code = rv;
        metrics.error.description = "Failed to stop event set";
    }
}


std::vector<PapiMetricValue> PapiMetrics::measure(EventSet& eventSet, MeasuredFunction function) {
    std::vector<PapiMetricValue> measurements;

    eventSet.start();

    auto ts_start = std::chrono::high_resolution_clock::now();

    // executing the tested function in a thread
    std::thread functionThread{function};

    //TODO query counters here in a loop

    functionThread.join();

    auto ts_end = std::chrono::high_resolution_clock::now();
    // Calculate the duration
    auto duration = std::chrono::duration_cast<MeasureTimeUnit>(ts_end - ts_start).count();
    eventSet.stop();

    for(auto c: eventSet.counters) {
        measurements.push_back(PapiMetricValue{duration, c});
    }
    return measurements;
}
