/**
 * @brief Example program demonstrating a simple class with an associated enum class.
 */
#include <fstream>
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
    Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true)
        : myName{name}
        , myAge{age}
        , myGender{gender}
        , myAddress{address}
        , myOccupation{occupation}
        , myIsSingle{single} 
    {
    }

    /**
     * @brief Delete person instance.
     */
    ~Person() = default;

	/**
     * @brief Get the person's name.
     * 
     * @return The person's name as a string.
     */
    const char* name() const { return myName; }

    /**
     * @brief Get the person's age.
     * 
     * @return The person's age as an integer.
     */
    unsigned age() const { return myAge; }
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) { myIsSingle = single; }
	
	/**
	 * @brief Print personal data about the person.
	 * 
	 * @param[in] ostream The output stream to use (default = terminal print).
	 */
	void print(std::ostream& ostream = std::cout) const
	{
		ostream << "--------------------------------------------------------------------------------\n";
		ostream << "Name:\t\t" << myName << "\n";
		ostream << "Age:\t\t" << myAge << "\n";
		ostream << "Gender:\t\t" << genderStr() << "\n";
		ostream << "Address:\t" << myAddress << "\n";
		ostream << "Occupation:\t" << myOccupation << "\n";
		ostream << "Single:\t\t" << isSingleStr() << "\n"; 
		ostream << "--------------------------------------------------------------------------------\n\n";
	}
	
	Person() = delete; // No default constructor.

private:
	/**
	 * @brief Get the person's gender as a string.
	 * 
	 * @return The person's gender as a string.
	 */
	const char* genderStr() const
	{
		if (myGender == Gender::Male) { return "Male"; }
		else if (myGender == Gender::Female) { return "Female"; }
		else { return "Other"; }
	}

	/**
	 * @brief Get string indicating whether the person is single.
	 * 
	 * @return "Yes" if the person is single, else "No".
	 */
	const char* isSingleStr() const { return myIsSingle ? "Yes" : "No"; }
	
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

/**
 * @brief Print personal data in the terminal and to a file named `person.txt`.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create objects holding personal data.
    Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};
    Person person2{"Sven Andersson", 37U, Gender::Male, "Kunskapsgatan 4", "Teacher", false};
    
    // Print stored personal data in the terminal.
    person1.print();
    person2.print();

    // Open file `person.txt` for writing.
    std::ofstream ofstream{"person.txt"};

    // Write the personal data to `person.txt`, then terminate the program.
    person1.print(ofstream);
    person2.print(ofstream);
    return 0;
}