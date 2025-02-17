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
#include <array>

std::array<const char *, 3> CAC_Name = {"CACMO", "CACEHP", "CACOE"};
std::array<const char *, 4> CACMO_Vanne_name = {"VCE", "VCO", "Vanne3", "Vanne4"};
std::array<int, 4> CACMO_Vanne_pin = {5, 6, 19, 26};

#endif
