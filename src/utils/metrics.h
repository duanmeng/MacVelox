#pragma once

struct Metrics {
    int numMetrics = 0;

    long* inputRows;
    long* inputVectors;
    long* inputBytes;
    long* rawInputRows;
    long* rawInputBytes;
    long* outputRows;
    long* outputVectors;
    long* outputBytes;

    // CpuWallTiming.
    long* count;
    long* wallNanos;
    long veloxToArrow;

    long* peakMemoryBytes;
    long* numMemoryAllocations;

    // Runtime metrics.
    long* numDynamicFiltersProduced;
    long* numDynamicFiltersAccepted;
    long* numReplacedWithDynamicFilterRows;

    Metrics(int size) : numMetrics(size) {
        inputRows = new long[numMetrics]();
        inputVectors = new long[numMetrics]();
        inputBytes = new long[numMetrics]();
        rawInputRows = new long[numMetrics]();
        rawInputBytes = new long[numMetrics]();
        outputRows = new long[numMetrics]();
        outputVectors = new long[numMetrics]();
        outputBytes = new long[numMetrics]();
        count = new long[numMetrics]();
        wallNanos = new long[numMetrics]();
        peakMemoryBytes = new long[numMetrics]();
        numMemoryAllocations = new long[numMetrics]();
        numDynamicFiltersProduced = new long[numMetrics]();
        numDynamicFiltersAccepted = new long[numMetrics]();
        numReplacedWithDynamicFilterRows = new long[numMetrics]();
    }

    ~Metrics() {
        delete[] inputRows;
        delete[] inputVectors;
        delete[] inputBytes;
        delete[] rawInputRows;
        delete[] rawInputBytes;
        delete[] outputRows;
        delete[] outputVectors;
        delete[] outputBytes;
        delete[] count;
        delete[] wallNanos;
        delete[] peakMemoryBytes;
        delete[] numMemoryAllocations;
    }
};
