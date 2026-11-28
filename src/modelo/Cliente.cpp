#include "Cliente.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

Cliente::Cliente(int idCliente, std::string nom, std::string tel, std::string mail) {
    if (idCliente <= 0) {
        throw ErrorDominio("El identificador del cliente debe ser positivo.");
    }
    nom = Validaciones::textoObligatorio(nom, "nombre del cliente");
    tel = Validaciones::recortar(tel);
    mail = Validaciones::recortar(mail);
    if (tel.empty() && mail.empty()) {
        throw ErrorDominio("El cliente necesita al menos un medio de contacto (telefono o correo).");
    }
    id = idCliente;
    nombre = nom;
    telefono = tel;
    correo = mail;
}

int Cliente::getId() const { return id; }
std::string Cliente::getNombre() const { return nombre; }
std::string Cliente::getTelefono() const { return telefono; }
std::string Cliente::getCorreo() const { return correo; }

void Cliente::actualizarContacto(const std::string& nuevoTelefono, const std::string& nuevoCorreo) {
    std::string tel = Validaciones::recortar(nuevoTelefono);
    std::string mail = Validaciones::recortar(nuevoCorreo);
    if (tel.empty() && mail.empty()) {
        throw ErrorDominio("El cliente necesita al menos un medio de contacto (telefono o correo).");
    }
    telefono = tel;
    correo = mail;
}

std::string Cliente::datosResumen() const {
    return "ID " + std::to_string(id) + " | " + nombre + " | Tel: " +
           (telefono.empty() ? "-" : telefono) + " | Correo: " + (correo.empty() ? "-" : correo);
}
