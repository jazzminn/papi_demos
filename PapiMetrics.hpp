#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <functional>

#include <papi.h>

template <typename T, typename V>
struct MetricValue {
    T ts;
    V value;
};

struct PapiEvent {
    int type;
    std::string name;
};

using DurationType = int64_t;
using PapiCounterType = long long;
using PapiMetricValue = MetricValue<DurationType, PapiCounterType>;
using MeasuredFunction = void(void);
using MeasureTimeUnit = std::chrono::microseconds;

/**
 * @brief Papi Metrics Provider
 * 
 * Concept:
 * - 
 */
struct PapiMetrics {
    struct Error {
        int code = PAPI_OK;
        std::string description;
        bool happened() { return code != PAPI_OK; }
    };

    Error error;

    struct EventSet {
        std::vector<PapiCounterType> counters;
        std::vector<PapiEvent> events;

        PapiMetrics& metrics;
        int eventSet = PAPI_NULL;
        EventSet(PapiMetrics& metrics);
        virtual ~EventSet();
        virtual void add(int event, const std::string& name);
        virtual void start();
        virtual void stop();
    };

    PapiMetrics();
    virtual ~PapiMetrics();

    EventSet createEventSet();
    std::vector<PapiMetricValue> measure(EventSet& eventSet, MeasuredFunction function);

    //TODO measurements at fixed periods
    //std::vector<PapiMetricValue> measure(EventSet& eventSet, MeasuredFunction function, IntervalType interval);
};

