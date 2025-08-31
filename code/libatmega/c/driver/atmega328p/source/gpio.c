/**
 * @brief Gpio implementation details for ATmega328P.
 */
#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "driver/atmega328p/gpio.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** The number of I/O ports on the ATmega328P. */
#define IO_PORT_COUNT 3U

/** The number of GPIO pins on the ATmega328P. */
#define PIN_COUNT 20U

/** Callback index for I/O port B. */
#define CALLBACK_INDEX_PORTB 0U

/** Callback index for I/O port C. */
#define CALLBACK_INDEX_PORTC 1U

/** Callback index for I/O port D. */
#define CALLBACK_INDEX_PORTD 2U

/** Pin offset for I/O port B. */
#define PIN_OFFSET_PORTB 8U

/** Pin offset for I/O port C. */
#define PIN_OFFSET_PORTC 14U

/** Pin offset for I/O port D. */
#define PIN_OFFSET_PORTD 0U

/*******************************************************************************
 * Forward declarations.
 ******************************************************************************/
 /** Hardware structure. */
typedef struct GpioHardware GpioHardware;

/*******************************************************************************
 * Local type definitions.
 ******************************************************************************/
/** Callback type. */
typedef void (*Callback)(void);

/**
 * @brief Enumeration representing supported I/O ports.
 */
typedef enum GpioPort 
{
    GpioPort_B, /** Gpio port B. */
    GpioPort_C, /** Gpio port C. */
    GpioPort_D, /** Gpio port D. */
} GpioPort;

/**
 * @brief Gpio implementation structure for ATmega328P.
 */
typedef struct Gpio 
{
    /** GPIO interface implementation. */
    GpioInterface itf;

    /** Hardware structure for read/write access. */
    GpioHardware* hw;

    /** Physical pin on the associated I/O port. */
    uint8_t pin;
} Gpio;

/**
 * @brief Gpio hardware structure for read/write access.
 */
typedef struct GpioHardware 
{
    /** Data direction register. */
    volatile uint8_t* const dirReg;

    /** Port (output) register. */
    volatile uint8_t* const portReg;

    /** Pin (input) register, also used for toggling. */
    volatile uint8_t* const pinReg;

    /** Pin change interrupt mask register. */
    volatile uint8_t* const pcMskReg;

    /** I/O port associated with the device. */
    const GpioPort port;

    /** Index for callback associated with the I/O port. */
    const uint8_t cbIndex;
} GpioHardware;

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static void gpio_enableInterrupt(const GpioInterface* self, bool enable);
static const GpioVtable* const gpioVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Hardware structure for access to PORT B. */
static GpioHardware myHwPortB = 
{
    .dirReg   = &DDRB,
    .portReg  = &PORTB,
    .pinReg   = &PINB,
    .pcMskReg = &PCMSK0,
    .port     = GpioPort_B,
    .cbIndex  = CALLBACK_INDEX_PORTB,
};

/** Hardware structure for access to PORT C. */
static GpioHardware myHwPortC =
 {
    .dirReg   = &DDRC,
    .portReg  = &PORTC,
    .pinReg   = &PINC,
    .pcMskReg = &PCMSK1,
    .cbIndex  = CALLBACK_INDEX_PORTC,
};

/** Hardware structure for access to PORT D. */
static GpioHardware myHwPortD = 
{
    .dirReg   = &DDRD,
    .portReg  = &PORTD,
    .pinReg   = &PIND,
    .pcMskReg = &PCMSK2,
    .port     = GpioPort_D,
    .cbIndex  = CALLBACK_INDEX_PORTD,
};

/** Pin registry (1 = reserved, 0 = free). */
static uint32_t myPinRegistry = 0UL;

/** Array holding callback pointers. */
static Callback myCallbacks[IO_PORT_COUNT] = {NULL, NULL, NULL};

