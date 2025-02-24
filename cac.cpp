/**

 * \file cac.cpp
 * \brief header file of the cac module
 * \author Jiajin LU

 * \version 1.2
 * \date 24/02/2025
 *
 * Contains all functions definitions related to reading sensors values
 * from the sysfs linux module file for an MCP3008
 */
/*
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
*/
#include "cac.h"

CAC::CAC(const std::string &name, uint8_t id) : name(name), id(id), tab_sensors(nullptr), tab_vannes(nullptr)
{
}

CAC::~CAC()
{
}

statusErrDef CAC::init(std::map<int, std::variant<Sensor, Valve>>)
{
    statusErrDef res = noError;

    int shm_fd_vanne = shm_open(SHM_Vanne, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_vanne, sizeof(VanneData));
    tab_vannes = (VanneData *)mmap(0, sizeof(VanneData), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd_vanne, 0);

    int shm_fd_sensor = shm_open(SHM_Sensor, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd_sensor, sizeof(SensorData));
    tab_sensors = (SensorData *)mmap(0, sizeof(SensorData), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd_sensor, 0);

    // Verification nb capteur ...

    for (const auto &[id_composant, value] : dict_CACMO)
    {
        if (id_composant < id * 10 + 5)
        {
            tab_vannes->vannes[id_composant % 10] = value;
        }
        else
        {
            tab_sensors->sensors[id_composant % 10 - 5] = value;
        }
    }

    return res;
}
