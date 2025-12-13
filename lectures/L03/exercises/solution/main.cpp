/**
 * @brief Test of solution for L03 exercise.
 */
#include <fstream>
#include <iostream>

/**
 * @brief Enumeration of product status.
 */
enum class ProductStatus 
{ 
    Prototype,    /** The product is a prototype. */
    Active,       /** The product is active. */
    Discontinued, /** The product is discontinued. */
};

/**
 * @brief Implementation of a single device.
 */
class Device
{
public:
    /**
     * @brief Creates new device.
     * 
     * @param[in] manufacturer Device manufacturer.
     * @param[in] model Device model.
     * @param[in] revision Device revision.
     * @param[in] yearOfManufacture The year the device was manufactured.
     * @param[in] color Device color.
     * @param[in] productStatus Product status of the device (default = active).
     */
    Device(const char* manufacturer, const char* model, const unsigned revision,
           const unsigned yearOfManufacture, const char* color, 
           const ProductStatus productStatus = ProductStatus::Active);

    /**
     * @brief Delete the device.
     */
    ~Device() = default;

    /**
     * @brief Get the device manufacturer.
     * 
     * @return The device manufacturer as a string.
     */
    const char* manufacturer() const { return myManufacturer; }

    /**
     * @brief Get the device model.
     * 
     * @return The device model as a string.
     */
    const char* model() const { return myModel; }

    /**
     * @brief Get the device revision.
     * 
     * @return The revision as an unsigned integer.
     */
    unsigned revision() const { return myRevision; }

    /**
     * @brief Get the year the device was manufactured.
     * 
     * @return The year the device was manufactured as an unsigned integer.
     */
    unsigned yearOfManufacture() const { return myYearOfManufacture; }

    /**
     * @brief Get the device color.
     * 
     * @return The device color as a string.
     */
    const char* color() const { return myColor; }
    
    /**
     * @brief Get the product status of the device.
     * 
     * @return The product status of the device as an enum.
     */
    ProductStatus productStatus() const { return myProductStatus; }

    /**
     * @brief Set the color of the device.
     * 
     * @param[in] color The new device color.
     */
    void setColor(const char* color) { myColor = color; }

    /**
     * @brief Set the product status of the device.
     * 
     * @param[in] productStatus The new product status of the device.
     */
    void setProductStatus(const ProductStatus productStatus) { myProductStatus = productStatus; }

    /**
     * @brief Print device information.
     * 
     * @param[in] ostream The output stream to use (default = terminal print).
     */
    void print(std::ostream& ostream = std::cout) const
    {
        ostream << "--------------------------------------------------------------------------------\n";
        ostream << "Manufacturer:\t\t" << myManufacturer << "\n";
        ostream << "Model:\t\t\t" << myModel << "\n";
        ostream << "Revision:\t\t" << myRevision << "\n";
        ostream << "Year of manufacture:\t" << myYearOfManufacture << "\n";
        ostream << "Color:\t\t\t" << myColor << "\n";
        ostream << "Product status:\t\t" << productStatusAsText() << "\n";
        ostream << "--------------------------------------------------------------------------------\n\n";
    }

    Device() = delete; // No default constructor.

private:
    /**
     * @brief Get the product status of the device as a string.
     * 
     * @return The product status of the device as a string.
     */
    const char* productStatusAsText() const
    {
        switch (myProductStatus)
        {
            case ProductStatus::Prototype: return "Prototype";
            case ProductStatus::Active   : return "Active";
            default                      : return "Discontinued";
        }
    }

    /** Device manufacturer. */
    const char* myManufacturer;

    /** Device model. */
    const char* myModel;

    /** Device revision. */
    unsigned myRevision;

    /** The year the device was manufactured. */
    unsigned myYearOfManufacture;

    /** The color of the device. */
    const char* myColor;

    /** Product status of this device. */
    ProductStatus myProductStatus;
};

/**
 * @brief Print information about a device in the terminal and to a file.
 * 
 * @return 0 on termination of the program.
 */
int main()
{    
    // Store information about the `Arduino Uno Rev3`.
    Device device1{"Arduino", "Uno", 3, 2012, "Green"};

    // Print stored device data in the terminal.
    device1.print();

    // Open file `device.txt` for writing.
    std::ofstream ostream{"device.txt"};

    // Write stored device data to `device.txt`, then terminate the program. 
    device1.print(ostream);
    return 0;
}