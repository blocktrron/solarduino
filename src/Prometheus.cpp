#include <Prometheus.h>

PrometheusExporterBase::PrometheusExporterBase(PrometheusMetric* metrics, size_t capacity) {
    this->metrics = metrics;
    this->capacity = capacity;
}

PrometheusMetric::PrometheusMetric() {}
PrometheusMetric::PrometheusMetric(const __FlashStringHelper* fs, float value) {
    this->metric_name = fs;
    this->value = value;
}

void PrometheusExporterBase::printMetric(Print *p, size_t i) {
    p->print(metrics[i].metric_name);
    p->print(F(" "));
    p->print(String(metrics[i].value));
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
