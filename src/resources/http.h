#pragma once

#include <crow/app.h>
#include <string>

namespace http {
// Função que define uma rota GET
template <typename Func>
inline void Get(crow::SimpleApp &app, const std::string route, Func func) {
  app.route_dynamic(route).methods(crow::HTTPMethod::Get)(func);
}

template <typename Func, typename T>
inline void Get(crow::SimpleApp &app, std::string route, Func func, T arg) {
  app.route_dynamic(route).methods(crow::HTTPMethod::Get)(func);
}

template <typename Func>
inline void Post(crow::SimpleApp &app, std::string route, Func func) {
  app.route_dynamic(route).methods(crow::HTTPMethod::Post)(func);
}

template <typename Func>
inline void Put(crow::SimpleApp &app, std::string route, Func func) {
  app.route_dynamic(route).methods(crow::HTTPMethod::Put)(func);
}

template <typename Func>
inline void Delete(crow::SimpleApp &app, std::string route, Func func) {
  app.route_dynamic(route).methods(crow::HTTPMethod::Delete)(func);
}


} // namespace http