#include <Prometheus.h>

PrometheusExporterBase::PrometheusExporterBase(PrometheusMetric* metrics, size_t capacity) {
    this->metrics = metrics;
    this->capacity = capacity;
}

PrometheusMetric::PrometheusMetric() {}
PrometheusMetric::PrometheusMetric(const __FlashStringHelper* fs, float val) {
    this->metric_name = fs;
    this->value_type = PROMETHEUS_METRIC_TYPE_FLOAT;
    this->value_float = val;
}

PrometheusMetric::PrometheusMetric(const __FlashStringHelper* fs, double val) {
    this->metric_name = fs;
    this->value_type = PROMETHEUS_METRIC_TYPE_DOUBLE;
    this->value_double = val;
}

PrometheusMetric::PrometheusMetric(const __FlashStringHelper* fs, long val) {
    this->metric_name = fs;
    this->value_type = PROMETHEUS_METRIC_TYPE_LONG;
    this->value_long = val;
}

PrometheusMetric::PrometheusMetric(const __FlashStringHelper* fs, unsigned long val) {
    this->metric_name = fs;
    this->value_type = PROMETHEUS_METRIC_TYPE_ULONG;
    this->value_ulong = val;
}

void PrometheusExporterBase::printMetric(Print *p, size_t i) {
    p->print(metrics[i].metric_name);
    p->print(F(" "));
    switch(metrics[i].value_type) {
        case PROMETHEUS_METRIC_TYPE_FLOAT:
            p->print(String(metrics[i].value_float));
            break;
        case PROMETHEUS_METRIC_TYPE_DOUBLE:
            p->print(String(metrics[i].value_double));
            break;
        case PROMETHEUS_METRIC_TYPE_LONG:
            p->print(String((long)metrics[i].value_long));
            break;
        case PROMETHEUS_METRIC_TYPE_ULONG:
            p->print(String((unsigned long)metrics[i].value_ulong));
            break;
    }
    p->print(F("\n"));
}

void PrometheusExporterBase::printMetrics(Print *p) {
    for(size_t i = 0; i<stored_metrics; i++) {
        printMetric(p, i);
    }
}

void PrometheusExporterBase::setMetric(PrometheusMetric m) {
    for(size_t i = 0; i<stored_metrics; i++) {
        if(metrics[i].metric_name == m.metric_name) {
            memcpy(&metrics[i], &m, sizeof(PrometheusMetric));
            return;
        }
    }

    if(stored_metrics == capacity) {
        return;
    }

    memcpy(&metrics[stored_metrics], &m, sizeof(PrometheusMetric));
    stored_metrics++;
}

void PrometheusExporterBase::clearMetrics() {
    memset(metrics, 0, capacity);
    stored_metrics = 0;
}
