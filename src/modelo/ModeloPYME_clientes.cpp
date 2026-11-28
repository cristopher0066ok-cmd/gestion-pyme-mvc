// Seccion CLIENTES de ModeloPYME. Responsable: Martin.
#include "ModeloPYME.hpp"

// Un telefono o correo (no vacio) no puede repetirse entre clientes (ni entre proveedores).
void ModeloPYME::verificarContactoUnico(const std::string& telefono, const std::string& correo,
                                        int idPropio, bool esCliente) const {
    std::string mail = Validaciones::aMinusculas(correo);
    if (esCliente) {
        for (const auto& c : clientes) {
            if (c.getId() == idPropio) continue;
            if (!telefono.empty() && c.getTelefono() == telefono) {
                throw ErrorDominio("Ya existe un cliente con ese telefono (ID " + std::to_string(c.getId()) + ").");
            }
            if (!mail.empty() && Validaciones::aMinusculas(c.getCorreo()) == mail) {
                throw ErrorDominio("Ya existe un cliente con ese correo (ID " + std::to_string(c.getId()) + ").");
            }
        }
    } else {
        for (const auto& p : proveedores) {
            if (p.getId() == idPropio) continue;
            if (!telefono.empty() && p.getTelefono() == telefono) {
                throw ErrorDominio("Ya existe un proveedor con ese telefono (ID " + std::to_string(p.getId()) + ").");
            }
            if (!mail.empty() && Validaciones::aMinusculas(p.getCorreo()) == mail) {
                throw ErrorDominio("Ya existe un proveedor con ese correo (ID " + std::to_string(p.getId()) + ").");
            }
        }
    }
}

int ModeloPYME::registrarCliente(const std::string& nombre, const std::string& telefono,
                                 const std::string& correo) {
    Cliente nuevo(siguienteIdCliente, nombre, telefono, correo); // valida nombre y contacto
    verificarContactoUnico(nuevo.getTelefono(), nuevo.getCorreo(), 0, true);
    clientes.push_back(nuevo);
    return siguienteIdCliente++;
}

Cliente* ModeloPYME::buscarClientePorId(int id) {
    for (auto& c : clientes) {
        if (c.getId() == id) return &c;
    }
    return nullptr;
}

std::vector<Cliente> ModeloPYME::buscarClientes(const std::string& texto) const {
    std::string t = Validaciones::recortar(texto);
    std::vector<Cliente> resultado;
    for (const auto& c : clientes) {
        if (t.empty() || Validaciones::contiene(c.getNombre(), t) ||
            Validaciones::contiene(c.getTelefono(), t) || Validaciones::contiene(c.getCorreo(), t)) {
            resultado.push_back(c);
        }
    }
    return resultado;
}

const std::vector<Cliente>& ModeloPYME::getClientes() const {
    return clientes;
}

void ModeloPYME::editarCliente(int id, const std::string& telefono, const std::string& correo) {
    Cliente* actual = buscarClientePorId(id);
    if (actual == nullptr) {
        throw ErrorDominio("No se puede editar: Cliente no encontrado.");
    }
    Cliente copia = *actual;
    copia.actualizarContacto(telefono, correo);
    verificarContactoUnico(copia.getTelefono(), copia.getCorreo(), id, true);
    *actual = copia;
}
