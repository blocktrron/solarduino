#include <stdint.h>
#include <string.h>
#include <ctype.h>

/**
 * Class for storing VeDirect protocol metrics
 */
class VeDirectMetric {
public:
    /**
     * Default constructor for VeDirectMetric
     */
    VeDirectMetric();

    /**
     * Constructor for VeDirectMetric
     * @param n metric name
     * @param v metric value
     */
    VeDirectMetric(char *n, char *v);

    char name[15];      ///< name of metric
    char value_c[15];   ///< value of metric
};

/**
 * Class for a VeDirect protocol parser
 */
class VeDirectParser {
public:
    /**
     * Constructor for a VeDirect protocol parser
     * @param metric_buff buffer to store metrics
     * @param buff_size size of the buffer
     */
    VeDirectParser();

    /**
     * Parses the current line
     * @param target pointer to target memory
     */
    void parseLine(VeDirectMetric *result);

    /**
     * Reads the supplied char to the temporary buffer
     * @param c char to read
     */
    bool readChar(char c);

    /**
     * Erases the temporary line buffer
     */
    void clearLineBuffer();

private:
    bool firstline;
    bool pending_line_buffer_clear;

    char linebuffer[20];
    size_t linebuffer_size;
    size_t linebuffer_pointer;
};