#include "VeDirectParser.h"

VeDirectMetric::VeDirectMetric() {
    memset(this, 0, sizeof(VeDirectMetric));
}

VeDirectMetric::VeDirectMetric(char *n, char *v) {
    memset(this, 0, sizeof(VeDirectMetric));
    memcpy(n, name, strlen(n));
    memcpy(v, value_c, strlen(v));
}

VeDirectParser::VeDirectParser() {
    memset(this, 0, sizeof(VeDirectParser));
    memset(linebuffer, 0, 20);

    pending_line_buffer_clear = true;
    linebuffer_size = 19;
    linebuffer_pointer = 0;

    firstline = true;
};

void VeDirectParser::parseLine(VeDirectMetric *result) {
    memset(result, 0, sizeof(VeDirectMetric));
    char *value_start = (char *) &linebuffer;
    for (int i = 0; i < 14; i++) {
        if (linebuffer[i] == '\t') {
            value_start++;
            break;
        }
        result->name[i] = linebuffer[i];
        value_start++;
    }

    memcpy(result->value_c, value_start, strlen(value_start));
}

bool VeDirectParser::readChar(char c) {
    if (linebuffer_pointer == linebuffer_size - 1) {
        firstline = true;
        clearLineBuffer();
    }
    if(pending_line_buffer_clear) {
        pending_line_buffer_clear = false;
        clearLineBuffer();
    }
    if (c == '\r') {
        if (firstline) {
            firstline = false;
            return false;
        }
        pending_line_buffer_clear = true;
        return true;
    } else if (c == '\n') {
        // do nothing
        return false;
    }
    linebuffer[linebuffer_pointer] = c;
    linebuffer_pointer++;
    return false;
}

void VeDirectParser::clearLineBuffer() {
    memset(linebuffer, 0, linebuffer_size);
    linebuffer_pointer = 0;
}