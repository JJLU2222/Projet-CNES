/**

 * \file sensor.h
 * \brief header file of the sensor module that reads sensor values
 * \author Mael Parot, Corentin Berthon

 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all functions definitions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */

#ifndef SENSOR_H
#define SENSOR_H
//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

#include "configDefine.h"
#include "statusErrorDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <string>
#include <cstdint>
#include <cstring>
#include <iostream>

/*
#if (TARGET_SYSTEM == _WIN32_)
#include <getopt/getopt.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <modbus/modbus.h>
#endif

*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

/**
 * \brief sensor module class.
 *
 */
class Sensor
{
private:
    std::string name; /**< Valve name */
    uint8_t id;
    int16_t value;
    int type;
    int channel;

public:
    Sensor(const std::string &name, uint8_t id, int type, int channel);
    virtual ~Sensor();
    statusErrDef initSensor();
    statusErrDef extinctSensor();
    statusErrDef readChannel();
    statusErrDef closeAdc();
    int readAdc(int fd);
    int openAdc();
    void print_value() const;
};

#endif // SENSOR_H
