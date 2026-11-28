#ifndef PENDIENTE_HPP
#define PENDIENTE_HPP

#include <string>

// Sentido de la deuda (RF-14)
enum class SentidoPendiente {
    LocalDebe,     // el local le debe al proveedor
    ProveedorDebe  // el proveedor le debe al local (producto no entregado o dinero)
};

class Pendiente {
private:
    int id;
    int idProveedor;
    SentidoPendiente sentido;
    std::string descripcion;
    double monto;               // > 0 si es dinero; 0 si es producto
    std::string codigoProducto; // solo si es producto
    int cantidad;               // solo si es producto
    std::string fecha;          // AAAA-MM-DD
    bool saldado;
    std::string fechaSaldado;
    std::string medioSaldado;   // "efectivo" o "banco" (obligatorio al saldar dinero que debe el local)

public:
    Pendiente(int id, int idProveedor, SentidoPendiente sentido, std::string descripcion,
              double monto, std::string codigoProducto, int cantidad, std::string fecha);

    int getId() const;
    int getIdProveedor() const;
    SentidoPendiente getSentido() const;
    std::string getDescripcion() const;
    double getMonto() const;
    std::string getCodigoProducto() const;
    int getCantidad() const;
    std::string getFecha() const;
    bool isSaldado() const;
    std::string getFechaSaldado() const;
    std::string getMedioSaldado() const;
    bool esDinero() const;

    void saldar(const std::string& fechaSaldado, const std::string& medio);
    bool superaDias(const std::string& fechaHoy, int dias) const;
};

std::string sentidoATexto(SentidoPendiente sentido);
SentidoPendiente textoASentido(const std::string& texto);

#endif // PENDIENTE_HPP
