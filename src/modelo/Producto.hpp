#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <string>

class Producto {
private:
    std::string codigo;
    std::string nombre;
    double precio;
    int stock;
    int stockMinimo;
    bool activo;

public:
    Producto(std::string cod, std::string nom, double prec, int stk, int stkMin);

    // Getters
    std::string getCodigo() const;
    std::string getNombre() const;
    double getPrecio() const;
    int getStock() const;
    int getStockMinimo() const;
    bool isActivo() const;

    // Métodos de negocio y validaciones
    void actualizarPrecio(double nuevoPrecio);
    void ajustarStock(int nuevoStock);
    void vender(int cantidad);
    void desactivar();
    bool requiereReposicion() const;
};

#endif // PRODUCTO_HPP