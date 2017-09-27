#include <Arduino.h>
#include <Logger.h>

Logger::Logger(bool serial, bool led) {
    serialOutputActive = serial;
    ledOutputActive = led;
}

bool Logger::serialActive() {
    return serialOutputActive;
}

void Logger::setSerialActive(bool active) {
    serialOutputActive = active;
}

bool Logger::ledActive() {
    return ledOutputActive;
}

void Logger::setLedActive(bool active) {
    ledOutputActive = active;
}

void Logger::info(const __FlashStringHelper* fs) {
    log(F("[INFO]"), fs);    
}

void Logger::info(char* s) {
    log(F("[INFO]"), s);    
}

void Logger::debug(const __FlashStringHelper* fs) {
    log(F("[DEBUG]"), fs);    
}

void Logger::debug(char* s) {
    log(F("[DEBUG]"), s);    
}

void Logger::warn(const __FlashStringHelper* fs) {
    log(F("[WARN]"), fs);    
}

void Logger::warn(char* s) {
    log(F("[WARN]"), s);    
}

void Logger::error(const __FlashStringHelper* fs) {
    log(F("[ERROR]"), fs);
}

void Logger::error(char* s) {
    log(F("[ERROR]"), s);
}

void Logger::log(const __FlashStringHelper* level, const __FlashStringHelper* msg) {
    if(serialOutputActive) {
        Serial.print(level);
        Serial.println(msg);
    }
}

void Logger::log(const __FlashStringHelper* level, char* msg) {
    if(serialOutputActive) {
        Serial.print(level);
        Serial.println(msg);
    }
}

void Logger::blink(uint8_t pattern) {
    if(!ledOutputActive) {
        return;
    }
    digitalWrite(LED_BUILTIN, LOW);
    delay(LOGGER_BLINK_DELAY<<1);

    uint8_t mask = 0x80;
    for(uint8_t i = 0; i<8; i++) {
        delay(LOGGER_BLINK_DELAY);
        uint8_t result = pattern & mask;
        if(result > 0) {
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(LED_BUILTIN, LOW);
        }
        mask = mask >> 1;
    }
    delay(LOGGER_BLINK_DELAY);
    digitalWrite(LED_BUILTIN, LOW);    
    delay(2000);
}