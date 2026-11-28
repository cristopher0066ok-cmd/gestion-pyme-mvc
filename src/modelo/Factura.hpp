#ifndef FACTURA_HPP
#define FACTURA_HPP

#include <string>
#include <vector>

// Linea que el Controlador envia al Modelo al registrar una factura de compra.
struct LineaFactura {
    std::string codigoProducto;
    int cantidad;
    double precioCompra;
};

class DetalleFactura {
private:
    std::string codigoProducto;
    int cantidad;
    double precioCompra;

public:
    DetalleFactura(std::string codigoProducto, int cantidad, double precioCompra);

    std::string getCodigoProducto() const;
    int getCantidad() const;
    double getPrecioCompra() const;
    double calcularSubtotal() const;
};

class Factura {
private:
    std::string numero;
    std::string claveAcceso;
    std::string fecha; // AAAA-MM-DD
    int idProveedor;
    std::string atendidoPor;
    std::string rutaFoto; // solo la ruta del archivo; no se lee la imagen
    std::vector<DetalleFactura> detalles;

public:
    Factura(std::string numero, std::string claveAcceso, std::string fecha, int idProveedor,
            std::string atendidoPor, std::string rutaFoto);

    std::string getNumero() const;
    std::string getClaveAcceso() const;
    std::string getFecha() const;
    int getIdProveedor() const;
    std::string getAtendidoPor() const;
    std::string getRutaFoto() const;
    const std::vector<DetalleFactura>& getDetalles() const;
    double calcularTotal() const;
    bool incluyeProducto(const std::string& codigo) const;

    void agregarDetalle(const DetalleFactura& detalle);
};

#endif // FACTURA_HPP
