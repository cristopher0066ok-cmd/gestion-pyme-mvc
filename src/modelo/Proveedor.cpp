#include "Proveedor.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

Proveedor::Proveedor(int idProveedor, std::string nomComercial, std::string nomVendedor,
                     std::string tel, std::string mail) {
    if (idProveedor <= 0) {
        throw ErrorDominio("El identificador del proveedor debe ser positivo.");
    }
    nomComercial = Validaciones::textoObligatorio(nomComercial, "nombre del proveedor");
    tel = Validaciones::recortar(tel);
    mail = Validaciones::recortar(mail);
    if (tel.empty() && mail.empty()) {
        throw ErrorDominio("El proveedor necesita al menos un medio de contacto (telefono o correo).");
    }
    id = idProveedor;
    nombreComercial = nomComercial;
    nombreVendedor = Validaciones::recortar(nomVendedor);
    telefono = tel;
    correo = mail;
}

int Proveedor::getId() const { return id; }
std::string Proveedor::getNombreComercial() const { return nombreComercial; }
std::string Proveedor::getNombreVendedor() const { return nombreVendedor; }
std::string Proveedor::getTelefono() const { return telefono; }
std::string Proveedor::getCorreo() const { return correo; }

void Proveedor::actualizarContacto(const std::string& nuevoTelefono, const std::string& nuevoCorreo) {
    std::string tel = Validaciones::recortar(nuevoTelefono);
    std::string mail = Validaciones::recortar(nuevoCorreo);
    if (tel.empty() && mail.empty()) {
        throw ErrorDominio("El proveedor necesita al menos un medio de contacto (telefono o correo).");
    }
    telefono = tel;
    correo = mail;
}

void Proveedor::definirVendedor(const std::string& nuevoVendedor) {
    nombreVendedor = Validaciones::recortar(nuevoVendedor);
}

std::string Proveedor::datosResumen() const {
    return "ID " + std::to_string(id) + " | " + nombreComercial + " | Atiende: " +
           (nombreVendedor.empty() ? "-" : nombreVendedor) + " | Tel: " +
           (telefono.empty() ? "-" : telefono) + " | Correo: " + (correo.empty() ? "-" : correo);
}
