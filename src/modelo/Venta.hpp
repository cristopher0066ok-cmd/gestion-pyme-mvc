#ifndef VENTA_HPP
#define VENTA_HPP

#include <string>
#include <vector>
#include "DetalleVenta.hpp"

// Linea que el Controlador envia al Modelo: el precio lo decide el Modelo.
struct LineaVenta {
    std::string codigoProducto;
    int cantidad;
};

class Venta {
private:
    int numero;
    std::string fecha; // AAAA-MM-DD
    std::string hora;  // HH:MM
    int idCliente;
    std::vector<DetalleVenta> detalles;
    double total;
    // RF-11
    std::string metodoPago; // "efectivo" o "transferencia"
    std::string banco;
    double montoRecibido;
    double vuelto;
    bool transferenciaRecibida;
    bool pagoDefinido;
    bool confirmada;

public:
    Venta(int numero, std::string fecha, std::string hora, int idCliente);

    int getNumero() const;
    std::string getFecha() const;
    std::string getHora() const;
    int getIdCliente() const;
    const std::vector<DetalleVenta>& getDetalles() const;
    double getTotal() const;
    std::string getMetodoPago() const;
    std::string getBanco() const;
    double getMontoRecibido() const;
    double getVuelto() const;
    bool isTransferenciaRecibida() const;
    bool isConfirmada() const;

    void agregarDetalle(const DetalleVenta& detalle);
    double calcularTotal();
    void definirPago(const std::string& metodo, const std::string& banco,
                     double montoRecibido, bool transferenciaRecibida);
    void confirmar();
};

#endif // VENTA_HPP
