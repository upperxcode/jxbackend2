#include "field.h"

// Especialização para DateTimeField

class DateTimeField : public Field<std::time_t> {
public:
  using Field::Field; // Inherit constructors
  using value_type = std::time_t;

  std::string
  formatValue(const std::time_t &value,
              const std::string &format = "%Y-%m-%d %H:%M:%S") const override {
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), format.c_str(),
                  std::localtime(&value));
    return buffer;
  }

  // Método para definir a data e hora atuais
  void setCurrentTime() {
    std::time_t now = std::time(nullptr);
    this->setValue(now);
  }

  // Método para obter a data e hora no formato do banco de dados
  std::string getDatabaseFormat() const {
    return formatValue(value_, "%Y-%m-%d %H:%M:%S");
  }

  // Sobrecarga do método formattedValue para usar o formato padrão, se não
  // especificado
  std::string formattedValue() const override {
    return formatValue(value_, format_.empty() ? "%Y-%m-%d %H:%M:%S" : format_);
  }

 std::string getValue() const {
        return formattedValue();
    }
  // Sobrecarga do operador de atribuição para std::time_t
  DateTimeField &operator=(std::time_t value) {
    this->setValue(value);
    return *this;
  }
};
