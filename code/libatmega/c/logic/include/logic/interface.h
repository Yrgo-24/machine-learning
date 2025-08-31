/**
 * @brief Logic interface for the main system.
 */
#pragma once

/** Factory interface implementation. */
typedef struct FactoryInterface FactoryInterface;

/** Logic virtual table implementation. */
typedef struct LogicVtable LogicVtable;

/**
 * @brief Logic interface for the main system.
 */
typedef struct LogicInterface
{
    /** Pointer to the associated virtual table. */
    LogicVtable const *vptr;
} LogicInterface;

/**
 * @brief Enumeration representing supported logic revisions.
 */
typedef enum LogicRevision
{
    LogicRevision_Rev1,  /** Logic revision 1. */
    LogicRevision_Count, /** The number of supported logic revisions. */
} LogicRevision;

/**
 * @brief Logic virtual table implementation.
 */
typedef struct LogicVtable 
{
    /**
     * @brief Get the logic revision.
     * 
     * @param[in] self Reference to the logic implementation.
     * 
     * @return The logic revision as an enumerator.
     */
    LogicRevision (*revision)(const LogicInterface* self);

    /**
     * @brief Run the system continuously.
     * 
     * @param[in] self Reference to the logic implementation.
     */
    void (*run)(LogicInterface* self);

    /**
     * @brief Delete the logic implementation used for the system.
     *
     *        Release all resources allocated for the system. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the logic to delete. The pointer will be set to nullptr.
     */
    void (*delete)(LogicInterface** self);
} LogicVtable;

/**
 * @brief Create logic implementation for the main system.
 * 
 * @param[in] revision Logic revision to use.
 * @param[in] factory Factory to use for creation.
 * 
 * @return Pointer to the new logic implementation, or nullptr if the logic couldn't be created.
 */
LogicInterface* logic_new(LogicRevision revision, FactoryInterface* factory);
