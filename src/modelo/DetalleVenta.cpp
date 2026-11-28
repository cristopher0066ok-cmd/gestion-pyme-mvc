#include "DetalleVenta.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

DetalleVenta::DetalleVenta(std::string codigo, int cant, double precio) {
    codigo = Validaciones::textoObligatorio(codigo, "codigo del producto");
    if (cant <= 0) {
        throw ErrorDominio("La cantidad de cada detalle debe ser positiva.");
    }
    if (precio < 0) {
        throw ErrorDominio("El precio unitario no puede ser negativo.");
    }
    codigoProducto = codigo;
    cantidad = cant;
    precioUnitario = precio;
}

std::string DetalleVenta::getCodigoProducto() const { return codigoProducto; }
int DetalleVenta::getCantidad() const { return cantidad; }
double DetalleVenta::getPrecioUnitario() const { return precioUnitario; }
double DetalleVenta::calcularSubtotal() const { return cantidad * precioUnitario; }