/*******************************************************************************
 * PIN-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool isPinConnectedToPortB(const uint8_t pin) 
{
    // Return true if the given pin is within the range of I/O port B.
    return (PIN_OFFSET_PORTB <= pin) && (PIN_OFFSET_PORTC > pin);
}

// -----------------------------------------------------------------------------
static inline bool isPinConnectedToPortC(const uint8_t pin)
{
    // Return true if the given pin is within the range of I/O port C.
    return (PIN_OFFSET_PORTC <= pin) && (PIN_COUNT > pin);
}

// -----------------------------------------------------------------------------
static inline bool isPinConnectedToPortD(const uint8_t pin) 
{
    // Return true if the given pin is within the range of I/O port D.
    return (PIN_OFFSET_PORTB > pin); 
}

// -----------------------------------------------------------------------------
static uint8_t physicalPin(const uint8_t pin)
{
    // Convert GPIO pin to physical pin, or return 0xFF if the pin number is invalid.
    if (isPinConnectedToPortB(pin))      { return pin - PIN_OFFSET_PORTB; }
    else if (isPinConnectedToPortC(pin)) { return pin - PIN_OFFSET_PORTC; }
    else if (isPinConnectedToPortD(pin)) { return pin; }
    else                                 { return (uint8_t)(-1); }
}

// -----------------------------------------------------------------------------
static inline uint8_t physicalBitMask(const uint8_t pin) 
{ 
    // Convert from GPIO pin to physical before creating the bit mask.
    return (uint8_t)(1UL << physicalPin(pin)); 
}

/*******************************************************************************
 * Pin registry functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool pinRegistry_isValid(const uint8_t pin) 
{ 
    // Return true if the given pin is a valid number.
    return PIN_COUNT > pin; 
}

 // -----------------------------------------------------------------------------
static inline bool pinRegistry_isFree(const uint8_t pin)
{
    // Return true if the pin is valid and the corresponding bit in the registry is clear.
    return pinRegistry_isValid(pin) ? 0U == (myPinRegistry & (1UL << pin)) : false;
}

// -----------------------------------------------------------------------------
static inline void pinRegistry_reserve(const uint8_t pin) 
{ 
    // Set the associated bit in the registry if the pin is valid.
    if (pinRegistry_isValid(pin)) { myPinRegistry |= (1UL << pin); }
}

// -----------------------------------------------------------------------------
static inline void pinRegistry_free(const uint8_t pin) 
{ 
    // Clear the associated bit in the registry if the pin is valid.
    if (pinRegistry_isValid(pin)) { myPinRegistry &= ~(1UL << pin);  }
}

/*******************************************************************************
 * GPIO mode-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool isModeValid(const GpioMode mode)
{ 
    // Return true if the specified mode is any of the supported types.
    return GpioMode_Count > mode;
}

/*******************************************************************************
 * Hardware-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static GpioHardware* gpioHardware_get(const uint8_t pin)
{
    // Return the hardware structure associated with the given pin, or nullptr if invalid.
    if (isPinConnectedToPortB(pin))      { return &myHwPortB; }
    else if (isPinConnectedToPortC(pin)) { return &myHwPortC; }
    else if (isPinConnectedToPortD(pin)) { return &myHwPortD; }
    return NULL;
}

// -----------------------------------------------------------------------------
static void gpioHardware_setMode(GpioHardware* const hw, const uint8_t pin, const GpioMode mode)
{
    // Set the associated bit in the port register to enable the internal pull-up resistor.
    if (GpioMode_InputPullup == mode) { *(hw->portReg) |= physicalBitMask(pin); } 

    // Set the associated bit in the data direction register to set the GPIO pin to output.
    else if (GpioMode_Output == mode) { *(hw->dirReg) |= physicalBitMask(pin);  }
}

// -----------------------------------------------------------------------------
static GpioHardware* gpioHardware_init(const uint8_t pin, const GpioMode mode)
{
    // Get the hardware structure associated with the given pin.
    GpioHardware* const hardware = gpioHardware_get(pin);

    // Reserve the given pin.
    pinRegistry_reserve(pin);

    // Set GPIO mode of the given pin.
    gpioHardware_setMode(hardware, pin, mode);

    // Return the hardware structure after the initialization is finished.
    return hardware;
}

// -----------------------------------------------------------------------------
static void gpioHardware_free(GpioHardware* const self, const uint8_t pin)
{
    // Terminate the function if no valid hardware has been specified.
    if (!self) { return; }

    // Clear the associated bit in the data direction register.
    *(self->dirReg) &= ~physicalBitMask(pin);

    // Clear the associated bit in the port register.
    *(self->portReg) &= ~physicalBitMask(pin);

    // Free the given pin.
    pinRegistry_free(pin);
}

/*******************************************************************************
 * Callback-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool callback_isSet(const uint8_t index)
{ 
    // Return true if the given callback index is valid and a callback exists.
    return IO_PORT_COUNT > index ? myCallbacks[index] : false;
}

// -----------------------------------------------------------------------------
static inline void callback_add(Callback callback, const uint8_t index)
{ 
    // Add the callback is the callback index is valid.
    if (IO_PORT_COUNT > index) { myCallbacks[index] = callback; }
}

// -----------------------------------------------------------------------------
static inline void callback_remove(const uint8_t index)
{ 
    // Remove the callback if the given index is valid.
    if (IO_PORT_COUNT > index) { myCallbacks[index] = NULL; }
}

// -----------------------------------------------------------------------------
static void callback_invoke(const uint8_t index)
{
    // Invoke callback if it exists.
    if (callback_isSet(index)) { myCallbacks[index](); }
}

/*******************************************************************************
 * GPIO-specific functions.
 ******************************************************************************/
 // -----------------------------------------------------------------------------
