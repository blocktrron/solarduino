#include <Arduino.h>

#define PROMETHEUS_EXPORTER_HEADER F("text/plain; version=0.0.4")

/**
 * Clss representing a Prometheus Metric
 */
class PrometheusMetric {
    public:
        /**
         * Default constructor for a Prometheus Metric
         */
        PrometheusMetric();

        /**
         * Constructor for a Prometheus Metric
         * @param fs metric name
         * @param vlaue metric value
         */
        PrometheusMetric(const __FlashStringHelper* fs, float value);

        const __FlashStringHelper* metric_name;     ///< Metric name
        float value;                                ///< Metric value
};

/**
 * Base class for a Prometheus Exporter
 */
class PrometheusExporterBase {
    public:
        /**
         * Contructor for a Base Prometheus Exporter
         * @param metrics buffer for stored metrics
         * @param capacity number of storable metrics
         */
        PrometheusExporterBase(PrometheusMetric* metrics, size_t capacity);

        /**
         * Prints a selected metric to an output stream
         * @param p output stream
         * @param i index of metric to print
         */
        void printMetric(Print *p, size_t i);

        /**
         * Prints all available metrics to an output stream
         * @param p output stream
         */
        void printMetrics(Print *p);

        /**
         * Adds/Updates a metric
         * @param m metric to add/update
         */
        void setMetric(PrometheusMetric m);

        /**
         * Drop all stored metrics
         */
        void clearMetrics();
    private:
        size_t stored_metrics = 0;     ///< Amount of stored metrics
        size_t capacity;               ///< Buffer capacity
        PrometheusMetric *metrics;     ///< Pointer to metric buffer
};

/**
 * Class for a fixed sized metric buffer Prometheus Exporter
 */
template <size_t CAPACITY>      ///< Metric buffer size
class StaticPrometheusExporter : public PrometheusExporterBase {
    public:
        /**
         * Default constructor for a static prometheus exporter
         */
        explicit StaticPrometheusExporter() : PrometheusExporterBase(_buffer, CAPACITY) {}

    private:
        PrometheusMetric _buffer[CAPACITY];     ///< Metric buffer
};