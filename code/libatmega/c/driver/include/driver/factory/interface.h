/**
 * @brief Factory interface implementation.
 */
#pragma once

#include <stdint.h>

#include "driver/adc/interface.h"
#include "driver/config/interface.h"
#include "driver/eeprom/interface.h"
#include "driver/gpio/interface.h"
#include "driver/serial/interface.h"
#include "driver/timer/interface.h"
#include "driver/watchdog/interface.h"

/** Factory virtual table implementation. */
typedef struct FactoryVtable FactoryVtable;

/**
 * @brief Factory interface implementation.
 */
typedef struct FactoryInterface 
{
    /** Pointer to the associated virtual table. */
    FactoryVtable const* vptr;
} FactoryInterface;

/**
 * @brief Factory virtual table implementation.
 */
typedef struct FactoryVtable 
{
    /**
     * @brief Create an A/D converter.
     * 
     * @return Pointer to the new A/D converter, or nullptr if creation failed.
     */
    AdcInterface* (*adc_new)(void);

    /**
     * @brief Create a configuration.
     * 
     * @return Pointer to the new configuration, or nullptr if creation failed.
     */
    ConfigInterface* (*config_new)(void);

    /**
     * @brief Create an EEPROM.
     * 
     * @return Pointer to the new EEPROM, or nullptr if creation failed.
     */
    EepromInterface* (*eeprom_new)(void);

    /**
     * @brief Create a GPIO.
     * 
     * @param[in] pin PIN number on the device.
     * @param[in] mode GPIO mode.
     * @param[in] callback Pointer to associated callback function (nullptr = none).
     * 
     * @return Pointer to the new GPIO, or nullptr if creation failed.
     */
    GpioInterface* (*gpio_new)(uint8_t pin, GpioMode mode, void (*callback)(void));

    /**
     * @brief Create a serial device.
     * 
     * @return Pointer to the new serial device, or nullptr if creation failed.
     */
    SerialInterface* (*serial_new)(void);

    /**
     * @brief Create a timer.
     * 
     * @param[in] duration_ms Timer duration in milliseconds.
     * @param[in] mode Timer mode.
     * @param[in] callback Pointer to associated callback function.
     * 
     * @return Pointer to the new timer, or nullptr if creation failed.
     */
    TimerInterface* (*timer_new)(uint32_t duration_ms, TimerMode mode, void (*callback)(void));

    /**
     * @brief Create a watchdog timer.
     * 
     * @return Pointer to the new watchdog timer, or nullptr if creation failed.
     */
    WatchdogInterface* (*watchdog_new)(void);

    /**
     * @brief Delete the factory.
     *
     *        Release all resources allocated for the factory. 
     *        
     *        Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the factory to delete. The pointer will be set to nullptr.
     */
    void (*delete)(FactoryInterface** self);
} FactoryVtable;

/**
 * @brief Enumeration representing supported factory types.
 */
typedef enum FactoryType 
{
    FactoryType_Atmega328p, /** ATmega328P factory. */
    FactoryType_Count,      /** The number of supported factory types. */
} FactoryType;

/**
 * @brief Create a factory.
 * 
 * @param[in] type The type of factory to create.
 * 
 * @return A pointer to the created factory, or a nullptr if an unsupported factory was specified.
 */
FactoryInterface* factory_create(FactoryType type);