static inline Gpio* gpio(GpioInterface* const self) 
{ 
    // Downcast to child structure Gpio, return the corresponding pointer.
    return (Gpio*)(self); 
}

// -----------------------------------------------------------------------------
static inline const Gpio* gpioConst(const GpioInterface* const self)
{
    // Downcast to child structure Gpio, return the corresponding pointer.
    return (const Gpio*)(self);
}

// -----------------------------------------------------------------------------
static uint8_t gpio_digitalPin(const Gpio* const self)
{
    // Convert from physical to GPIO pin, or return 0xFF is the pin is invalid.
    if (GpioPort_B == self->hw->port)      { return self->pin - PIN_OFFSET_PORTB; }
    else if (GpioPort_C == self->hw->port) { return self->pin - PIN_OFFSET_PORTC; }
    else if (GpioPort_D == self->hw->port) { return self->pin; }
    return (uint8_t)(-1);
}

// -----------------------------------------------------------------------------
static inline uint8_t gpio_bitMask(const Gpio* const self) 
{ 
    // Return an 8-bit bitmask for modifying the corresponding bit in the GPIO registers.
    return (uint8_t)(1UL << self->pin); 
}

// -----------------------------------------------------------------------------
static bool gpio_checkParameters(const uint8_t pin, const GpioMode mode)
{ 
    // Return true if the PIN is free and the mode is valid.
    return pinRegistry_isFree(pin) && isModeValid(mode);
}

// -----------------------------------------------------------------------------
static void gpio_setCallback(const Gpio* const self, void (*callback)(void)) 
{ 
    // Try to add the callback via the associated callback index.
    callback_add(callback, self->hw->cbIndex);
}

// -----------------------------------------------------------------------------
static void gpio_disable(Gpio* const self)
{
    // Free hardware resources.
    gpioHardware_free(self->hw, gpio_digitalPin(self));
    self->hw = NULL;

    // Disable pin change interrupts for the PIN.
    gpio_enableInterrupt((const GpioInterface*)(self), false);
}

// -----------------------------------------------------------------------------
static Gpio* gpio_new(const uint8_t pin, const GpioMode mode)
{
    // Allocate memory for a new GPIO structure.
    Gpio* const self = (Gpio*)(malloc(sizeof(Gpio)));

    // Return a nullptr if memory couldn't be allocated.
    if (!self) { return NULL; }

    // Initialize the vtable pointer to give the interface access to the overridden functions.
    self->itf.vptr = gpioVtable_ptr();

    // Initialize the hardware, point at the associated structure for hardware access.
    self->hw = gpioHardware_init(pin, mode);

    // Store the physical pin number on the associated port.
    self->pin = physicalPin(pin);

    // Return the newly allocated GPIO structure.
    return self;
}

// -----------------------------------------------------------------------------
static void gpio_delete(Gpio* const self)
{
    // Disable and free the hardware allocated for the device.
    gpio_disable(self);

    // Free the resources allocated for the device itself.
    free(self); 
}

// -----------------------------------------------------------------------------
static uint8_t gpio_pin(const GpioInterface* const self)
{
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Return the pin the GPIO is connected to.
    return gpio_digitalPin(impl);
}

// -----------------------------------------------------------------------------
static GpioMode gpio_mode(const GpioInterface* const self)
{
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Report as output if the associated bit in the data direction register is set.
    if (*(impl->hw->dirReg) & gpio_bitMask(impl)) { return GpioMode_Output; }

    // Report as input with the internal pull-up register activated if the associated bit in 
    // the port register is set.
    else if (*(impl->hw->portReg) & gpio_bitMask(impl)) { return GpioMode_InputPullup; }

    // Report as input if neither of the aforementioned bits is set.
    else { return GpioMode_Input; }
}

