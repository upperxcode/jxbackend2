// Database.h
#pragma once

#include <vector>
#include <string>
#include <unordered_map>

using Row = std::unordered_map<std::string, std::string>;
using Result = std::vector<Row>;

class Database {
public:
    virtual ~Database() = default;

    virtual Result queryRow(const std::string& query) = 0;
    virtual Result queryRows(const std::string& query) = 0;
    virtual int exec(const std::string& query) = 0;
};