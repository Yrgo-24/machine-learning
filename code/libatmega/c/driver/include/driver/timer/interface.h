/**
 * @brief Timer interface implementation.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/** Timer virtual table implementation. */
typedef struct TimerVtable TimerVtable;

/**
 * @brief Timer interface implementation.
 */
typedef struct TimerInterface 
{
    /** Pointer to the associated virtual table. */
    TimerVtable const *vptr;
} TimerInterface;

/**
 * @brief Enumeration representing supported timer modes.
 */
typedef enum TimerMode 
{
    TimerMode_Periodic, /** Periodic timer, i.e. the timer restarts after it elapses. */
    TimerMode_OneShot,  /** One-shot timer, i.e., the timer stops after it elapses. */
    TimerMode_Count,    /** The number of supported timer modes. */
} TimerMode;

/**
 * @brief Timer virtual table implementation.
 */
typedef struct TimerVtable 
{
    /**
     * @brief Get the ID of the timer.
     * 
     * @param[in] self Reference to the timer.
     * 
     * @return The ID of the timer.
     */
    uint8_t (*id)(const TimerInterface* self);

    /**
     * @brief Get the timer duration.
     * 
     * @param[in] self Reference to the timer.
     * 
     * @return The timer duration in milliseconds.
     */
    uint32_t (*duration_ms)(const TimerInterface* this);
    
    /**
     * @brief Get the timer mode.
     * 
     * @param[in] self Reference to the timer.
     * 
     * @return The timer mode as an enumerator.
     */
    TimerMode (*mode)(const TimerInterface* self);

    /**
     * @brief Check if the timer is enabled.
     * 
     * @param[in] self Reference to the timer.
     * 
     * @return True if the timer is enabled, otherwise false.
     */
    bool (*isEnabled)(const TimerInterface* self);

    /**
     * @brief Start the timer.
     * 
     * @param[in] self Reference to the timer.
     */
    void (*start)(const TimerInterface* self);

    /**
     * @brief Stop the timer.
     * 
     * @param[in] self Reference to the timer.
     * @param[in] reset If true, reset the timer's internal count to zero.
     */
    void (*stop)(const TimerInterface* self, bool reset);

    /**
     * @brief Toggle the timer.
     * 
     * @param[in] self Reference to the timer.
     */
    void (*toggle)(const TimerInterface* self);

    /**
     * @brief Restart the timer. 
     * 
     *        The timer's internal count will be reset to zero before the restart.
     * 
     * @param[in] self Reference to the timer.
     */
    void (*restart)(const TimerInterface* self);

    /**
     * @brief Delete the timer.
     *
     *        Release all resources allocated for the timer. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the timer to delete. The pointer will be set to nullptr.
     */
    void (*delete)(TimerInterface** self);
} TimerVtable;
