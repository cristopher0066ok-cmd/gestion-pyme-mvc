// Seccion PRODUCTOS de ModeloPYME. Responsable: Mateo.
#include "ModeloPYME.hpp"

void ModeloPYME::registrarProducto(const Producto& p) {
    if (indiceProducto(p.getCodigo()) >= 0) {
        throw ErrorDominio("Ya existe un producto registrado con el código: " + p.getCodigo());
    }
    if (p.getIdProveedor() > 0 && buscarProveedorPorId(p.getIdProveedor()) == nullptr) {
        throw ErrorDominio("El proveedor indicado no existe (ID " + std::to_string(p.getIdProveedor()) + ").");
    }
    productos.push_back(p);
}

Producto* ModeloPYME::buscarProductoPorCodigo(const std::string& codigo) {
    int i = indiceProducto(Validaciones::recortar(codigo));
    return i >= 0 ? &productos[static_cast<size_t>(i)] : nullptr;
}

std::vector<Producto> ModeloPYME::buscarProductos(const std::string& texto) const {
    std::string t = Validaciones::recortar(texto);
    std::vector<Producto> resultado;
    for (const auto& p : productos) {
        if (t.empty() || Validaciones::contiene(p.getCodigo(), t) || Validaciones::contiene(p.getNombre(), t)) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

const std::vector<Producto>& ModeloPYME::getProductos() const {
    return productos;
}

// Se trabaja sobre una copia: si algo falla, el producto original no cambia.
void ModeloPYME::editarProducto(const std::string& codigo, const std::string& nombre, double precio,
                                int stock, int stockMinimo, const DatosComerciales& datos) {
    int i = indiceProducto(Validaciones::recortar(codigo));
    if (i < 0) {
        throw ErrorDominio("No se puede editar: Producto no encontrado.");
    }
    Producto nuevo(productos[static_cast<size_t>(i)].getCodigo(), nombre, precio, stock, stockMinimo);
    nuevo.definirDatosComerciales(datos);
    if (nuevo.getIdProveedor() > 0 && buscarProveedorPorId(nuevo.getIdProveedor()) == nullptr) {
        throw ErrorDominio("El proveedor indicado no existe (ID " + std::to_string(nuevo.getIdProveedor()) + ").");
    }
    if (!productos[static_cast<size_t>(i)].isActivo()) {
        nuevo.desactivar();
    }
    productos[static_cast<size_t>(i)] = nuevo;
}

void ModeloPYME::desactivarProducto(const std::string& codigo) {
    Producto* p = buscarProductoPorCodigo(codigo);
    if (p == nullptr) {
        throw ErrorDominio("No se puede desactivar: Producto no encontrado.");
    }
    p->desactivar();
}

std::vector<Producto> ModeloPYME::productosConStockBajo() const {
    std::vector<Producto> resultado;
    for (const auto& p : productos) {
        if (p.requiereReposicion()) resultado.push_back(p);
    }
    return resultado;
}

// RF-15: activos con caducidad entre hoy y hoy + dias (inclusive).
std::vector<Producto> ModeloPYME::productosPorCaducar(const std::string& fechaHoy, int dias) const {
    if (dias < 0) {
        throw ErrorDominio("El numero de dias no puede ser negativo.");
    }
    long hoy = Validaciones::diasDesdeEpoca(fechaHoy);
    std::vector<Producto> resultado;
    for (const auto& p : productos) {
        if (!p.isActivo() || p.getFechaCaducidad().empty()) continue;
        long restantes = Validaciones::diasDesdeEpoca(p.getFechaCaducidad()) - hoy;
        if (restantes >= 0 && restantes <= dias) resultado.push_back(p);
    }
    return resultado;
}

void ModeloPYME::asignarPromocion(const std::string& codigo, double precioPromocion) {
    Producto* p = buscarProductoPorCodigo(codigo);
    if (p == nullptr) {
        throw ErrorDominio("No se puede asignar la promocion: Producto no encontrado.");
    }
    p->asignarPromocion(precioPromocion);
}
