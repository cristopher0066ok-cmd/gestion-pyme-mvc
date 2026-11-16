#include "Producto.hpp"
#include "ErrorDominio.hpp"

Producto::Producto(std::string cod, std::string nom, double prec, int stk, int stkMin) {
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
    activo = true;
}

// Getters
std::string Producto::getCodigo() const { return codigo; }
std::string Producto::getNombre() const { return nombre; }
double Producto::getPrecio() const { return precio; }
int Producto::getStock() const { return stock; }
int Producto::getStockMinimo() const { return stockMinimo; }
bool Producto::isActivo() const { return activo; }

// Métodos de negocio y validaciones
void Producto::actualizarPrecio(double nuevoPrecio) {
    if (nuevoPrecio < 0) {
        throw ErrorDominio("El nuevo precio no puede ser negativo.");
    }
    precio = nuevoPrecio;
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