// -----------------------------------------------------------------------------
static bool gpio_read(const GpioInterface* const self)
{ 
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Return true if the associated bit in the pin register is set.
    return *(impl->hw->pinReg) & gpio_bitMask(impl);
}

// -----------------------------------------------------------------------------
static void gpio_write(const GpioInterface* const self, const bool value)
{
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Set the associated bit in the port register to set the output high.
    if (value) { *(impl->hw->portReg) |= gpio_bitMask(impl); }

    // Clear the associated bit in the port register to set the output low.
    else { *(impl->hw->portReg) &= ~gpio_bitMask(impl); }
}

// -----------------------------------------------------------------------------
static void gpio_toggle(const GpioInterface* const self) 
{ 
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Set the associated bit in the pin register to toggle the output.
    // Note that the pin register is read-only, but any attempts to set this pin will
    // cause the associated bit in the port register to toggle.
    *(impl->hw->pinReg) |= gpio_bitMask(impl); 
}

// -----------------------------------------------------------------------------
static void gpio_enableInterruptsOnPort(const GpioInterface* const self, const bool enable) 
{ 
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Enable interrupts on port if specified.
    if (enable) 
    { 
        // Set the associated bit in pin change interrupt control register to enable interrupts.
        PCICR |= (1UL << impl->hw->port); 

        // Also enable interrupts globally.
        asm("SEI");
    }
    // Clear the associated bit in pin change interrupt control register to disable interrupts.
    else { PCICR &= ~(1UL << impl->hw->port); }
}

// -----------------------------------------------------------------------------
static void gpio_enableInterrupt(const GpioInterface* const self, const bool enable)
{
    // Downcast to child structure Gpio for accessing the hardware.
    const Gpio* const impl = gpioConst(self);

    // Enable pin change interrupts both globally and locally if specified.
    if (enable)
    {
        // Enable pin change interrupts for the associated I/O port.
        PCICR |= (1UL << impl->hw->port);

        // Enable pin change interrupts for the associated pin.
        *(impl->hw->pcMskReg) |= gpio_bitMask(impl);

        // Enable interrupts globally.
        asm("SEI");
    }
    // Disable pin change interrupts locally if specified.
    else { *(impl->hw->pcMskReg) &= ~gpio_bitMask(impl); }
}

/*******************************************************************************
 * GPIO vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const GpioVtable* const gpioVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden GPIO functions.
    static const GpioVtable myVtable =
    {
        .pin                    = gpio_pin,
        .mode                   = gpio_mode,
        .read                   = gpio_read,
        .write                  = gpio_write,
        .toggle                 = gpio_toggle,
        .enableInterrupt        = gpio_enableInterrupt,
        .enableInterruptsOnPort = gpio_enableInterruptsOnPort,
        .delete                 = atmega328p_gpio_delete,
    };
    // Return a pointer to the vtable for access through the GPIO interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) GPIO interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
GpioInterface* atmega328p_gpio_new(const uint8_t pin, const GpioMode mode, void (*callback)(void))
{
    // Return a nullptr if the given parameters are invalid or the pin is already reserved.
    if (!gpio_checkParameters(pin, mode)) { return NULL; }

    // Allocate memory for the new device.
    Gpio* const self = gpio_new(pin, mode);

    // Return a nullptr if memory couldn't be allocated.
    if (!self) { return NULL; }

    // Set callback if specified.
    if (callback) { gpio_setCallback(self, callback); }

    // Return a pointer to new GPIO, cast to the corresponding interface.
    return (GpioInterface*)(self);
}

// -----------------------------------------------------------------------------
void atmega328p_gpio_delete(GpioInterface** const self)
{
    // Terminate the function if the GPIO is invalid.
    if (!self || !(*self)) { return; }

    // Delete the GPIO and free allocated resources.
    gpio_delete(gpio(*self));

    // Set the associated pointer to null.
    *self = NULL;
}

/*******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/
// -----------------------------------------------------------------------------
ISR (PCINT0_vect) 
{ 
    // Invoke the registered callback for PORTB, if any.
    callback_invoke(CALLBACK_INDEX_PORTB); 
}

// -----------------------------------------------------------------------------
ISR (PCINT1_vect) 
{ 
    // Invoke the registered callback for PORTC, if any.
    callback_invoke(CALLBACK_INDEX_PORTC); 
}

// -----------------------------------------------------------------------------
ISR (PCINT2_vect) 
{ 
    // Invoke the registered callback for PORTD, if any.
    callback_invoke(CALLBACK_INDEX_PORTD); 
}
