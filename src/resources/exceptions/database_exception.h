#pragma  once
#include <exception>
#include <string>

class DatabaseException : public std::exception {
public:
    explicit DatabaseException(const std::string& message)
        : message(message) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    std::string msg() const {
        return message;
    }

private:
    std::string message;
};