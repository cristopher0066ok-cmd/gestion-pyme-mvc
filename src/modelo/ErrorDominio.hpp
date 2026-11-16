#ifndef ERROR_DOMINIO_HPP
#define ERROR_DOMINIO_HPP

#include <stdexcept>
#include <string>

class ErrorDominio : public std::runtime_error {
public:
    explicit ErrorDominio(const std::string& mensaje) : std::runtime_error(mensaje) {}
};

#endif // ERROR_DOMINIO_HPP
