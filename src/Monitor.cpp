#include <Wire.h>
#include <SoftwareSerial.h>
#include <BME280I2C.h>
#include <VeDirectParser.h>
#include <VeDirectPrometheusMapping.h>
#include <EtherSia.h>
#include <Prometheus.h>
#include <Environment.h>
#include <Logger.h>

#ifndef MAC_ADDRESS
#define MAC_ADDRESS "de:ad:be:ef:01:02"
#endif

/** Serial Baudrate **/
#define SERIAL_BAUD 115200
/** MPPT Baudrate **/
#define MPPT_BAUD 19200

/** Task code for MPPT update processing **/
#define TASK_UPDATE_MPPT 1
/** Task code for ethernet packet processing **/
#define TASK_PROCESS_ETHERNET 0

/** Set update delay of the MPPT Sensor in milliseconds **/
#define MPPT_UPDATE_DELAY  40000
/** Set update interval of the MPPT Sensor in milliseconds **/
#define MPPT_UPDATE_INTERVAL  30000
/** Set update delay of the MPPT Sensor in milliseconds **/
#define MPPT_UPDATE_TIMEOUT  40000

/** Last update of MPPT **/
unsigned long last_mppt_update = 0;

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

/** Last task-switch **/
unsigned long last_task_switch = 0;

/** Software Serial for MPPT sensor **/
SoftwareSerial mpptSerial(14, 15);

/** BME 280 I2C Connection **/
BME280I2C bme;
/** Ethernet Interface **/
EtherSia_ENC28J60 ether;

/** MAC Address **/
MACAddress macAddress(MAC_ADDRESS);
/** HTTP server */
HTTPServer http(ether);

/** VeDirect Parser **/
VeDirectParser veParser;

/** Prometheus exporter **/
StaticPrometheusExporter<15> prometheus;

/** Lines to parse until MPPT message is parsed **/
uint16_t lines_to_parse = 0;
/** Current job the board is performing **/
uint8_t currentTask = TASK_PROCESS_ETHERNET;

/** Logger **/
Logger logger(true, true);


void updateBME280Data() {
  Environment env;
  bme.read(env.pressure, env.temperature, env.humidity, BME_TEMP_CELSIUS, BME_PRESSURE_BAR);
  prometheus.setMetric(PrometheusMetric(F("environment_temperature"), env.temperature));
  prometheus.setMetric(PrometheusMetric(F("environment_humidity"), env.humidity));
}

void switchTask(uint8_t task) {
    last_task_switch = millis();
    currentTask = task;
}

void setup() {
    Serial.begin(MPPT_BAUD);
    mpptSerial.begin(MPPT_BAUD);
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
        logger.debug(F("BME Update start"));
        last_bme_update = millis();
        updateBME280Data();
    }

    if((millis()-MPPT_UPDATE_INTERVAL > last_mppt_update || millis() < last_mppt_update) && currentTask != TASK_UPDATE_MPPT && millis() > MPPT_UPDATE_DELAY) {
        logger.debug(F("MPPT Update start"));
        last_mppt_update = millis();
        switchTask(TASK_UPDATE_MPPT);
        lines_to_parse = 18;
    }

    char c = mpptSerial.read();
    if(currentTask == TASK_UPDATE_MPPT) {
        if(last_task_switch+3000 < millis()) {
            logger.debug(F("MPPT Update timeout"));
            switchTask(TASK_PROCESS_ETHERNET);
        }
        if(c != -1 && veParser.readChar(c)) {
            lines_to_parse--;
            VeDirectMetric metric;
            VeDirectPrometheusMapper mapper;
            const __FlashStringHelper* fs;

            // Parse line
            veParser.parseLine(&metric);
            bool found = mapper.mapMetric(metric.name, &fs);
            if(found) {
                prometheus.setMetric(PrometheusMetric(fs, (float)atof(metric.value_c)));
            }
            if(lines_to_parse == 0) {
                logger.debug(F("MPPT Update finished"));
                switchTask(TASK_PROCESS_ETHERNET);
            }
        }
    }

    if(currentTask == TASK_PROCESS_ETHERNET) {
        ether.receivePacket();
        if (http.isGet(F("/metrics"))) {
            http.printHeaders(PROMETHEUS_EXPORTER_HEADER);
            prometheus.printMetrics(&http);
            http.sendReply();
        } else if (http.havePacket()) {
            http.notFound();
        }
    }
}
