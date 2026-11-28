// Seccion PROVEEDORES (y pendientes RF-14) de ModeloPYME. Responsable: Martin.
#include "ModeloPYME.hpp"

int ModeloPYME::registrarProveedor(const std::string& nombreComercial, const std::string& nombreVendedor,
                                   const std::string& telefono, const std::string& correo) {
    Proveedor nuevo(siguienteIdProveedor, nombreComercial, nombreVendedor, telefono, correo);
    verificarContactoUnico(nuevo.getTelefono(), nuevo.getCorreo(), 0, false);
    proveedores.push_back(nuevo);
    return siguienteIdProveedor++;
}

Proveedor* ModeloPYME::buscarProveedorPorId(int id) {
    for (auto& p : proveedores) {
        if (p.getId() == id) return &p;
    }
    return nullptr;
}

std::vector<Proveedor> ModeloPYME::buscarProveedores(const std::string& texto) const {
    std::string t = Validaciones::recortar(texto);
    std::vector<Proveedor> resultado;
    for (const auto& p : proveedores) {
        if (t.empty() || Validaciones::contiene(p.getNombreComercial(), t) ||
            Validaciones::contiene(p.getNombreVendedor(), t) ||
            Validaciones::contiene(p.getTelefono(), t) || Validaciones::contiene(p.getCorreo(), t)) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

const std::vector<Proveedor>& ModeloPYME::getProveedores() const {
    return proveedores;
}

void ModeloPYME::editarProveedor(int id, const std::string& nombreVendedor,
                                 const std::string& telefono, const std::string& correo) {
    Proveedor* actual = buscarProveedorPorId(id);
    if (actual == nullptr) {
        throw ErrorDominio("No se puede editar: Proveedor no encontrado.");
    }
    Proveedor copia = *actual;
    copia.actualizarContacto(telefono, correo);
    copia.definirVendedor(nombreVendedor);
    verificarContactoUnico(copia.getTelefono(), copia.getCorreo(), id, false);
    *actual = copia;
}

// ---------------------------------------------------------------- RF-14

int ModeloPYME::registrarPendiente(int idProveedor, SentidoPendiente sentido,
                                   const std::string& descripcion, double monto,
                                   const std::string& codigoProducto, int cantidad,
                                   const std::string& fecha) {
    if (buscarProveedorPorId(idProveedor) == nullptr) {
        throw ErrorDominio("El pendiente debe pertenecer a un proveedor existente.");
    }
    Pendiente nuevo(siguienteIdPendiente, idProveedor, sentido, descripcion, monto,
                    codigoProducto, cantidad, fecha);
    pendientes.push_back(nuevo);
    return siguienteIdPendiente++;
}

void ModeloPYME::saldarPendiente(int id, const std::string& fechaPago, const std::string& medio) {
    for (auto& p : pendientes) {
        if (p.getId() == id) {
            p.saldar(fechaPago, medio);
            return;
        }
    }
    throw ErrorDominio("No se puede saldar: Pendiente no encontrado.");
}

const std::vector<Pendiente>& ModeloPYME::getPendientes() const {
    return pendientes;
}

std::vector<Pendiente> ModeloPYME::pendientesAbiertos() const {
    std::vector<Pendiente> resultado;
    for (const auto& p : pendientes) {
        if (!p.isSaldado()) resultado.push_back(p);
    }
    return resultado;
}

std::vector<Pendiente> ModeloPYME::pendientesVencidos(const std::string& fechaHoy, int dias) const {
    if (dias < 0) {
        throw ErrorDominio("El numero de dias no puede ser negativo.");
    }
    std::vector<Pendiente> resultado;
    for (const auto& p : pendientes) {
        if (!p.isSaldado() && p.superaDias(fechaHoy, dias)) resultado.push_back(p);
    }
    return resultado;
}
