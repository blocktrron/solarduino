#include <Arduino.h>

#define PROMETHEUS_EXPORTER_HEADER F("text/plain; version=0.0.4")
#define PROMETHEUS_METRIC_TYPE_FLOAT 1
#define PROMETHEUS_METRIC_TYPE_DOUBLE 2
#define PROMETHEUS_METRIC_TYPE_LONG 3
#define PROMETHEUS_METRIC_TYPE_ULONG 4

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
         * Constructor for a float Prometheus Metric
         * @param fs metric name
         * @param vlaue metric value
         */
        PrometheusMetric(const __FlashStringHelper* fs, float val);

        /**
         * Constructor for a double Prometheus Metric
         * @param fs metric name
         * @param vlaue metric value
         */
        PrometheusMetric(const __FlashStringHelper* fs, double val);

        /**
         * Constructor for a long Prometheus Metric
         * @param fs metric name
         * @param vlaue metric value
         */
        PrometheusMetric(const __FlashStringHelper* fs, long val);

        /**
         * Constructor for a ulong Prometheus Metric
         * @param fs metric name
         * @param vlaue metric value
         */
        PrometheusMetric(const __FlashStringHelper* fs, unsigned long val);

        const __FlashStringHelper* metric_name;     ///< Metric name
        uint8_t value_type;
        union {
            float value_float;          ///< Metric value (float)
            double value_double;        ///< Metric value (double)
            long value_long;            ///< Metric value (long)
            unsigned long value_ulong;  ///< Metric value (unsigned long)
        };      ///< Metric value
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