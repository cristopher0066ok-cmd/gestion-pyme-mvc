#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>

class Cliente {
private:
    int id;
    std::string nombre;
    std::string telefono;
    std::string correo;

public:
    Cliente(int id, std::string nombre, std::string telefono, std::string correo);

    int getId() const;
    std::string getNombre() const;
    std::string getTelefono() const;
    std::string getCorreo() const;

    void actualizarContacto(const std::string& nuevoTelefono, const std::string& nuevoCorreo);
    std::string datosResumen() const;
};

#endif // CLIENTE_HPP
