#include "Producto.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

Producto::Producto(std::string cod, std::string nom, double prec, int stk, int stkMin)
    : precio(0), stock(0), stockMinimo(0), activo(true), unidadVenta("unidad"),
      precioCompra(0), idProveedor(0), precioPromocion(0) {
    cod = Validaciones::recortar(cod);
    nom = Validaciones::recortar(nom);
    if (cod.empty()) {
        throw ErrorDominio("El código del producto no puede estar vacío.");
    }
    if (nom.empty()) {
        throw ErrorDominio("El nombre del producto no puede estar vacío.");
    }
    if (prec < 0) {
        throw ErrorDominio("El precio del producto no puede ser negativo.");
    }
    if (stk < 0 || stkMin < 0) {
        throw ErrorDominio("El stock y el stock mínimo no pueden ser negativos.");
    }

    codigo = cod;
    nombre = nom;
    precio = prec;
    stock = stk;
    stockMinimo = stkMin;
}

// Getters
std::string Producto::getCodigo() const { return codigo; }
std::string Producto::getNombre() const { return nombre; }
double Producto::getPrecio() const { return precio; }
int Producto::getStock() const { return stock; }
int Producto::getStockMinimo() const { return stockMinimo; }
bool Producto::isActivo() const { return activo; }
std::string Producto::getCategoria() const { return categoria; }
std::string Producto::getUnidadVenta() const { return unidadVenta; }
double Producto::getPrecioCompra() const { return precioCompra; }
std::string Producto::getFechaCaducidad() const { return fechaCaducidad; }
int Producto::getIdProveedor() const { return idProveedor; }
std::string Producto::getVariante() const { return variante; }
double Producto::getPrecioPromocion() const { return precioPromocion; }
bool Producto::tienePromocion() const { return precioPromocion > 0; }
double Producto::getPrecioVigente() const { return tienePromocion() ? precioPromocion : precio; }

// Métodos de negocio y validaciones
void Producto::actualizarPrecio(double nuevoPrecio) {
    if (nuevoPrecio < 0) {
        throw ErrorDominio("El nuevo precio no puede ser negativo.");
    }
    precio = nuevoPrecio;
    if (precioPromocion > precio) {
        precioPromocion = 0; // la promocion ya no es valida
    }
}

void Producto::ajustarStock(int nuevoStock) {
    if (nuevoStock < 0) {
        throw ErrorDominio("El stock no puede ajustarse a un valor negativo.");
    }
    stock = nuevoStock;
}

void Producto::vender(int cantidad) {
    if (!activo) {
        throw ErrorDominio("No se puede vender un producto desactivado.");
    }
    if (cantidad <= 0) {
        throw ErrorDominio("La cantidad a vender debe ser mayor a cero.");
    }
    if (cantidad > stock) {
        throw ErrorDominio("Stock insuficiente para realizar la venta.");
    }
    stock -= cantidad;
}

void Producto::desactivar() {
    activo = false;
}

bool Producto::requiereReposicion() const {
    return stock <= stockMinimo;
}

// RF-10: se valida todo antes de asignar, para no dejar el producto a medias.
void Producto::definirDatosComerciales(const DatosComerciales& d) {
    std::string unidad = Validaciones::aMinusculas(Validaciones::recortar(d.unidadVenta));
    if (unidad != "unidad" && unidad != "bulto") {
        throw ErrorDominio("La unidad de venta solo puede ser 'unidad' o 'bulto'.");
    }
    if (d.precioCompra < 0) {
        throw ErrorDominio("El precio de compra no puede ser negativo.");
    }
    std::string caducidad = Validaciones::recortar(d.fechaCaducidad);
    if (!caducidad.empty() && !Validaciones::fechaValida(caducidad)) {
        throw ErrorDominio("La fecha de caducidad debe tener el formato AAAA-MM-DD y ser una fecha valida.");
    }
    if (d.idProveedor < 0) {
        throw ErrorDominio("El identificador del proveedor no puede ser negativo.");
    }

    categoria = Validaciones::recortar(d.categoria);
    unidadVenta = unidad;
    precioCompra = d.precioCompra;
    fechaCaducidad = caducidad;
    idProveedor = d.idProveedor;
    variante = Validaciones::recortar(d.variante);
    if (precioPromocion > 0 && precioPromocion < precioCompra) {
        precioPromocion = 0;
    }
}

void Producto::agregarStock(int cantidad) {
    if (cantidad <= 0) {
        throw ErrorDominio("La cantidad a ingresar al stock debe ser mayor a cero.");
    }
    stock += cantidad;
}

void Producto::actualizarPrecioCompra(double nuevoPrecioCompra) {
    if (nuevoPrecioCompra < 0) {
        throw ErrorDominio("El precio de compra no puede ser negativo.");
    }
    precioCompra = nuevoPrecioCompra;
}

void Producto::asignarPromocion(double nuevoPrecio) {
    if (!activo) {
        throw ErrorDominio("Solo se puede aplicar una promocion a un producto activo.");
    }
    if (nuevoPrecio <= 0) {
        throw ErrorDominio("El precio de promocion debe ser mayor a cero.");
    }
    if (nuevoPrecio < precioCompra) {
        throw ErrorDominio("El precio de promocion no puede ser menor al precio de compra.");
    }
    if (nuevoPrecio > precio) {
        throw ErrorDominio("El precio de promocion no puede ser mayor al precio normal.");
    }
    precioPromocion = nuevoPrecio;
}

void Producto::quitarPromocion() {
    precioPromocion = 0;
}
