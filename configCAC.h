/**
 * \file configCAC.h
 * \brief header file that contains CAC defines
 * \author Jiajin LU
 * \version 0.0
 * \date 17/02/2025
 *
 * Contains all of the program defines from each module.
 * The defines are set at compile time so they can't be changed
 * at runtime.
 */

#ifndef CONFIG_CAC
#define CONFIG_CAC

// OPL
#include <map>
#include <array>
#include <variant>
#include "sensor.h"
#include "valve.h"

#define CAC_name "CACMO"
#define NCapteur 4
#define NVanne 4

std::array<const char *, 3> CAC_Name = {"CACMO", "CACEHP", "CACOE"};
std::array<const char *, 4> CACMO_Vanne_name = {"VCE", "VCO", "Vanne3", "Vanne4"};
std::array<int, 4> CACMO_Vanne_pin = {5, 6, 19, 26};
std::array<const char *, 4> CACMO_Sensor_name = {"TP-01", "PR-01", "PR-02", "PR-03"};
std::array<int, 4> CACMO_Sensor_channel = {5, 6, 7, 2};

inline std::map<int, std::variant<Sensor, Valve>> dict_CACMO = {
    {10, Valve("VCE", "CACMO", 5)},
    {11, Valve("VCo", "CACMO", 6)},
    {12, Valve("Vanne3", "CACMO", 19)},
    {13, Valve("Vanne4", "CACMO", 26)},
    {15, Sensor("TP-01", "CACMO", 1, 5)},
    {16, Sensor("PR-01", "CACMO", 1, 6)},
    {17, Sensor("PR-02", "CACMO", 1, 7)},
    {18, Sensor("PR-03", "CACMO", 1, 2)}};

#endif
