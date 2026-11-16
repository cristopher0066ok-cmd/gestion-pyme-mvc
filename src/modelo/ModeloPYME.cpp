#include "ModeloPYME.hpp"
#include "ErrorDominio.hpp"
#include <algorithm>

ModeloPYME::ModeloPYME() {
    // Constructor por defecto
}

void ModeloPYME::registrarProducto(const Producto& p) {
    // Validar si ya existe un producto con el mismo código (Regla de negocio)
    for (const auto& prod : productos) {
        if (prod.getCodigo() == p.getCodigo()) {
            throw ErrorDominio("Ya existe un producto registrado con el código: " + p.getCodigo());
        }
    }
    productos.push_back(p);
}

Producto* ModeloPYME::buscarProductoPorCodigo(const std::string& codigo) {
    for (auto& prod : productos) {
        if (prod.getCodigo() == codigo) {
            return &prod; // Retorna puntero al producto encontrado
        }
    }
    return nullptr; // Retorna nullptr si no existe
}

const std::vector<Producto>& ModeloPYME::getProductos() const {
    return productos;
}

void ModeloPYME::desactivarProducto(const std::string& codigo) {
    Producto* p = buscarProductoPorCodigo(codigo);
    if (p == nullptr) {
        throw ErrorDominio("No se puede desactivar: Producto no encontrado.");
    }
    p->desactivar();
}

void ModeloPYME::cargarDatos() {
    // Método reservado para la capa de persistencia (archivos CSV)
}

void ModeloPYME::guardarDatos() {
    // Método reservado para la capa de persistencia (archivos CSV)
}