#ifndef MODELO_PYME_HPP
#define MODELO_PYME_HPP

#include <vector>
#include <string>
#include "Producto.hpp"

class ModeloPYME {
private:
    std::vector<Producto> productos;
    // Aquí cada compañero agregará las colecciones de Clientes, Proveedores y Ventas

public:
    ModeloPYME();

    // Operaciones de Productos (Asignado a Mateo)
    void registrarProducto(const Producto& p);
    Producto* buscarProductoPorCodigo(const std::string& codigo);
    const std::vector<Producto>& getProductos() const;
    void desactivarProducto(const std::string& codigo);

    // Métodos generales de persistencia (Asignado a Persistencia)
    void cargarDatos();
    void guardarDatos();
};

#endif // MODELO_PYME_HPP