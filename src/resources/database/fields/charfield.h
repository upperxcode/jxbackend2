#include "field.h"

// Especialização para CharField
class CharField : public Field<char> {
public:
    using Field::Field; // Inherit constructors
     using value_type = char;

       // Sobrecarga do operador de atribuição para char
    CharField& operator=(char value) {
        this->setValue(value);
        return *this;
    }
};