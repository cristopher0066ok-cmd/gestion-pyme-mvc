#ifndef REPOSITORIO_ARCHIVO_HPP
#define REPOSITORIO_ARCHIVO_HPP

#include <string>
#include <utility>
#include <vector>
#include "Cliente.hpp"
#include "Factura.hpp"
#include "Pendiente.hpp"
#include "Producto.hpp"
#include "Proveedor.hpp"
#include "Venta.hpp"

// Configuracion que se guarda junto con los datos (saldo base de los cierres mensuales).
struct ConfiguracionCierres {
    std::string mesBase;       // "" si no esta definido
    double saldoBaseCaja = 0.0;
    double saldoBaseBanco = 0.0;
};

// Guarda y carga cada coleccion en archivos CSV (separador ';') dentro de un directorio.
// No conoce a ModeloPYME (no hay dependencia circular) y no imprime nada: las lineas
// danadas se informan en la lista de avisos que recibe cada metodo de carga.
class RepositorioArchivo {
private:
    std::string directorio;

    using Fila = std::pair<int, std::vector<std::string>>; // numero de linea y columnas
    std::vector<Fila> leerFilas(const std::string& archivo, size_t columnas,
                                std::vector<std::string>& avisos) const;
    void escribirArchivo(const std::string& archivo, const std::string& encabezado,
                         const std::vector<std::string>& lineas) const;

public:
    explicit RepositorioArchivo(std::string directorio = "datos");

    std::string getDirectorio() const;
    bool existeArchivo(const std::string& archivo) const;
    static std::vector<std::string> nombresArchivos();

    void guardarProductos(const std::vector<Producto>& productos) const;
    void guardarClientes(const std::vector<Cliente>& clientes) const;
    void guardarProveedores(const std::vector<Proveedor>& proveedores) const;
    void guardarVentas(const std::vector<Venta>& ventas) const;
    void guardarFacturas(const std::vector<Factura>& facturas) const;
    void guardarPendientes(const std::vector<Pendiente>& pendientes) const;
    void guardarConfiguracion(const ConfiguracionCierres& config) const;

    std::vector<Producto> cargarProductos(std::vector<std::string>& avisos) const;
    std::vector<Cliente> cargarClientes(std::vector<std::string>& avisos) const;
    std::vector<Proveedor> cargarProveedores(std::vector<std::string>& avisos) const;
    std::vector<Venta> cargarVentas(std::vector<std::string>& avisos) const;
    std::vector<Factura> cargarFacturas(std::vector<std::string>& avisos) const;
    std::vector<Pendiente> cargarPendientes(std::vector<std::string>& avisos) const;
    ConfiguracionCierres cargarConfiguracion(std::vector<std::string>& avisos) const;
};

#endif // REPOSITORIO_ARCHIVO_HPP
