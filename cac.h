/**

 * \file cac.h
 * \brief header file of the cac module
 * \author Jiajin LU

 * \version 1.2
 * \date 20/02/2025
 *
 * Contains all functions definitions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */

#ifndef CAC_H
#define CAC_H
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
#include "sensor.h"
#include "valve.h"
#include "configCAC.h"
#include <sys/mman.h> // For shared memory

#define SHM_Sensor "/sensor_shm"
#define SHM_Vanne "/vanne_shm"

struct SensorData
{
    Sensor sensors[NCapteur];
};

struct VanneData
{
    Valve vannes[NVanne];
};

class CAC
{
private:
    uint8_t id;
    std::string name;

public:
    SensorData *tab_sensors;
    VanneData *tab_vannes;
    CAC(const std::string &name, uint8_t id);
    ~CAC();
    statusErrDef init(std::map<int, std::variant<Sensor, Valve>>);
};

#endif
