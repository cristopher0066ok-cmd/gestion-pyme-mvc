#ifndef VISTA_CONSOLA_HPP
#define VISTA_CONSOLA_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include "../modelo/Cliente.hpp"
#include "../modelo/Factura.hpp"
#include "../modelo/Pendiente.hpp"
#include "../modelo/Producto.hpp"
#include "../modelo/Proveedor.hpp"
#include "../modelo/Reportes.hpp"
#include "../modelo/Venta.hpp"

// Se lanza cuando la entrada de consola termina (fin de archivo / Ctrl+D): el Controlador
// guarda y sale de forma ordenada en lugar de quedar en un bucle infinito.
class FinDeEntrada : public std::runtime_error {
public:
    FinDeEntrada() : std::runtime_error("Fin de la entrada de datos.") {}
};

// La Vista solo muestra y lee. No aplica reglas de negocio (eso es del Modelo) y solo recibe
// objetos del Modelo para leer sus datos.
class VistaConsola {
private:
    int mostrarMenu(const std::string& titulo, const std::vector<std::string>& opciones) const;
    std::string leerLinea(const std::string& etiqueta) const;

public:
    VistaConsola();

    // Menus y navegacion (RF-09). Cada uno incluye la opcion 0 para volver o salir.
    int mostrarMenuPrincipal();
    int mostrarMenuProductos();
    int mostrarMenuClientes();
    int mostrarMenuProveedores();
    int mostrarMenuVentas();
    int mostrarMenuFacturas();
    int mostrarMenuPendientes();
    int mostrarMenuCierres();
    int mostrarMenuReportes();

    // Mensajes
    void mostrarMensajeExito(const std::string& mensaje) const;
    void mostrarMensajeError(const std::string& mensaje) const;
    void mostrarMensajeInfo(const std::string& mensaje) const;
    void mostrarAvisos(const std::vector<std::string>& avisos) const;
    void mostrarTotal(const std::string& etiqueta, double valor) const;

    // Listados y detalle
    void mostrarProducto(const Producto& producto) const;
    void mostrarListaProductos(const std::vector<Producto>& productos,
                               const std::string& titulo = "LISTADO DE PRODUCTOS") const;
    void mostrarListaClientes(const std::vector<Cliente>& clientes) const;
    void mostrarListaProveedores(const std::vector<Proveedor>& proveedores) const;
    void mostrarVenta(const Venta& venta) const;
    void mostrarListaVentas(const std::vector<Venta>& ventas) const;
    void mostrarFactura(const Factura& factura) const;
    void mostrarListaFacturas(const std::vector<Factura>& facturas) const;
    // 'vencidos' son los ids de pendientes que superan N dias: se resaltan con "(!)".
    void mostrarListaPendientes(const std::vector<Pendiente>& pendientes,
                                const std::vector<int>& vencidos, const std::string& titulo) const;

    // Reportes y cierres
    void mostrarResumenVentas(const ResumenVentas& resumen) const;
    void mostrarCierreDiario(const CierreDiario& cierre) const;
    void mostrarCierreMensual(const CierreMensual& cierre) const;

    // Entradas seguras por consola (solo validan el formato: texto, entero o decimal)
    std::string leerTexto(const std::string& etiqueta) const;
    double leerDecimal(const std::string& etiqueta) const;
    int leerEntero(const std::string& etiqueta) const;
    bool solicitarConfirmacion(const std::string& pregunta) const;
    void pausar() const;
};

#endif // VISTA_CONSOLA_HPP
