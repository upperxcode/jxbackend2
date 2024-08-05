#pragma once

#include <cstdio>
#include <database/field>
#include <string>
#include <utils/print.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <crow/json.h>


namespace record {

// Definindo a macro para simplificar a definição de campos
#define DEFINE_FIELDS(...)                                                     \
  auto fields() { return std::tie(__VA_ARGS__); }                              \
  const auto fields() const { return std::tie(__VA_ARGS__); }

// Função para aplicar uma função a cada campo de uma tuple
template <typename Func, typename Tuple, std::size_t... I>
void for_each_in_tuple(Tuple &&t, Func &&f, std::index_sequence<I...>) {
  (f(std::get<I>(std::forward<Tuple>(t))), ...);
}

template <typename Func, typename Tuple>
void for_each_in_tuple(Tuple &&t, Func &&f) {
  constexpr auto size = std::tuple_size<std::decay_t<Tuple>>::value;
  for_each_in_tuple(std::forward<Tuple>(t), std::forward<Func>(f),
                    std::make_index_sequence<size>{});
}

// Definição do enum para os tipos de consulta SQL
enum class SQLQueryType { INSERT, UPDATE, SELECT, DELETE };

// Função para gerar consultas SQL genéricas
template <typename T>
std::string generateSQLQuery(const T &record, 
                             SQLQueryType queryType,
                             const std::string &conditions = "") {
  std::ostringstream sql;
  bool first = true;

  std::string tableName = record.tableName;

  auto formatToSql = [](const auto &member) -> std::string {
    using ValueType = std::decay_t<decltype(member.getValue())>;
    const auto &value = member.formattedValue();

    println("formatToSql => ", value, " - ", member.getValue() );
    if constexpr (std::is_same_v<ValueType, std::string> ||
                  std::is_same_v<ValueType, const char *> ||
                  std::is_same_v<ValueType, char>) {
      return "'" + value + "'";
    } else if constexpr (std::is_same_v<ValueType, bool>) {
      return member.getValue() ? "'true'" : "'false'";
    } else {
      return value;
    }
  };

  switch (queryType) {
  case SQLQueryType::INSERT: {
    std::ostringstream values;
    sql << "INSERT INTO " << tableName << " (";
    values << "VALUES (";

    for_each_in_tuple(record.fields(), [&](const auto &member) {
      if (!member.isKey()) {
        if (!first) {
          sql << ", ";
          values << ", ";
        }
        sql << member.getDb();
        values << formatToSql(member);
        first = false;
      }
    });

    sql << ") " << values.str() << ")";
    break;
  }
  case SQLQueryType::UPDATE: {
    sql << "UPDATE " << tableName << " SET ";

    for_each_in_tuple(record.fields(), [&](const auto &member) {
      if (!member.isKey()) {
        if (!first)
          sql << ", ";
        sql << member.getDb() << " = " << formatToSql(member);
        first = false;
      }
    });

    sql << " WHERE ";
    first = true;
    for_each_in_tuple(record.fields(), [&](const auto &member) {
      if (member.isKey()) {
        if (!first)
          sql << " AND ";
        sql << member.getDb() << " = " << formatToSql(member);
        first = false;
      }
    });
    sql << ";";
    break;
  }
  case SQLQueryType::SELECT: {
    sql << "SELECT ";

    for_each_in_tuple(record.fields(), [&](const auto &member) {
      if (!first)
        sql << ", ";
      sql << member.getDb();
      first = false;
    });

    sql << " FROM " << tableName;
    if (!conditions.empty()) {
      sql << " WHERE " << conditions;
    }
    sql << ";";
    break;
  }
  case SQLQueryType::DELETE: {
    sql << "DELETE FROM " << tableName << " WHERE ";

    for_each_in_tuple(record.fields(), [&](const auto &member) {
      if (member.isKey()) {
        if (!first)
          sql << " AND ";
        sql << member.getDb() << " = " << formatToSql(member);
        first = false;
      }
    });
    sql << ";";
    break;
  }
  }

  return sql.str();
}
// Função para popular a lista de registros genéricos
template <typename T>
std::vector<T> getRecords(
    const std::vector<std::unordered_map<std::string, std::string>> &result) {
  std::vector<T> records;

  try {
    for (const auto &row : result) {
      T record;
      for_each_in_tuple(record.fields(), [&](auto &member) {
        auto it = row.find(member.getDb());
        if (it != row.end() && !it->second.empty()) {
          const std::string &value = it->second;
          if constexpr (std::is_same_v<decltype(member), StringField &>) {
            member.setValue(value);
          } else if constexpr (std::is_same_v<decltype(member),
                                              IntegerField &>) {
            try {
              member.setValue(std::stoi(value));
            } catch (const std::invalid_argument &) {
              std::cerr << "Invalid integer: " << value << std::endl;
            }
          } else if constexpr (std::is_same_v<decltype(member), FloatField &>) {
            try {
              member.setValue(std::stof(value));
            } catch (const std::invalid_argument &) {
              std::cerr << "Invalid float: " << value << std::endl;
            }
          } else if constexpr (std::is_same_v<decltype(member), MoneyField &>) {
            try {
              member.setValue(std::stod(value));
            } catch (const std::invalid_argument &) {
              std::cerr << "Invalid double: " << value << std::endl;
            }
          } else if constexpr (std::is_same_v<decltype(member), BoolField &>) {
            member.setValue(value == "true");
          } else if constexpr (std::is_same_v<decltype(member),
                                              DateTimeField &>) {
            std::tm tm = {};
            std::istringstream ss(value);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            if (ss.fail())
              std::cerr << "Invalid datetime format: " << value << std::endl;
            else
              member.setValue(std::mktime(&tm));
          } else if constexpr (std::is_same_v<decltype(member), CharField &>) {
            if (!value.empty())
              member.setValue(value[0]);
          }
        }
      });
      records.push_back(record);
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return records;
}

template <typename T> crow::json::wvalue toJson(const T &record) {
  crow::json::wvalue j;
  for_each_in_tuple(record.fields(), [&](const auto &member) {
    using MemberType =
        typename std::remove_reference_t<decltype(member)>::value_type;
    const auto &value = member.getValue();

    if constexpr (std::is_same_v<MemberType, int> ||
                  std::is_same_v<MemberType, long> ||
                  std::is_same_v<MemberType, float> ||
                  std::is_same_v<MemberType, double>) {
      j[member.getJson()] = value;
    } else if constexpr (std::is_same_v<MemberType, bool>) {
      j[member.getJson()] = value ? "true" : "false";
    } else if constexpr (std::is_same_v<MemberType, std::string> ||
                         std::is_same_v<MemberType, const char *> ||
                         std::is_same_v<MemberType, char>) {
      j[member.getJson()] = value;
    } else {
      static_assert(std::is_same_v<MemberType, void>, "Unsupported type");
    }
  });
  return j;
}
// Helper template to trigger static_assert for unsupported types
template <class> inline constexpr bool always_false = false;

template <typename T> void fromJson(T &record, const crow::json::rvalue &j) {
  for_each_in_tuple(record.fields(), [&](auto &member) {
    if (j.has(member.getJson())) {
      using MemberType =
          typename std::remove_reference_t<decltype(member)>::value_type;
      const auto &jsonValue = j[member.getJson()];

      if constexpr (std::is_same_v<MemberType, int>) {
        member.setValue(jsonValue.i());
      } else if constexpr (std::is_same_v<MemberType, long>) {
        member.setValue(jsonValue.i());
      } else if constexpr (std::is_same_v<MemberType, float>) {
        member.setValue(static_cast<float>(jsonValue.d()));
      } else if constexpr (std::is_same_v<MemberType, double>) {
        member.setValue(jsonValue.d());
      } else if constexpr (std::is_same_v<MemberType, bool>) {
        member.setValue(jsonValue.b());
      } else if constexpr (std::is_same_v<MemberType, std::string>) {
        member.setValue(jsonValue.s());
      } else {
        static_assert(std::is_same_v<MemberType, void>, "Unsupported type");
      }
    }
  });
}



// Função para atualizar um registro com novos dados
template <typename T>
void updateRecord(T &record,
                  const std::unordered_map<std::string, std::string> &newData) {
  for_each_in_tuple(record.fields(), [&](auto &member) {
    auto it = newData.find(member.getDb());
    if (it != newData.end() && !it->second.empty()) {
      const std::string &value = it->second;
      if constexpr (std::is_same_v<decltype(member), StringField &>) {
        member.setValue(value);
      } else if constexpr (std::is_same_v<decltype(member), IntegerField &>) {
        try {
          member.setValue(std::stoi(value));
        } catch (const std::invalid_argument &) {
          std::cerr << "Invalid integer: " << value << std::endl;
        }
      } else if constexpr (std::is_same_v<decltype(member), FloatField &>) {
        try {
          member.setValue(std::stof(value));
        } catch (const std::invalid_argument &) {
          std::cerr << "Invalid float: " << value << std::endl;
        }
      } else if constexpr (std::is_same_v<decltype(member), MoneyField &>) {
        try {
          member.setValue(std::stod(value));
        } catch (const std::invalid_argument &) {
          std::cerr << "Invalid double: " << value << std::endl;
        }
      } else if constexpr (std::is_same_v<decltype(member), BoolField &>) {
        member.setValue(value == "true");
      } else if constexpr (std::is_same_v<decltype(member), DateTimeField &>) {
        std::tm tm = {};
        std::istringstream ss(value);
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail())
          std::cerr << "Invalid datetime format: " << value << std::endl;
        else
          member.setValue(std::mktime(&tm));
      } else if constexpr (std::is_same_v<decltype(member), CharField &>) {
        if (!value.empty())
          member.setValue(value[0]);
      }
    }
  });
}

} // namespace record
