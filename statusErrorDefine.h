/**
 * \file statusErrorDefine.h
 * \brief header file containing all of the program enumerations
 * \author Mael Parot
 * \version 1.2
 * \date 11/04/2024
 *
 * Contains all of the program enumerations namely, the program
 * status and error codes, the program modes and the state machine states.
 */

#ifndef STATUSERRORDEFINE_H
#define STATUSERRORDEFINE_H

/**
 * \enum statusErrDef
 * \brief the program status and error codes
 * 
 */
typedef enum
{
	noError						= 0x0000, /**< No error has been detected, the function is working. */

	/*=====INFORMATION AND SUCCESS=====*/
	// CSV (from 0x0100 to 0x01FF)
	infoInitCSV					= 0x0101, /**< The CSV module has successfully initialized. */
	infoCSVChanged				= 0x0102, /**< The general state CSV has been changed. */
	infoShutdownCSV				= 0x01FF, /**< The CSV module has successfully shutdown. */

	// OPL (from 0x0200 to 0x02FF)
	infoInitOPL					= 0x0201, /**< The OpenPOWERLINK module has successfully initialized. */
	infoEGNotChanged			= 0x0202, /**< The general state from the MN has not changed. */
	infoStopOrderReceived		= 0x0203, /**< A stop order from the MN has been received. */
	infoNoStopOrder				= 0x0204, /**< No stop order from the MN has been received. */
	infoModeSetToManual			= 0x0205, /**< The mode to manual order has been received from the MN. */
	infoEGsetToZero				= 0x0206, /**< The default EG value when the MN is not connected. */
	infoShutdownOPL				= 0x02FF, /**< The OpenPOWERLINK module has successfully shutdown. */

	// Valve (from 0x0300 to 0x03FF)
	infoInitValve				= 0x0301, /**< The valve module has successfully initialized. */
	infoVerifDependSucess		= 0x0302, /**< The verification of valve dependances has succeeded. */
	infoNoDepend				= 0x0303, /**< There is no dependances for this valve. */
	infoValveAlreadyActivated	= 0x0304, /**< The valve is already activated. */
	infoAllDependNotActivated	= 0x0305, /**< All the valve dependances are not yet activated. */
	infoShutdownValve			= 0x03FF, /**< The valve module has successfully shutdown. */

	// Sensor (from 0x0400 to 0x04FF)
	infoInitSensor				= 0x0401, /**< The sensor module has successfully initialized. */
	infoReadChannels			= 0x0402, /**< Reading sensor channels has succeeded. */
	infoShutdownSensor			= 0x04FF, /**< The sensor module has successfully shutdown. */
	
	// EG codes (from 0x1000 to 0x6FFF)

	// Main state (from 0x7000 to 0x7FFF)
	infoStateToInit				= 0x7001, /**< The main state has been changed to initialisation. */
	infoStateToControl			= 0x7002, /**< The main state has been changed to acquisition and control. */
	infoStateToManualMode		= 0x7777, /**< The general state has been changed to manual mode. */
	infoStateToShutdown			= 0x7FFF, /**< The main state has been changed to shutdown state. */

	/*=============ERROR===============*/
	// Sensor (from 0xE000 to 0xE0FF)
	errOpenTelemFile			= 0xE001, /**< Opening the telemetry file has failed. */
	errTestWriteFile			= 0xE002, /**< Writing in the telemetry file has failed. */
	errCloseTelemFile			= 0xE0FF, /**< Closing the telemetry file has failed. */

	// CSV (from 0xE100 to 0xE1FF)
	errAllocDataEG				= 0xE101, /**< Memory allocation failure for the structure LigneEG. */
	errAllocDataEtats			= 0xE102, /**< Memory allocation failure for the structure LigneCSV. */
	errAllocDataPhysValves		= 0xE103, /**< Memory allocation failure for the structure LigneVannes. */
	errAllocDataPhysSensors		= 0xE104, /**< Memory allocation failure for the structure LigneSensors. */
	errAllocDataActivation		= 0xE105, /**< Memory allocation failure for the structure LigneActivation. */
	errOpenEGFile				= 0xE106, /**< The "liaisonEGEtat.csv" CSV file has failed to open. */
	errEGNotFoundInFile			= 0xE107, /**< The general state was not found in "liaisonEGEtat.csv". */
	errOpenEtatsFile			= 0xE108, /**< A general state CSV file has failed to open. */
	errOpenPhysValvesFile		= 0xE109, /**< The "physicalCONFIG_valves.csv" CSV file has failed to open. */
	errOpenPhysSensorsFile		= 0xE10A, /**< The "physicalCONFIG_sensors.csv" CSV file has failed to open. */
	errOpenActivationFile		= 0xE10B, /**< The "activation.csv" CSV file has failed to open. */

	// OPL (from 0xE200 to 0xE2FF)
	errOPLSystemInit			= 0xE201, /**< OpenPOWERLINK fails to set the correct configuration for the current operating system. */
	errSelNetInterface			= 0xE202, /**< The selection of the network interface fails when in a Windows machine. Can be caused by the absence of WinPcap. */
	errInitObjDictionary		= 0xE203, /**< The object dictionary header file (objdict.h) has incorrect values or syntax. */
	errOplkInit					= 0xE204, /**< The OpenPOWERLINK stack fails to initialize. Main cause: the stack is not found by the application, check the CN .lib files. */
	errOplkCreate				= 0xE205, /**< The OpenPOWERLINK stack fails to create a new instance. */
	errOplkAllocProcessImage	= 0xE206, /**< The allocation of the input and/or output structure fails because parts of the structure doesn't exist in the objdict.h file. */
	errLinkPDOout				= 0xE207, /**< The linking of an output object (TPDO) fails because the object doesn't exist in the objdict.h file. */
	errLinkPDOEGin				= 0xE208, /**< The linking of the input general state fails because the object doesn't exist in the objdict.h file. */
	errLinkPDOin				= 0xE209, /**< The linking of an input object (RPDO) fails because the object doesn't exist in the objdict.h file.*/
	errSendNMTResetCommand		= 0xE20A, /**< The OpenPOWERLINK reset command fails. */
	errOplkFreeProcessImage		= 0xE20B, /**< The freeing of memory of the OpenPOWERLINK stack fails. */
	errSystemSendTerminate		= 0xE20C, /**< A termination signal from the operating system has been received. */
	errOplKernelStackDown		= 0xE20D, /**< The OpenPOWERLINK kernel is down. */

	// Valve (from 0xE300 to 0xE3FF)
	errGPIOPathEmpty			= 0xE301, /**< The gpio chip path is not correctly set. */
	errOpenGPIO					= 0xE302, /**< The gpio chip fails to open. */
	errGPIOGetLine				= 0xE303, /**< The gpio lines are busy, line get fails. */
	errGPIORequestOutput		= 0xE304, /**< The gpio lines can't be set to output mode. */
	errGetValveInitValue		= 0xE305, /**< The getter of the initial values fails. */
	errValueInitIsNotBinary		= 0xE306, /**< An initial valve value is not 0 or 1. */
	errGPIOSetInitValue			= 0xE307, /**< An initial valve value is not set to a line. */
	errGPIOGetValue				= 0xE308, /**< The getter of a gpio line value fails. */
	errGPIODependValue			= 0xE309, /**< The getter of a gpio line dependance value fails. */
	errDependOutsideOfRange		= 0xE30A, /**< A dependance is not among the valves of the same board. */
	errValueIsNotBinary			= 0xE30B, /**< A valve value is not 0 or 1. */
	errGPIOSetValue				= 0xE30C, /**< The valve value has not been set to the gpio line. */
	errGPIORelease				= 0xE3FF, /**< A gpio line fails to be released. */

	// Sensor (from 0xE400 to 0xE4FF)
	errOpenAdc					= 0xE401, /**< A sysfs file of the MCP3008 fails to open. */
	errReadAdc					= 0xE402, /**< A sysfs file read of the MCP3008 fails. */
	errCloseAdc					= 0xE4FF, /**< A sysfs file of the MCP3008 fails to close. */


} statusErrDef;

/**
 * \enum Mode
 * \brief the modes of the program
 */
typedef enum
{
	automatic					= 0x00, /**< The valve values are taken from a general state CSV file. */
	manual 						= 0x01, /**< The valve values are taken from from the OpenPOWERLINK input values from the MN. */
	
} Mode;
extern Mode mode;

/**
 * \enum stateDef
 * \brief list of states for the main state machine
 */
typedef enum
{
    init, 								/**< Initialize all modules with status and error telemetry. */
	controlAndAcquisition, 				/**< Activate valves and read sensors values. */
    shutdown, 							/**< Shutdown all modules with status and error telemetry. */
    ending, 							/**< Stop the program. */
} stateDef;

#endif
