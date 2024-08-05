#pragma once
#include "utils/print.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

// Classe base genérica para todos os campos
template <typename T>
class Field {
public:
    // Construtor
    Field(const std::string& json = "", bool key = false, const std::string& db = "", bool visible = true, const std::string& format = "")
        : json_(json), db_(db), visible_(visible), key_(key), format_(format) {
             if (db == "") {
                db_ = json;
            }
            //debug("criado => ", db_);
           }

    // Métodos de acesso
    
    const T& getValue() const { return value_; }
    void setValue(const T& value) { value_ = value; }

    const std::string& getJson() const { return json_; }
    void setJson(const std::string& json) { json_ = json; }

    const std::string& getDb() const { return db_; }
    void setDb(const std::string& db) { db_ = db; }

    bool isVisible() const { return visible_; }
    void setVisible(bool visible) { visible_ = visible; }

    bool isKey() const { return key_; }
    void setKey(bool key) { key_ = key; }

    const std::string& getFormat() const { return format_; }
    void setFormat(const std::string& format) { format_ = format; }

    bool isEmpty() const { return value_ == T(); }

    virtual std::string formattedValue() const {
        if (format_.empty()) {
            return toString(value_);
        } else {
            return formatValue(value_, format_);
        }
    }

    // Sobrecarga do operador de atribuição
    Field& operator=(const T& value) {
        value_ = value;
        return *this;
    }

protected:
    T value_;
    std::string json_;
    std::string db_;
    bool visible_;
    bool key_;
    std::string format_;

    std::string toString(const T& value) const {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    virtual std::string formatValue(const T& value, const std::string& format) const {
        return toString(value); // Implementação padrão, pode ser especializada
    }
};

// Sobrecarga do operador de saída para Field
template <typename T>
std::ostream& operator<<(std::ostream& os, const Field<T>& field) {
    if (field.isVisible()) {
        os << field.formattedValue();
    }
    return os;
}