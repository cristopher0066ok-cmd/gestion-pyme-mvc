#ifndef MODELO_PYME_HPP
#define MODELO_PYME_HPP

// =====================================================================
//  ModeloPYME: fachada del dominio (Modelo del MVC).
//  REGLAS PARA EDITAR ESTE ARCHIVO (evita conflictos de merge):
//   1. Cada integrante edita SOLO su seccion (entre INICIO y FIN).
//   2. Los #include van en la zona de su seccion, no en otra.
//   3. La implementacion va en SU archivo ModeloPYME_<seccion>.cpp.
//   4. Si necesitas algo de otra seccion, pidelo en el grupo.
//   5. El Modelo no hace entrada ni salida por consola. Las reglas violadas lanzan ErrorDominio.
// =====================================================================

#include <string>
#include <vector>

#include "ErrorDominio.hpp"

// ---- INICIO INCLUDES PRODUCTOS (Mateo) ----
#include "Producto.hpp"
// ---- FIN INCLUDES PRODUCTOS ----

// ---- INICIO INCLUDES CLIENTES Y PROVEEDORES (Martin) ----
// ---- FIN INCLUDES CLIENTES Y PROVEEDORES ----

// ---- INICIO INCLUDES VENTAS (Sebastian) ----
// ---- FIN INCLUDES VENTAS ----

// ---- INICIO INCLUDES PERSISTENCIA Y REPORTES (Limaico) ----
// ---- FIN INCLUDES PERSISTENCIA Y REPORTES ----

class ModeloPYME {
private:
    // ---- INICIO ATRIBUTOS PRODUCTOS (Mateo) ----
    std::vector<Producto> productos;
    // ---- FIN ATRIBUTOS PRODUCTOS ----

    // ---- INICIO ATRIBUTOS CLIENTES Y PROVEEDORES (Martin) ----
    // Ejemplo: std::vector<Cliente> clientes; int siguienteIdCliente;
    // ---- FIN ATRIBUTOS CLIENTES Y PROVEEDORES ----

    // ---- INICIO ATRIBUTOS VENTAS (Sebastian) ----
    // Ejemplo: std::vector<Venta> ventas; int siguienteNumeroVenta;
    // ---- FIN ATRIBUTOS VENTAS ----

public:
    ModeloPYME();

    // ---- INICIO PRODUCTOS (Mateo) -> ModeloPYME_productos.cpp ----
    void registrarProducto(const Producto& p);
    Producto* buscarProductoPorCodigo(const std::string& codigo);
    const std::vector<Producto>& getProductos() const;
    void desactivarProducto(const std::string& codigo);
    // ---- FIN PRODUCTOS ----

    // ---- INICIO CLIENTES Y PROVEEDORES (Martin) -> ModeloPYME_clientes.cpp / _proveedores.cpp ----
    // ---- FIN CLIENTES Y PROVEEDORES ----

    // ---- INICIO VENTAS (Sebastian) -> ModeloPYME_ventas.cpp ----
    // ---- FIN VENTAS ----

    // ---- INICIO PERSISTENCIA (Limaico) -> ModeloPYME_persistencia.cpp ----
    void cargarDatos();
    void guardarDatos();
    // ---- FIN PERSISTENCIA ----

    // ---- INICIO REPORTES (Limaico) -> ModeloPYME_reportes.cpp ----
    // ---- FIN REPORTES ----
};

#endif // MODELO_PYME_HPP
