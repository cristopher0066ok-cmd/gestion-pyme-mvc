#ifndef MODELO_PYME_HPP
#define MODELO_PYME_HPP

// =====================================================================
//  ModeloPYME: fachada del dominio (Modelo del MVC).
//  REGLAS PARA EDITAR ESTE ARCHIVO (evita conflictos de merge):
//   1. Cada integrante edita SOLO su seccion (entre INICIO y FIN).
//   2. Los #include van en la zona de su seccion, no en otra.
//   3. La implementacion va en SU archivo ModeloPYME_<seccion>.cpp.
//   4. Si necesitas algo de otra seccion, pidelo en el grupo.
//   5. El Modelo no hace entrada ni salida por consola. Las reglas
//      violadas lanzan ErrorDominio.
// =====================================================================

#include <string>
#include <vector>

#include "ErrorDominio.hpp"
#include "Validaciones.hpp"

// ---- INICIO INCLUDES PRODUCTOS (Mateo) ----
#include "Producto.hpp"
// ---- FIN INCLUDES PRODUCTOS ----

// ---- INICIO INCLUDES CLIENTES Y PROVEEDORES (Martin) ----
#include "Cliente.hpp"
#include "Proveedor.hpp"
#include "Pendiente.hpp"
// ---- FIN INCLUDES CLIENTES Y PROVEEDORES ----

// ---- INICIO INCLUDES VENTAS (Sebastian) ----
#include "Venta.hpp"
#include "Factura.hpp"
#include "Tiempo.hpp"
// ---- FIN INCLUDES VENTAS ----

// ---- INICIO INCLUDES PERSISTENCIA Y REPORTES (Limaico) ----
#include "Reportes.hpp"
#include "RepositorioArchivo.hpp"
// ---- FIN INCLUDES PERSISTENCIA Y REPORTES ----

class ModeloPYME {
private:
    // ---- INICIO ATRIBUTOS PRODUCTOS (Mateo) ----
    std::vector<Producto> productos;
    // ---- FIN ATRIBUTOS PRODUCTOS ----

    // ---- INICIO ATRIBUTOS CLIENTES Y PROVEEDORES (Martin) ----
    std::vector<Cliente> clientes;
    std::vector<Proveedor> proveedores;
    std::vector<Pendiente> pendientes;
    int siguienteIdCliente;
    int siguienteIdProveedor;
    int siguienteIdPendiente;
    // ---- FIN ATRIBUTOS CLIENTES Y PROVEEDORES ----

    // ---- INICIO ATRIBUTOS VENTAS (Sebastian) ----
    std::vector<Venta> ventas;
    std::vector<Factura> facturas;
    int siguienteNumeroVenta;
    // ---- FIN ATRIBUTOS VENTAS ----

    // ---- INICIO ATRIBUTOS PERSISTENCIA Y REPORTES (Limaico) ----
    RepositorioArchivo repositorio;
    ConfiguracionCierres configuracionCierres;
    // ---- FIN ATRIBUTOS PERSISTENCIA Y REPORTES ----

    // Ayudas privadas (no forman parte de la interfaz publica)
    int indiceProducto(const std::string& codigo) const;
    void verificarContactoUnico(const std::string& telefono, const std::string& correo,
                                int idPropio, bool esCliente) const;

public:
    explicit ModeloPYME(const std::string& directorioDatos = "datos");

    // ---- INICIO PRODUCTOS (Mateo) -> ModeloPYME_productos.cpp ----
    void registrarProducto(const Producto& p);
    Producto* buscarProductoPorCodigo(const std::string& codigo);
    std::vector<Producto> buscarProductos(const std::string& texto) const; // por codigo o nombre
    const std::vector<Producto>& getProductos() const;
    void editarProducto(const std::string& codigo, const std::string& nombre, double precio,
                        int stock, int stockMinimo, const DatosComerciales& datos);
    void desactivarProducto(const std::string& codigo);
    std::vector<Producto> productosConStockBajo() const;
    std::vector<Producto> productosPorCaducar(const std::string& fechaHoy, int dias) const; // RF-15
    void asignarPromocion(const std::string& codigo, double precioPromocion);               // RF-15
    // ---- FIN PRODUCTOS ----

