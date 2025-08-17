/**
 * @brief Personal data implementation details.
 */
#include <iostream>

#include "person.h"

// -----------------------------------------------------------------------------
Person::Person(const char* name, const unsigned age, const Gender gender, const char* address, 
               const char* occupation, const bool single) noexcept
    : myName{name}
    , myAge{age}
    , myGender{gender}
    , myAddress{address}
    , myOccupation{occupation}
    , myIsSingle{single} 
{
}

// -----------------------------------------------------------------------------
const char* Person::name() const noexcept { return myName; }

// -----------------------------------------------------------------------------
unsigned Person::age() const noexcept { return myAge; }

// -----------------------------------------------------------------------------
Gender Person::gender() const noexcept { return myGender; }

// -----------------------------------------------------------------------------
const char* Person::address() const noexcept { return myAddress; }

// -----------------------------------------------------------------------------
const char* Person::occupation() const noexcept { return myOccupation; }

// -----------------------------------------------------------------------------
bool Person::isSingle() const noexcept { return myIsSingle; }

// -----------------------------------------------------------------------------
void Person::setAddress(const char* address) noexcept { myAddress = address; }

// -----------------------------------------------------------------------------
void Person::setOccupation(const char* occupation) noexcept { myOccupation = occupation; }

// -----------------------------------------------------------------------------
void Person::setSingle(const bool single) noexcept { myIsSingle = single; }

// -----------------------------------------------------------------------------
void Person::print(std::ostream& ostream) const
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

// -----------------------------------------------------------------------------
const char* Person::genderStr() const noexcept
{
    if (myGender == Gender::Male) { return "Male"; }
    else if (myGender == Gender::Female) { return "Female"; }
    else { return "Other"; }
}

// -----------------------------------------------------------------------------
const char* Person::isSingleStr() const noexcept { return myIsSingle ? "Yes" : "No"; }
