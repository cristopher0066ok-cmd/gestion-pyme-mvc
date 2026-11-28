#ifndef DETALLE_VENTA_HPP
#define DETALLE_VENTA_HPP

#include <string>

class DetalleVenta {
private:
    std::string codigoProducto;
    int cantidad;
    double precioUnitario; // precio vigente al confirmar la venta

public:
    DetalleVenta(std::string codigoProducto, int cantidad, double precioUnitario);

    std::string getCodigoProducto() const;
    int getCantidad() const;
    double getPrecioUnitario() const;
    double calcularSubtotal() const;
};

#endif // DETALLE_VENTA_HPP
