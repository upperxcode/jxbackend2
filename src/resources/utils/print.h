#ifndef PRINT_H
#define PRINT_H

#include <iostream>

// Macro para verificar se estamos em modo de depuração
#ifdef DEBUG
#define DEBUG_MODE true
#else
#define DEBUG_MODE false
#endif



// Códigos de cores ANSI
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";
const std::string MAGENTA = "\033[35m";
const std::string WHITE = "\033[37m";

// Função base para impressão
template <typename T>
void print(T t) {
    std::cout << t;
}

template <typename T, typename... Args>
void print(T t, Args... args) {
    std::cout << t;
    print(args...);
}

template <typename T>
void println(T t) {
    print(t);
    std::cout << std::endl;
}

template <typename T, typename... Args>
void println(T t, Args... args) {
    print(t, args...);
    std::cout << std::endl;
}

namespace console {

// Função base para logs com cor
template <typename T, typename... Args>
void logBase(const std::string &color, const std::string &label, T t, Args... args) {
    const char *line = "------------------------------------------------------------------------------------------------\n";
    std::cout << color << line << "[" << label << "] ";
    print(t, args...);
    std::cout << std::endl << line << RESET;
}

// Funções específicas para cada tipo de log
template <typename T, typename... Args>
void log(T t, Args... args) {
    logBase(WHITE, "LOG", t, args...);
}

template <typename T, typename... Args>
void info(T t, Args... args) {
    logBase(GREEN, "INFO", t, args...);
}

template <typename T, typename... Args>
void warning(T t, Args... args) {
    logBase(YELLOW, "WARNING", t, args...);
}

template <typename T, typename... Args>
void error(T t, Args... args) {
    logBase(RED, "ERROR", t, args...);
}

template <typename T, typename... Args>
void debug(const char *file, int line, const char *func, T t, Args... args) {
    if (DEBUG_MODE) {
        std::cout << MAGENTA << "[DEBUG] " << file << ":" << line << " (" << func << ") ";
        print(t, args...);
        std::cout << std::endl << RESET;
    }
}

} // namespace console

// Macro para facilitar a chamada da função debug com informações de arquivo, linha e função
#define LOG_DEBUG(...) console::debug(__FILE__, __LINE__, __func__, __VA_ARGS__)

#endif // PRINT_H