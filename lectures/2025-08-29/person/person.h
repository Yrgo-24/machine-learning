/**
 * @brief Personal data implementation.
 */
#pragma once

#include <iostream>

/**
 * @brief Enumeration of genders.
 */
enum class Gender 
{ 
    Male,   /** Male. */
    Female, /** Female. */
    Other,  /** Other gender. */
};

/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    explicit Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true) noexcept;

    /**
     * @brief Delete person instance.
     */
    ~Person() noexcept = default;

	/**
     * @brief Get the person's name.
     * 
     * @return The person's name as a string.
     */
    const char* name() const noexcept;

    /**
     * @brief Get the person's age.
     * 
     * @return The person's age as an integer.
     */
    unsigned age() const noexcept;
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const noexcept;

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const noexcept;

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const noexcept;

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const noexcept;
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) noexcept;

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) noexcept;

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) noexcept;
	
	/**
	 * @brief Print personal data about the person.
	 * 
	 * @param[in] ostream The output stream to use (default = terminal print).
	 */
	void print(std::ostream& ostream = std::cout) const;
	
	Person()                         = delete; // No default constructor.
    Person(const Person&)            = delete; // No copy constructor.
    Person(Person&&)                 = delete; // No move constructor.
    Person& operator=(const Person&) = delete; // No copy assignment.
    Person& operator=(Person&&)      = delete; // No move assignment.

private:
	/**
	 * @brief Get the person's gender as a string.
	 * 
	 * @return The person's gender as a string.
	 */
	const char* genderStr() const noexcept;

	/**
	 * @brief Get string indicating whether the person is single.
	 * 
	 * @return "Yes" if the person is single, else "No".
	 */
	const char* isSingleStr() const noexcept;
	
    /** The person's name. */
    const char* myName;

    /** The person's age. */
    unsigned myAge;

    /** The person's gender. */
    Gender myGender;

    /** The person's address. */
    const char* myAddress;

    /** The person's occupation. */
    const char* myOccupation;

    /** Indicate whether the person is single. */
    bool myIsSingle;
};