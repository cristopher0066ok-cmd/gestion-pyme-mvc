#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <string>

// Datos comerciales ampliados del producto (RF-10).
struct DatosComerciales {
    std::string categoria;                  // texto libre: humedo, seco, accesorio, golosina...
    std::string unidadVenta = "unidad";     // solo "unidad" o "bulto"
    double precioCompra = 0.0;              // no negativo
    std::string fechaCaducidad;             // vacio o AAAA-MM-DD
    int idProveedor = 0;                    // 0 = sin proveedor
    std::string variante;                   // opcional: sabor, color o aroma
};

class Producto {
private:
    std::string codigo;
    std::string nombre;
    double precio;
    int stock;
    int stockMinimo;
    bool activo;
    // RF-10
    std::string categoria;
    std::string unidadVenta;
    double precioCompra;
    std::string fechaCaducidad;
    int idProveedor;
    std::string variante;
    // RF-15: 0 significa que no hay promocion
    double precioPromocion;

public:
    Producto(std::string cod, std::string nom, double prec, int stk, int stkMin);

    // Getters
    std::string getCodigo() const;
    std::string getNombre() const;
    double getPrecio() const;
    int getStock() const;
    int getStockMinimo() const;
    bool isActivo() const;
    std::string getCategoria() const;
    std::string getUnidadVenta() const;
    double getPrecioCompra() const;
    std::string getFechaCaducidad() const;
    int getIdProveedor() const;
    std::string getVariante() const;
    double getPrecioPromocion() const;
    bool tienePromocion() const;
    double getPrecioVigente() const; // promocion si existe; si no, el precio normal

    // Metodos de negocio y validaciones
    void actualizarPrecio(double nuevoPrecio);
    void ajustarStock(int nuevoStock);
    void vender(int cantidad);
    void desactivar();
    bool requiereReposicion() const;

    // RF-10
    void definirDatosComerciales(const DatosComerciales& datos);
    // RF-12
    void agregarStock(int cantidad);
    void actualizarPrecioCompra(double nuevoPrecioCompra);
    // RF-15
    void asignarPromocion(double precioPromocion);
    void quitarPromocion();
};

#endif // PRODUCTO_HPP
