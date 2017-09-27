#include <Wire.h>
#include <BME280I2C.h>
#include <EtherSia.h>
#include <Prometheus.h>
#include <Environment.h>
#include <Logger.h>


/** Serial Baudrate **/
#define SERIAL_BAUD 115200

/** Set update interval of the BME Sensor in milliseconds **/
#define BME_UPDATE_INTERVAL  1000
/** Set BME temperature unit to celsius **/
#define BME_TEMP_CELSIUS true
/** Set BME pressure unit to bar **/
#define BME_PRESSURE_BAR 0x0
/** Set BME height unit to Meters **/
#define BME_ALTITUDE_METER false

/** Last update of BME **/
unsigned long last_bme_update = 0;

/** BME 280 I2C Connection **/
BME280I2C bme;
/** Ethernet Interface **/
EtherSia_ENC28J60 ether;

/** MAC Address **/
MACAddress macAddress("c8:5b:76:f4:21:31");
/** HTTP server */
HTTPServer http(ether);

/** Prometheus exporter **/
StaticPrometheusExporter<10> prometheus;

/** Logger **/
Logger logger(true, true);


void updateBME280Data() {
  Environment env;
  bme.read(env.pressure, env.temperature, env.humidity, BME_TEMP_CELSIUS, BME_PRESSURE_BAR);
  prometheus.setMetric(PrometheusMetric(F("environment_temperature"), env.temperature));
  prometheus.setMetric(PrometheusMetric(F("environment_humidity"), env.humidity));
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    IPv6Prefix p("2000::/3");
    ether.enablePrefixRestriction(&p);
    while(!bme.begin()){
        logger.error(F("Could not find BME280I2C sensor!"));
        logger.blink(0xD4);
    }
    if (ether.begin(macAddress) == false) {
        logger.error(F("Could not initialize Ethernet!"));
        logger.blink(0x99);
    }
    if (logger.serialActive()) {
        ether.globalAddress().println();
        ether.linkLocalAddress().println();
    }
}

void loop() {
    if(millis()-BME_UPDATE_INTERVAL > last_bme_update || millis() < last_bme_update) {
        last_bme_update = millis();
        updateBME280Data();
    }

    ether.receivePacket();
    if (http.isGet(F("/metrics"))) {
        http.printHeaders(PROMETHEUS_EXPORTER_HEADER);
        prometheus.printMetrics(&http);
        http.sendReply();
    } else if (http.havePacket()) {
        http.notFound();
    }

    delay(5);
}
