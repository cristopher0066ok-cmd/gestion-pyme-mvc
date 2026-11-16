#include <iostream>
#include "modelo/ModeloPYME.hpp"
#include "vista/VistaConsola.hpp"
#include "controlador/ControladorPYME.hpp"

int main() {
    try {
        ModeloPYME modelo;
        VistaConsola vista;
        ControladorPYME controlador(modelo, vista);

        controlador.ejecutar();
    } catch (const std::exception& e) {
        std::cerr << "Error crítico en el sistema: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}