    // ---- INICIO CLIENTES Y PROVEEDORES (Martin) -> ModeloPYME_clientes.cpp / _proveedores.cpp ----
    int registrarCliente(const std::string& nombre, const std::string& telefono, const std::string& correo);
    Cliente* buscarClientePorId(int id);
    std::vector<Cliente> buscarClientes(const std::string& texto) const;
    const std::vector<Cliente>& getClientes() const;
    void editarCliente(int id, const std::string& telefono, const std::string& correo);

    int registrarProveedor(const std::string& nombreComercial, const std::string& nombreVendedor,
                           const std::string& telefono, const std::string& correo);
    Proveedor* buscarProveedorPorId(int id);
    std::vector<Proveedor> buscarProveedores(const std::string& texto) const;
    const std::vector<Proveedor>& getProveedores() const;
    void editarProveedor(int id, const std::string& nombreVendedor,
                         const std::string& telefono, const std::string& correo);

    // RF-14: deudas y pendientes con proveedores
    int registrarPendiente(int idProveedor, SentidoPendiente sentido, const std::string& descripcion,
                           double monto, const std::string& codigoProducto, int cantidad,
                           const std::string& fecha);
    void saldarPendiente(int id, const std::string& fechaPago, const std::string& medio);
    const std::vector<Pendiente>& getPendientes() const;
    std::vector<Pendiente> pendientesAbiertos() const;
    std::vector<Pendiente> pendientesVencidos(const std::string& fechaHoy, int dias) const;
    // ---- FIN CLIENTES Y PROVEEDORES ----

    // ---- INICIO VENTAS (Sebastian) -> ModeloPYME_ventas.cpp ----
    double calcularTotalPrevio(const std::vector<LineaVenta>& lineas) const;
    Venta procesarVenta(int idCliente, const std::vector<LineaVenta>& lineas,
                        const std::string& fecha, const std::string& hora,
                        const std::string& metodoPago, const std::string& banco,
                        double montoRecibido, bool transferenciaRecibida);
    const std::vector<Venta>& getVentas() const;

    // RF-12: facturas de compra
    void registrarFactura(const std::string& numero, const std::string& claveAcceso,
                          const std::string& fecha, int idProveedor, const std::string& atendidoPor,
                          const std::string& rutaFoto, const std::vector<LineaFactura>& lineas);
    const std::vector<Factura>& getFacturas() const;
    // Filtros vacios (o idProveedor = 0) no filtran.
    std::vector<Factura> buscarFacturas(int idProveedor, const std::string& desde,
                                        const std::string& hasta, const std::string& numero,
                                        const std::string& codigoProducto) const;
    // ---- FIN VENTAS ----

    // ---- INICIO PERSISTENCIA (Limaico) -> ModeloPYME_persistencia.cpp ----
    // cargarDatos devuelve avisos (archivos faltantes o lineas omitidas); nunca lanza por datos danados.
    std::vector<std::string> cargarDatos();
    void guardarDatos();
    // ---- FIN PERSISTENCIA ----

    // ---- INICIO REPORTES (Limaico) -> ModeloPYME_reportes.cpp ----
    ResumenVentas generarResumen() const;
    CierreDiario cierreDiario(const std::string& fecha) const;                       // RF-13
    CierreMensual cierreMensual(const std::string& mes) const;                       // RF-13
    void establecerSaldoBase(const std::string& mes, double caja, double banco);     // RF-13
    const ConfiguracionCierres& getConfiguracionCierres() const;
    // ---- FIN REPORTES ----
};

#endif // MODELO_PYME_HPP
