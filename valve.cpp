// compilation : g++ -c valve.cpp -o valve.o $(pkg-config --cflags --libs libgpiod)

/**
 * \file valve.cpp
 * \brief Module to control the valves of the CAC Board
 * \author Alexandre Guidon
 * \version 1.0
 * \date 17/02/2025
 */

#include "valve.h"

/**
 * \brief Constructor for the Valve class.
 *
 * Initializes a Valve object with a name, board identifier, and GPIO pin number.
 *
 * \param name The name of the valve.
 * \param id_v The id of the sensor.
 * \param gpio_pin The GPIO pin number controlling the valve.
 */

Valve::Valve(const std::string &name, int8_t id_v, int gpio_pin)
    : name(name), id_v(id_v), state(0), gpio_pin(gpio_pin), chip(nullptr), line(nullptr) {}

/**
 * \brief Initializes the GPIO line for the valve control.
 *
 * This function opens the GPIO chip, retrieves the specified line,
 * and requests it as an output. If any step fails, an error message is printed,
 * and the function returns false.
 *
 * \return True if initialization is successful, false otherwise.
 */

statusErrDef Valve::init()
{

    statusErrDef res = noError;

    if (strcmp(CHIP_PATH, "") == 0 || strcmp(CHIP_PATH, " ") == 0)
    {
        perror("Error: GPIO chip path is not set.");
        return errGPIOPathEmpty;
    }

    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        perror("Open chip failed\n");
        return errOpenGPIO;
        ;
    }

    line = gpiod_chip_get_line(chip, gpio_pin);
    if (!line)
    {
        perror("Get line failed\n");
        gpiod_chip_close(chip);
        return errGPIOGetLine;
    }
    int ret = gpiod_line_request_output(line, "Valve_control", 0);
    if (ret < 0)
    {
        perror("Request line as output failed\n");
        gpiod_chip_close(chip);
        return errGPIORequestOutput;
    }
    return res;
}

void Valve::apply_change()
{
    if (line)
    {
        gpiod_line_set_value(line, state);
        std::cout << name << " change state to " << state << std::endl;
    }
}

/**
 * \brief Activates the valve.
 *
 * Sets the GPIO line value to 1, indicating that the valve is enabled.
 * An acknowledgment message is printed to the console.
 */

void Valve::activate()
{
    if (line)
    {
        state = 1;
        apply_change();
        std::cout << name << " enabled." << std::endl; // Il faudra envoyer l'acquitement
    }
}

/**
 * \brief Deactivates the valve.
 *
 * Sets the GPIO line value to 0, indicating that the valve is disabled.
 * An acknowledgment message is printed to the console.
 */

void Valve::desactivate()
{
    if (line)
    {
        state = 0;
        apply_change();
        std::cout << name << " disabled." << std::endl;
    }
}

/**
 * \brief Gets the current state of the valve.
 *
 * \return The state of the valve (1 for active, 0 for inactive).
 */

int Valve::getstate() const
{
    return state;
}

/**
 * \brief Destructor for the Valve class.
 *
 * Releases the GPIO line and closes the GPIO chip to free resources.
 */

Valve::~Valve()
{
    if (line)
    {
        gpiod_line_release(line);
    }
    if (chip)
    {
        gpiod_chip_close(chip);
    }
}
