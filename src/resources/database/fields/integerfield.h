#include "field.h"

// Especialização para IntegerField
class IntegerField : public Field<int> {
public:
    using Field::Field; // Inherit constructors
    using value_type = int;

     // Sobrecarga do operador de atribuição para int
    IntegerField& operator=(int value) {
        this->setValue(value);
        return *this;
    }
};

