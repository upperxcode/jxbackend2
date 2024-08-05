#include "field.h"
#include <iomanip>

// Especialização para MoneyField
class MoneyField : public Field<double> {
public:
  using Field::Field; // Inherit constructors
  using value_type = double;

  std::string formatValue(const double &value,
                          const std::string &format) const override {
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << value;
    return oss.str();
  }

  std::string formattedValue() const override {

    if (format_.empty()) {
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(2) << value_;
      return oss.str();
    }

    return formatValue(value_, format_.empty() ? "$" : format_);
  }
  // Sobrecarga do operador de atribuição para double
  MoneyField &operator=(double value) {
    this->setValue(value);
    return *this;
  }
};
