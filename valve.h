#ifndef VALVE_H
#define VALVE_H

#include <gpiod.h>
#include <iostream>
#include <cstring>
#include "statusErrorDefine.h"

#define CHIP_PATH "/dev/gpiochip0"

/**
 * \file valve.h
 * \brief Header file of the valve module that controls the valves of the CAC Board
 * \author Alexandre Guidon
 * \version 1.0
 * \date 17/02/2025
 *
 * This class allows for initializing, activating, and deactivating a valve using GPIO control.
 */
class Valve
{
private:
    std::string name;  /**< Valve name */
    std::string board; /**< Name of the board */
    int gpio_pin;      /**< GPIO pin controlling the Valve */
    gpiod_chip *chip;  /**< Pointer to the GPIO chip */
    gpiod_line *line;  /**< Pointer to the GPIO line */

public:
    int state; /**< State of the Valve (1 for active, 0 for inactive) */
    Valve(const std::string &name, const std::string &board, int gpio_pin);
    statusErrDef init();
    void apply_change();
    void activate();
    void desactivate();
    int getstate() const;
    ~Valve();
};

#endif
