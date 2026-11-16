#ifndef CONTROLADOR_PYME_HPP
#define CONTROLADOR_PYME_HPP

#include "../modelo/ModeloPYME.hpp"
#include "../vista/VistaConsola.hpp"

class ControladorPYME {
private:
    ModeloPYME& modelo;
    VistaConsola& vista;

    void gestionarProductos();

public:
    ControladorPYME(ModeloPYME& mod, VistaConsola& vis);
    void ejecutar();
};

#endif // CONTROLADOR_PYME_HPP