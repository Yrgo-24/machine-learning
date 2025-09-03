/**
 * @brief Button interface for using miscellaneous buttons.
 */
#pragma once

namespace driver
{
/**
 * @brief Button interface for using miscellaneous buttons.
 */
class ButtonInterface
{
public:
    /**
     * @brief Delete the button.
     * 
     * @note This destructor is used to call the destructor of the sub class
     *       before the button is deleted.
     */
    virtual ~ButtonInterface() noexcept = default;

    /**
     * @brief Read the button input.
     * 
     * @return True if the button is pressed, false otherwise.
     */
    virtual bool input() const = 0;
};
} // namespace driver