#include "Pendiente.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

std::string sentidoATexto(SentidoPendiente s) {
    return s == SentidoPendiente::LocalDebe ? "local_debe" : "proveedor_debe";
}

SentidoPendiente textoASentido(const std::string& t) {
    if (t == "local_debe") return SentidoPendiente::LocalDebe;
    if (t == "proveedor_debe") return SentidoPendiente::ProveedorDebe;
    throw ErrorDominio("Sentido de pendiente desconocido: '" + t + "'.");
}

Pendiente::Pendiente(int idPendiente, int proveedor, SentidoPendiente sent, std::string desc,
                     double m, std::string codigo, int cant, std::string fec)
    : id(0), idProveedor(0), sentido(sent), monto(0), cantidad(0), saldado(false) {
    if (idPendiente <= 0) {
        throw ErrorDominio("El identificador del pendiente debe ser positivo.");
    }
    if (proveedor <= 0) {
        throw ErrorDominio("El pendiente debe pertenecer a un proveedor.");
    }
    if (!Validaciones::fechaValida(fec)) {
        throw ErrorDominio("La fecha del pendiente debe tener el formato AAAA-MM-DD y ser valida.");
    }
    desc = Validaciones::textoObligatorio(desc, "descripcion del pendiente");
    codigo = Validaciones::recortar(codigo);
    if (m < 0) {
        throw ErrorDominio("El monto del pendiente no puede ser negativo.");
    }
    bool conDinero = m > 0;
    bool conProducto = !codigo.empty() || cant != 0;
    if (conDinero == conProducto) {
        throw ErrorDominio("El pendiente debe ser de dinero (monto positivo) o de producto (codigo y cantidad), no ambos ni ninguno.");
    }
    if (conProducto && (codigo.empty() || cant <= 0)) {
        throw ErrorDominio("El pendiente de producto requiere codigo y una cantidad positiva.");
    }
    id = idPendiente;
    idProveedor = proveedor;
    descripcion = desc;
    monto = m;
    codigoProducto = codigo;
    cantidad = conProducto ? cant : 0;
    fecha = fec;
}

int Pendiente::getId() const { return id; }
int Pendiente::getIdProveedor() const { return idProveedor; }
SentidoPendiente Pendiente::getSentido() const { return sentido; }
std::string Pendiente::getDescripcion() const { return descripcion; }
double Pendiente::getMonto() const { return monto; }
std::string Pendiente::getCodigoProducto() const { return codigoProducto; }
int Pendiente::getCantidad() const { return cantidad; }
std::string Pendiente::getFecha() const { return fecha; }
bool Pendiente::isSaldado() const { return saldado; }
std::string Pendiente::getFechaSaldado() const { return fechaSaldado; }
std::string Pendiente::getMedioSaldado() const { return medioSaldado; }
bool Pendiente::esDinero() const { return monto > 0; }

void Pendiente::saldar(const std::string& fechaPago, const std::string& medio) {
    if (saldado) {
        throw ErrorDominio("El pendiente ya esta saldado.");
    }
    if (!Validaciones::fechaValida(fechaPago)) {
        throw ErrorDominio("La fecha de pago debe tener el formato AAAA-MM-DD y ser valida.");
    }
    std::string m = Validaciones::aMinusculas(Validaciones::recortar(medio));
    bool pagoDelLocal = (sentido == SentidoPendiente::LocalDebe && esDinero());
    if (pagoDelLocal && m != "efectivo" && m != "banco") {
        throw ErrorDominio("Para saldar una deuda del local indique el medio: 'efectivo' o 'banco'.");
    }
    saldado = true;
    fechaSaldado = fechaPago;
    medioSaldado = m;
}

bool Pendiente::superaDias(const std::string& fechaHoy, int dias) const {
    return Validaciones::diasDesdeEpoca(fechaHoy) - Validaciones::diasDesdeEpoca(fecha) > dias;
}
