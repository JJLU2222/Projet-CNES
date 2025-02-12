/**
 * \file configDefine.h
 * \brief header file that contains all the program defines
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all of the program defines from each module.
 * The defines are set at compile time so they can't be changed
 * at runtime.
 */

#ifndef CONFIGOPL_H
#define CONFIGOPL_H

// OPL
/**
 * \brief OpenPOWERLINK cycle length of 10ms
 */
#define CYCLE_LEN           10000
/**
 * \brief The CN default ip address (not important)
 */
#define IP_ADDR             0xc0a801A6          // 192.168.1.166
/**
 * \brief The CN default gateway (not important)
 */
#define DEFAULT_GATEWAY     0xC0A801FE          // 192.168.1.C_ADR_RT1_DEF_NODE_ID
/**
 * \brief Network mask of /24
 */
#define SUBNET_MASK         0xFFFFFF00          // 255.255.255.0
/**
 * \brief The CN device name (look in /etc/network/interfaces for the name in Linux)
 */
#define DEVNAME				"eth0"

// xapOpl
/**
 * \brief Total size of RPDOs coming into every CNs
 */
#define SIZE_OUT    (NB_NODES * 25)
/**
 * \brief Total size of TPDOs coming out of every CNs
 */
#define SIZE_IN     (NB_NODES * 13)
/**
 * \brief Total size in bytes for every RPDOs (int16_t is 2 bytes)
 */
#define COMPUTED_PI_OUT_SIZE   ((SIZE_OUT + 1) * 2)
/**
 * \brief Total size in bytes for every TPDOs (int16_t is 2 bytes)
 */
#define COMPUTED_PI_IN_SIZE    ((SIZE_IN + 1) * 2)

// Sensor
/**
 * \brief sysfs MCP3008 files path
 */
#define IIOSYSPATH "/sys/bus/iio/devices/iio:device0/"
/**
 * \brief rs485 to USB serial device location
 */
#define MODBUS_DEVICELOC "/dev/ttyUSB0"
/**
 * \brief number of modbus serial registers to read
 */
#define MODBUS_NBREG 1
/**
 * \brief maximum number of sensors allowed
 */
#define MAX_SENSORS 12
/**
 * \brief maximum number of channels for the MCP3008
 */
#define MAX_ADC 8
/**
 * \brief channel read error code in 2 bytes signed
 */
#define ADC_READ_ERROR -32768

// Valve
/**
 * \brief GPIO device location
 */
#define CHIP_PATH "/dev/gpiochip0"
/**
 * \brief maximum number of valves allowed
 */
#define MAX_VALVES 12

// CSV
/**
 * \brief maximum path length for a general state CSV file
 */
#define MAX_PATH_LENGTH 1024
/**
 * \brief maximum line size for every common CSV file
 */
#define MAX_LINE_SIZE 2832
/**
 * \brief maximum line size for every physical CSV file
 */
#define MAX_PHYSICAL_LINE_SIZE 512
// TelemFiles
/**
 * \brief maximum length of telemetry status or error message
 */
#define EVENTLOG_MAX_LENGTH 1024

// Main
/**
 * \brief delay in milliseconds at the end of the initialisation state
 * to wait for the CN to connect to the MN.
 */
#define DELAYMSINIT 2000
/**
 * \brief delay in milliseconds at the end of the controlAndAcquisition state
 * to avoid dropping the connexion between the CN and the MN.
 */
#ifdef _WIN32
#define DELAYMSWIN 1
#else
#define DELAYUSLINUX 10
#endif

#endif
