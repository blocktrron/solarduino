#include <Arduino.h>

class VeDirectPrometheusMapping {
public:
    VeDirectPrometheusMapping(const __FlashStringHelper* input_mppt, const __FlashStringHelper* input_prometheus) {
        mppt = input_mppt;
        pm = input_prometheus;
    }
    const __FlashStringHelper* mppt;
    const __FlashStringHelper* pm;
};

class VeDirectPrometheusMapper {
public:
    bool mapMetric(char *input, const __FlashStringHelper** output) {
        VeDirectPrometheusMapping mapping[12] = {
            VeDirectPrometheusMapping(F("V"), F("mppt_main_battery_voltage")),
            VeDirectPrometheusMapping(F("VPV"), F("mppt_panel_voltage")),
            VeDirectPrometheusMapping(F("PPV"), F("mppt_panel_power")),
            VeDirectPrometheusMapping(F("I"), F("mppt_battery_current")),
            VeDirectPrometheusMapping(F("IL"), F("mppt_load_current")),
            VeDirectPrometheusMapping(F("ERR"), F("mppt_error_code")),
            VeDirectPrometheusMapping(F("H19"), F("mppt_vield_total_ur")),
            VeDirectPrometheusMapping(F("H20"), F("mppt_yield_total")),
            VeDirectPrometheusMapping(F("H21"), F("mppt_max_power_today")),
            VeDirectPrometheusMapping(F("H22"), F("mppt_yield_yesterday")),
            VeDirectPrometheusMapping(F("H23"), F("mppt_max_power_yesterday")),
            VeDirectPrometheusMapping(F("CS"), F("mppt_operation_state")),
        };
        for(size_t i = 0; i < 12; i++) {
            if(strcmp_PF(input, (uint_farptr_t) mapping[i].mppt) == 0) {
                *output = mapping[i].pm;
                return true;
            }
        }
        return false;
    }
};