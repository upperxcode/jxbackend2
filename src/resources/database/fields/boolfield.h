#include "field.h"
#include <iostream>

// Especialização para BoolField
class BoolField : public Field<bool> {
public:
    using Field::Field; // Inherit constructors
    using value_type = bool;
    std::string formatValue(const bool& value, const std::string& format) const override {
        return value ? "true" : "false";
    }


       // Sobrecarga do operador de atribuição para bool
    BoolField& operator=(bool value) {
        this->setValue(value);
        return *this;
    }
};

