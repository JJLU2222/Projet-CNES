/**
 * \file sensor.cpp
 * \brief Module to read sensor values
 * Contains all functions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */

#include "sensor.h"

/**
 * \brief sysfs channel file array
 */
static int fd;
/**
 * \brief sensor values array
 */
int16_t valSensor;

Sensor::Sensor(const std::string &name, const std::string &board, int type, int channel)
    : name(name), board(board), type(type), channel(channel), value(0)
{
}

Sensor::~Sensor()
{
}

/**
 * \brief function to initialize the sensor module.
 *
 * \return statusErrDef that values errOpenAdc
 * when a sysfs file of the MCP3008 fails to open
 * or errReadAdc when a sysfs file read of the MCP3008 fails
 * or noError when the function exits successfully.
 */
statusErrDef Sensor::initSensor()
{
    statusErrDef res = noError;
    switch (type)
    { // étape importante pour le modbus
    case 1:
        break;
    case 2: // modbus
        break;
    default:
        break;
    }
    // test if we can read channels correctly
    res = readChannel();
    return res;
}

/**
 * \brief function to shutdown the sensor module.
 *
 * \return statusErrDef that values errCloseAdc
 * when a sysfs file of the MCP3008 fails to close
 * or noError when the function exits successfully.
 */
statusErrDef Sensor::extinctSensor()
{
    statusErrDef res = noError;

    res = closeAdc();

    return res;
}

/**
 * \brief function the read the channels of the
 * MCP3008 by opening, reading and closing the sysfs files.
 *
 * \return statusErrDef that values errOpenAdc
 * when a sysfs file of the MCP3008 fails to open
 * or errReadAdc when a sysfs file read of the MCP3008 fails
 * or noError when the function exits successfully.
 */
statusErrDef Sensor::readChannel()
{
    statusErrDef res = noError;
    fd = 0;
    valSensor = 0;
    // we open the sysfs files of the MCP3008 channels
    fd = openAdc();
    if (fd == ADC_READ_ERROR)
    {
        res = errOpenAdc;
        return res;
    }

    // we read the values inside those files
    valSensor = readAdc(fd);

    // reset for next read
    lseek(fd, 0, SEEK_SET);

    if (valSensor == ADC_READ_ERROR)
    {
        res = errReadAdc;
        return res;
    }
    // up to date the value
    value = valSensor;

    // we close them for the next read
    closeAdc();
    return res;
}

/**
 * \brief function to close the sysfs files
 * of the MCP3008 kernel module.
 *
 * \return statusErrDef that values errCloseAdc
 * when a sysfs file of the MCP3008 fails to close
 * or noError when the function exits successfully.
 */
statusErrDef Sensor::closeAdc()
{
    statusErrDef res = noError;
    int ret = 0;
    if (fd > 0)
    {
        ret = close(fd);
        if (ret < 0)
        {
            res = errCloseAdc;
        }
    }

    return res;
}

/**
 * \brief function to read the value of a sensor
 * from a sysfs file of the MCP3008 kernel module.
 *
 * \param fd the file location
 * \return ADC_READ_ERROR when the file reading fails
 * or the value of a sensor.
 */
int Sensor::readAdc(int fd)
{
    char buff[8];
    buff[7] = 0;

    int val = ADC_READ_ERROR;

    memset(buff, 0, sizeof(buff));

    // read a specific length and convert it to an integer
    if (read(fd, buff, 8) < 0)
        perror("read()");
    else
        val = atoi(buff);

    return val;
}

/**
 * \brief function to open a sysfs file
 * of the MCP3008 kernel module.
 *
 * \param adc the MCP3008 channel number
 * \return ADC_READ_ERROR when the file opening fails
 * or the file location.
 */
int Sensor::openAdc()
{
    char path[128];

    // a specific channel sysfs file path
    sprintf(path, "%sin_voltage%d_raw", IIOSYSPATH, channel);
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open()");
        printf("%s\n", path);
        return ADC_READ_ERROR;
    }

    return fd;
}

void Sensor::print_value() const
{
    std::cout << "La valeur du " << name << " est : " << value << std::endl;
}
