#include <Arduino.h>

#define LOGGER_BLINK_DELAY 250

/**
 * Logger class for serial and led output
 */
class Logger {
    public:
        /**
         * Constructor for a Logger
         * @param serial indicates status of serial output
         * @param led indicates status of serial output
         */
        Logger(bool serial, bool led);
        
        /**
         * Returns status of serial output
         * @return status of serial output
         */ 
        bool serialActive();

        /**
         * Sets status of serial output
         * @param active status of serial output
         */ 
        void setSerialActive(bool active);

        /**
         * Returns status of led output
         * @return status of led output
         */
        bool ledActive();

        /**
         * Sets status of led output
         * @param active status of led output
         */ 
        void setLedActive(bool active);

        /**
         * Writes message of log level Info to serial output
         * @param fs message to write
         */ 
        void info(const __FlashStringHelper* fs);

        /**
         * Writes message of log level Info to serial output
         * @param s message to write
         */ 
        void info(char* s);

        /**
         * Writes message of log level Debug to serial output
         * @param fs message to write
         */
        void debug(const __FlashStringHelper* fs);

        /**
         * Writes message of log level Debug to serial output
         * @param s message to write
         */
        void debug(char* s);
        
        /**
         * Writes message of log level Warn to serial output
         * @param fs message to write
         */
        void warn(const __FlashStringHelper* fs);

        /**
         * Writes message of log level Warn to serial output
         * @param s message to write
         */
        void warn(char* s);
        
        /**
         * Writes message of log level Error to serial output
         * @param fs message to write
         */
        void error(const __FlashStringHelper* fs);

        /**
         * Writes message of log level Error to serial output
         * @param s message to write
         */
        void error(char* s);
        
        /**
         * Lets LED Blink in the supplied pattern. 
         * @param pattern pattern to blink by
         */
        void blink(uint8_t pattern);

    private:
        bool serialOutputActive;    ///< Serial Port status
        bool ledOutputActive;       ///< LED status

        /**
         * Write message to serial output
         * @param level message log level
         * @param msg message content
         */
        void log(const __FlashStringHelper* level, const __FlashStringHelper* msg);

        /**
         * Write message to serial output
         * @param level message log level
         * @param msg message content
         */
        void log(const __FlashStringHelper* level, char* msg);

};