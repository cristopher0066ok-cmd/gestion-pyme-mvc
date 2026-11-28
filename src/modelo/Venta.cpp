#include "Venta.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

Venta::Venta(int num, std::string fec, std::string hor, int cliente)
    : numero(0), idCliente(0), total(0), montoRecibido(0), vuelto(0),
      transferenciaRecibida(false), pagoDefinido(false), confirmada(false) {
    if (num <= 0) {
        throw ErrorDominio("El numero de venta debe ser positivo.");
    }
    if (!Validaciones::fechaValida(fec)) {
        throw ErrorDominio("La fecha de la venta debe tener el formato AAAA-MM-DD y ser valida.");
    }
    if (!Validaciones::horaValida(hor)) {
        throw ErrorDominio("La hora de la venta debe tener el formato HH:MM y ser valida.");
    }
    if (cliente <= 0) {
        throw ErrorDominio("La venta debe pertenecer a un cliente.");
    }
    numero = num;
    fecha = fec;
    hora = hor;
    idCliente = cliente;
}

int Venta::getNumero() const { return numero; }
std::string Venta::getFecha() const { return fecha; }
std::string Venta::getHora() const { return hora; }
int Venta::getIdCliente() const { return idCliente; }
const std::vector<DetalleVenta>& Venta::getDetalles() const { return detalles; }
double Venta::getTotal() const { return total; }
std::string Venta::getMetodoPago() const { return metodoPago; }
std::string Venta::getBanco() const { return banco; }
double Venta::getMontoRecibido() const { return montoRecibido; }
double Venta::getVuelto() const { return vuelto; }
bool Venta::isTransferenciaRecibida() const { return transferenciaRecibida; }
bool Venta::isConfirmada() const { return confirmada; }

void Venta::agregarDetalle(const DetalleVenta& detalle) {
    if (confirmada) {
        throw ErrorDominio("No se pueden agregar detalles a una venta confirmada.");
    }
    detalles.push_back(detalle);
    calcularTotal();
}

double Venta::calcularTotal() {
    double suma = 0;
    for (const auto& d : detalles) {
        suma += d.calcularSubtotal();
    }
    total = suma;
    return total;
}

void Venta::definirPago(const std::string& metodo, const std::string& nombreBanco,
                        double monto, bool recibida) {
    if (confirmada) {
        throw ErrorDominio("La venta ya esta confirmada.");
    }
    std::string m = Validaciones::aMinusculas(Validaciones::recortar(metodo));
    calcularTotal();
    if (m == "efectivo") {
        if (monto < total) {
            throw ErrorDominio("El monto recibido en efectivo es menor al total de la venta.");
        }
        metodoPago = m;
        banco.clear();
        montoRecibido = monto;
        vuelto = monto - total;
        transferenciaRecibida = false;
    } else if (m == "transferencia") {
        std::string b = Validaciones::recortar(nombreBanco);
        if (b.empty()) {
            throw ErrorDominio("La transferencia requiere indicar el banco.");
        }
        if (!recibida) {
            throw ErrorDominio("La transferencia debe estar marcada como recibida para confirmar la venta.");
        }
        metodoPago = m;
        banco = b;
        montoRecibido = total;
        vuelto = 0;
        transferenciaRecibida = true;
    } else {
        throw ErrorDominio("El metodo de pago solo puede ser 'efectivo' o 'transferencia'.");
    }
    pagoDefinido = true;
}

void Venta::confirmar() {
    if (confirmada) {
        throw ErrorDominio("La venta ya esta confirmada.");
    }
    if (detalles.empty()) {
        throw ErrorDominio("Una venta debe tener al menos un detalle.");
    }
    if (!pagoDefinido) {
        throw ErrorDominio("El pago de la venta esta incompleto.");
    }
    calcularTotal();
    confirmada = true;
}
