/**
 * \file sensor.cpp
 * \brief Module to read sensor values
 * Contains all functions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */

#include "sensor.h"

/**
 * \brief the for loops index
 */
static int i = 0;
/**
 * \brief sysfs channel file array
 */
static int fd[MAX_SENSORS];
/**
 * \brief sensor values array
 */
int16_t valSensors[MAX_SENSORS];

uint8_t tabSensorActivated[MAX_SENSORS] = {1,1,1,1,1,1,1,1,1,1,1,1};




/**
 * \brief function getter of the value of a sensor
 * from the array of sensors.
 * 
 * \param index the index of the array of sensors
 * \return the value of the sensor in 2 bytes signed.
 */
int16_t getAdc_value(int index) {
    return valSensors[index];
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
statusErrDef readChannels()
{
    statusErrDef res = noError;
    memset(fd, 0, sizeof(fd));
    memset(valSensors, 0, sizeof(valSensors));

    //we open the sysfs files of the MCP3008 channels
    for (i = 0; i < MAX_ADC; i++) {
        if (tabSensorActivated[i]) {
            fd[i] = openAdc(i);
            if (fd[i] == ADC_READ_ERROR)
            {
                res = errOpenAdc;
                break;
            }
        }
    }

    //we read the values inside those files
    for (i = 0; i < MAX_ADC; i++) {
        if (!tabSensorActivated[i])
            continue;

        valSensors[i] = readAdc(fd[i]);

        // reset for next read
        lseek(fd[i], 0, SEEK_SET);

        if (valSensors[i] == ADC_READ_ERROR)
        {
            res = errReadAdc;
            break;
        }
    }

    //we close them for the next read
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
statusErrDef closeAdc()
{
    statusErrDef res = noError;
    int ret = 0;

    for (i = 0; i < MAX_ADC; i++) {
        if (fd[i] > 0)
        {
            ret = close(fd[i]);
            if (ret < 0)
            {
                res = errCloseAdc;
                break;
            }
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
int readAdc(int fd)
{
    char buff[8];
    buff[7] = 0;

    int val = ADC_READ_ERROR;

    memset(buff, 0, sizeof(buff));

    //read a specific length and convert it to an integer
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
int openAdc(int adc)
{
    char path[128];
    
    //a specific channel sysfs file path
    sprintf(path, "%sin_voltage%d_raw", IIOSYSPATH, adc);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open()");
        printf("%s\n", path);
        return ADC_READ_ERROR;
    }

    return fd;
}
