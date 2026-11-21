// Seccion PRODUCTOS de ModeloPYME. Responsable: Mateo.
#include "ModeloPYME.hpp"
#include <algorithm>

void ModeloPYME::registrarProducto(const Producto& p) {
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
            return &prod;
        }
    }
    return nullptr;
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
