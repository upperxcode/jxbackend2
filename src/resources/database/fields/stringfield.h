#include "field.h"



// Especialização para StringField
class StringField : public Field<std::string> {
public:
    using Field::Field; // Inherit constructors
     using value_type = std::string;

    std::string formatValue(const std::string& value, const std::string& format) const override {
        return value; // No special formatting for strings
    }

    // Sobrecarga do operador de atribuição para const char*
    StringField& operator=(const char* value) {
        this->setValue(std::string(value));
        return *this;
    }
};