#ifndef VISTA_CONSOLA_HPP
#define VISTA_CONSOLA_HPP

#include <string>
#include "../modelo/Producto.hpp"
#include <vector>

class VistaConsola {
public:
    VistaConsola();

    // Menús principales y navegación (RF-09)
    int mostrarMenuPrincipal();
    int mostrarMenuProductos();

    // Mensajes y alertas
    void mostrarMensajeExito(const std::string& mensaje) const;
    void mostrarMensajeError(const std::string& mensaje) const;

    // Listados
    void mostrarListaProductos(const std::vector<Producto>& productos) const;

    // Entradas seguras por consola
    std::string leerTexto(const std::string& etiqueta) const;
    double leerDecimal(const std::string& etiqueta) const;
    int leerEntero(const std::string& etiqueta) const;
    void pausar() const;
};

#endif // VISTA_CONSOLA_HPP