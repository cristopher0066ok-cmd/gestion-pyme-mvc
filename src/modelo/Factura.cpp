#include "Factura.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

DetalleFactura::DetalleFactura(std::string codigo, int cant, double precio) {
    codigo = Validaciones::textoObligatorio(codigo, "codigo del producto");
    if (cant <= 0) {
        throw ErrorDominio("La cantidad de cada detalle de la factura debe ser positiva.");
    }
    if (precio < 0) {
        throw ErrorDominio("El precio de compra no puede ser negativo.");
    }
    codigoProducto = codigo;
    cantidad = cant;
    precioCompra = precio;
}

std::string DetalleFactura::getCodigoProducto() const { return codigoProducto; }
int DetalleFactura::getCantidad() const { return cantidad; }
double DetalleFactura::getPrecioCompra() const { return precioCompra; }
double DetalleFactura::calcularSubtotal() const { return cantidad * precioCompra; }

Factura::Factura(std::string num, std::string clave, std::string fec, int proveedor,
                 std::string atendido, std::string ruta) {
    num = Validaciones::textoObligatorio(num, "numero de factura");
    if (!Validaciones::fechaValida(fec)) {
        throw ErrorDominio("La fecha de la factura debe tener el formato AAAA-MM-DD y ser valida.");
    }
    if (proveedor <= 0) {
        throw ErrorDominio("La factura debe pertenecer a un proveedor.");
    }
    numero = num;
    claveAcceso = Validaciones::recortar(clave);
    fecha = fec;
    idProveedor = proveedor;
    atendidoPor = Validaciones::recortar(atendido);
    rutaFoto = Validaciones::recortar(ruta);
}

std::string Factura::getNumero() const { return numero; }
std::string Factura::getClaveAcceso() const { return claveAcceso; }
std::string Factura::getFecha() const { return fecha; }
int Factura::getIdProveedor() const { return idProveedor; }
std::string Factura::getAtendidoPor() const { return atendidoPor; }
std::string Factura::getRutaFoto() const { return rutaFoto; }
const std::vector<DetalleFactura>& Factura::getDetalles() const { return detalles; }

double Factura::calcularTotal() const {
    double suma = 0;
    for (const auto& d : detalles) suma += d.calcularSubtotal();
    return suma;
}

bool Factura::incluyeProducto(const std::string& codigo) const {
    for (const auto& d : detalles) {
        if (d.getCodigoProducto() == codigo) return true;
    }
    return false;
}

void Factura::agregarDetalle(const DetalleFactura& detalle) {
    detalles.push_back(detalle);
}
