#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include <string>

class Proveedor {
private:
    int id;
    std::string nombreComercial;
    std::string nombreVendedor; // opcional: persona que atiende
    std::string telefono;
    std::string correo;

public:
    Proveedor(int id, std::string nombreComercial, std::string nombreVendedor,
              std::string telefono, std::string correo);

    int getId() const;
    std::string getNombreComercial() const;
    std::string getNombreVendedor() const;
    std::string getTelefono() const;
    std::string getCorreo() const;

    void actualizarContacto(const std::string& nuevoTelefono, const std::string& nuevoCorreo);
    void definirVendedor(const std::string& nuevoVendedor);
    std::string datosResumen() const;
};

#endif // PROVEEDOR_HPP
