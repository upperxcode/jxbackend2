#include "field.h"
#include <iomanip>
// Especialização para FloatField
class FloatField : public Field<float> {
public:
    using Field::Field; // Inherit constructors
    using value_type = float;
    
    std::string formatValue(const float& value, const std::string& format) const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(3) << value;
        return oss.str();
    }

     std::string formattedValue() const override {

    if (format_.empty()) {
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(3) << value_;
      return oss.str();
    }

    return formatValue(value_, format_.empty() ? "$" : format_);
  }

     // Sobrecarga do operador de atribuição para float
    FloatField& operator=(float value) {
        this->setValue(value);
        return *this;
    }
};

