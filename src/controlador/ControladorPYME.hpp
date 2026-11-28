#ifndef CONTROLADOR_PYME_HPP
#define CONTROLADOR_PYME_HPP

#include "../modelo/ModeloPYME.hpp"
#include "../vista/VistaConsola.hpp"

// Coordina el flujo: recibe las opciones de la Vista, invoca al Modelo y pide a la Vista que
// muestre el resultado. No imprime, no lee del teclado y no repite reglas del Modelo.
class ControladorPYME {
private:
    ModeloPYME& modelo;
    VistaConsola& vista;

    void gestionarProductos();
    void gestionarClientes();
    void gestionarProveedores();
    void gestionarVentas();
    void gestionarFacturas();
    void gestionarPendientes();
    void gestionarCierres();
    void consultarReportes();

    // Flujos auxiliares
    void registrarVenta();
    void registrarFactura();
    void guardar();                                  // guarda y avisa si falla
    std::string fechaONuevaHoy(const std::string& texto) const; // vacio = fecha de hoy

public:
    ControladorPYME(ModeloPYME& mod, VistaConsola& vis);
    void ejecutar();
};

#endif // CONTROLADOR_PYME_HPP
