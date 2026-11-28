#include "RepositorioArchivo.hpp"
#include "ErrorDominio.hpp"
#include "Validaciones.hpp"
#include <cmath>
#include <iomanip>
#include <locale>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>

namespace fs = std::filesystem;

namespace {

const char SEPARADOR = ';';

std::string sanear(const std::string& texto) {
    std::string r = texto;
    for (char& c : r) {
        if (c == SEPARADOR) c = ',';
        if (c == '\n' || c == '\r') c = ' ';
    }
    return r;
}

std::string decimal(double valor) {
    std::ostringstream flujo;
    flujo.imbue(std::locale::classic());
    flujo << std::fixed << std::setprecision(2) << valor;
    return flujo.str();
}

std::string unir(const std::vector<std::string>& campos) {
    std::string linea;
    for (size_t i = 0; i < campos.size(); ++i) {
        if (i > 0) linea += SEPARADOR;
        linea += sanear(campos[i]);
    }
    return linea;
}

std::vector<std::string> dividir(const std::string& linea) {
    std::vector<std::string> campos;
    std::string actual;
    for (char c : linea) {
        if (c == SEPARADOR) {
            campos.push_back(actual);
            actual.clear();
        } else {
            actual += c;
        }
    }
    campos.push_back(actual);
    return campos;
}

int aEntero(const std::string& texto) {
    size_t usados = 0;
    int valor = 0;
    try {
        valor = std::stoi(texto, &usados);
    } catch (const std::exception&) {
        throw ErrorDominio("numero entero invalido: '" + texto + "'");
    }
    if (usados != texto.size()) {
        throw ErrorDominio("numero entero invalido: '" + texto + "'");
    }
    return valor;
}

double aDecimal(const std::string& texto) {
    size_t usados = 0;
    double valor = 0;
    try {
        valor = std::stod(texto, &usados);
    } catch (const std::exception&) {
        throw ErrorDominio("numero decimal invalido: '" + texto + "'");
    }
    if (usados != texto.size() || !std::isfinite(valor)) {
        throw ErrorDominio("numero decimal invalido: '" + texto + "'");
    }
    return valor;
}

bool aBool(const std::string& texto) {
    if (texto == "1") return true;
    if (texto == "0") return false;
    throw ErrorDominio("valor logico invalido: '" + texto + "'");
}

std::string avisoLinea(const std::string& archivo, int linea, const std::string& motivo) {
    return "Linea " + std::to_string(linea) + " de " + archivo + " omitida: " + motivo;
}

} // namespace

RepositorioArchivo::RepositorioArchivo(std::string dir) : directorio(std::move(dir)) {}

std::string RepositorioArchivo::getDirectorio() const { return directorio; }

bool RepositorioArchivo::existeArchivo(const std::string& archivo) const {
    std::error_code ec;
    return fs::exists(fs::path(directorio) / archivo, ec);
}

std::vector<std::string> RepositorioArchivo::nombresArchivos() {
    return {"productos.csv", "clientes.csv", "proveedores.csv", "ventas.csv",
            "detalles_venta.csv", "facturas.csv", "detalles_factura.csv",
            "pendientes.csv", "config.csv"};
}

std::vector<RepositorioArchivo::Fila> RepositorioArchivo::leerFilas(
        const std::string& archivo, size_t columnas, std::vector<std::string>& avisos) const {
    std::vector<Fila> filas;
    std::ifstream entrada(fs::path(directorio) / archivo);
    if (!entrada.is_open()) {
        return filas; // archivo inexistente: ModeloPYME informa que inicia vacio
    }
    std::string linea;
    int numeroLinea = 0;
    while (std::getline(entrada, linea)) {
        ++numeroLinea;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (numeroLinea == 1) continue;           // encabezado
        if (Validaciones::recortar(linea).empty()) continue;
        std::vector<std::string> campos = dividir(linea);
        if (campos.size() != columnas) {
            avisos.push_back(avisoLinea(archivo, numeroLinea,
                "se esperaban " + std::to_string(columnas) + " columnas y hay " +
                std::to_string(campos.size()) + "."));
            continue;
        }
        filas.push_back({numeroLinea, campos});
    }
    return filas;
}

void RepositorioArchivo::escribirArchivo(const std::string& archivo, const std::string& encabezado,
                                         const std::vector<std::string>& lineas) const {
    std::error_code ec;
    fs::create_directories(directorio, ec);
    if (ec) {
        throw ErrorDominio("No se pudo crear la carpeta de datos '" + directorio + "'.");
    }
    fs::path destino = fs::path(directorio) / archivo;
    fs::path temporal = fs::path(directorio) / (archivo + ".tmp");
    {
        std::ofstream salida(temporal, std::ios::trunc);
        if (!salida.is_open()) {
            throw ErrorDominio("No se pudo escribir el archivo '" + archivo + "'.");
        }
        salida << encabezado << "\n";
        for (const auto& l : lineas) salida << l << "\n";
        salida.flush();
        if (!salida) {
            throw ErrorDominio("Error al escribir el archivo '" + archivo + "'.");
        }
    }
    // Se escribe primero en un temporal y luego se renombra: un corte a la mitad no daña el archivo.
    fs::rename(temporal, destino, ec);
    if (ec) {
        fs::remove(destino, ec);
        fs::rename(temporal, destino, ec);
        if (ec) {
            throw ErrorDominio("No se pudo reemplazar el archivo '" + archivo + "'.");
        }
    }
}

// ---------------------------------------------------------------- Guardar

void RepositorioArchivo::guardarProductos(const std::vector<Producto>& productos) const {
    std::vector<std::string> lineas;
    for (const auto& p : productos) {
        lineas.push_back(unir({p.getCodigo(), p.getNombre(), decimal(p.getPrecio()),
                               std::to_string(p.getStock()), std::to_string(p.getStockMinimo()),
                               p.isActivo() ? "1" : "0", p.getCategoria(), p.getUnidadVenta(),
                               decimal(p.getPrecioCompra()), p.getFechaCaducidad(),
                               std::to_string(p.getIdProveedor()), p.getVariante(),
                               decimal(p.getPrecioPromocion())}));
    }
    escribirArchivo("productos.csv",
        "codigo;nombre;precio;stock;stockMinimo;activo;categoria;unidadVenta;precioCompra;"
        "fechaCaducidad;idProveedor;variante;precioPromocion", lineas);
}

void RepositorioArchivo::guardarClientes(const std::vector<Cliente>& clientes) const {
    std::vector<std::string> lineas;
    for (const auto& c : clientes) {
        lineas.push_back(unir({std::to_string(c.getId()), c.getNombre(), c.getTelefono(), c.getCorreo()}));
    }
    escribirArchivo("clientes.csv", "id;nombre;telefono;correo", lineas);
}

void RepositorioArchivo::guardarProveedores(const std::vector<Proveedor>& proveedores) const {
    std::vector<std::string> lineas;
    for (const auto& p : proveedores) {
        lineas.push_back(unir({std::to_string(p.getId()), p.getNombreComercial(), p.getNombreVendedor(),
                               p.getTelefono(), p.getCorreo()}));
    }
    escribirArchivo("proveedores.csv", "id;nombreComercial;nombreVendedor;telefono;correo", lineas);
}

void RepositorioArchivo::guardarVentas(const std::vector<Venta>& ventas) const {
    std::vector<std::string> cabeceras;
    std::vector<std::string> detalles;
    for (const auto& v : ventas) {
        cabeceras.push_back(unir({std::to_string(v.getNumero()), v.getFecha(), v.getHora(),
                                  std::to_string(v.getIdCliente()), v.getMetodoPago(), v.getBanco(),
                                  decimal(v.getMontoRecibido()),
                                  v.isTransferenciaRecibida() ? "1" : "0", decimal(v.getTotal())}));
        for (const auto& d : v.getDetalles()) {
            detalles.push_back(unir({std::to_string(v.getNumero()), d.getCodigoProducto(),
                                     std::to_string(d.getCantidad()), decimal(d.getPrecioUnitario())}));
        }
    }
    escribirArchivo("ventas.csv",
        "numero;fecha;hora;idCliente;metodoPago;banco;montoRecibido;transferenciaRecibida;total", cabeceras);
    escribirArchivo("detalles_venta.csv", "numeroVenta;codigoProducto;cantidad;precioUnitario", detalles);
}

void RepositorioArchivo::guardarFacturas(const std::vector<Factura>& facturas) const {
    std::vector<std::string> cabeceras;
    std::vector<std::string> detalles;
    for (const auto& f : facturas) {
        cabeceras.push_back(unir({f.getNumero(), f.getClaveAcceso(), f.getFecha(),
                                  std::to_string(f.getIdProveedor()), f.getAtendidoPor(), f.getRutaFoto()}));
        for (const auto& d : f.getDetalles()) {
            detalles.push_back(unir({std::to_string(f.getIdProveedor()), f.getNumero(),
                                     d.getCodigoProducto(), std::to_string(d.getCantidad()),
                                     decimal(d.getPrecioCompra())}));
        }
    }
    escribirArchivo("facturas.csv", "numero;claveAcceso;fecha;idProveedor;atendidoPor;rutaFoto", cabeceras);
    escribirArchivo("detalles_factura.csv",
        "idProveedor;numeroFactura;codigoProducto;cantidad;precioCompra", detalles);
}

void RepositorioArchivo::guardarPendientes(const std::vector<Pendiente>& pendientes) const {
    std::vector<std::string> lineas;
    for (const auto& p : pendientes) {
        lineas.push_back(unir({std::to_string(p.getId()), std::to_string(p.getIdProveedor()),
                               sentidoATexto(p.getSentido()), p.getDescripcion(), decimal(p.getMonto()),
                               p.getCodigoProducto(), std::to_string(p.getCantidad()), p.getFecha(),
                               p.isSaldado() ? "1" : "0", p.getFechaSaldado(), p.getMedioSaldado()}));
    }
    escribirArchivo("pendientes.csv",
        "id;idProveedor;sentido;descripcion;monto;codigoProducto;cantidad;fecha;saldado;fechaSaldado;medio",
        lineas);
}

void RepositorioArchivo::guardarConfiguracion(const ConfiguracionCierres& config) const {
    escribirArchivo("config.csv", "clave;valor",
        {unir({"mesBase", config.mesBase}),
         unir({"saldoBaseCaja", decimal(config.saldoBaseCaja)}),
         unir({"saldoBaseBanco", decimal(config.saldoBaseBanco)})});
}

// ---------------------------------------------------------------- Cargar

std::vector<Producto> RepositorioArchivo::cargarProductos(std::vector<std::string>& avisos) const {
    std::vector<Producto> productos;
    for (const auto& fila : leerFilas("productos.csv", 13, avisos)) {
        const auto& c = fila.second;
        try {
            Producto p(c[0], c[1], aDecimal(c[2]), aEntero(c[3]), aEntero(c[4]));
            DatosComerciales d;
            d.categoria = c[6];
            d.unidadVenta = c[7];
            d.precioCompra = aDecimal(c[8]);
            d.fechaCaducidad = c[9];
            d.idProveedor = aEntero(c[10]);
            d.variante = c[11];
            p.definirDatosComerciales(d);
            double promo = aDecimal(c[12]);
            if (promo > 0) p.asignarPromocion(promo);
            if (!aBool(c[5])) p.desactivar();
            productos.push_back(p);
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("productos.csv", fila.first, e.what()));
        }
    }
    return productos;
}

std::vector<Cliente> RepositorioArchivo::cargarClientes(std::vector<std::string>& avisos) const {
    std::vector<Cliente> clientes;
    for (const auto& fila : leerFilas("clientes.csv", 4, avisos)) {
        const auto& c = fila.second;
        try {
            clientes.emplace_back(aEntero(c[0]), c[1], c[2], c[3]);
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("clientes.csv", fila.first, e.what()));
        }
    }
    return clientes;
}

std::vector<Proveedor> RepositorioArchivo::cargarProveedores(std::vector<std::string>& avisos) const {
    std::vector<Proveedor> proveedores;
    for (const auto& fila : leerFilas("proveedores.csv", 5, avisos)) {
        const auto& c = fila.second;
        try {
            proveedores.emplace_back(aEntero(c[0]), c[1], c[2], c[3], c[4]);
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("proveedores.csv", fila.first, e.what()));
        }
    }
    return proveedores;
}

std::vector<Venta> RepositorioArchivo::cargarVentas(std::vector<std::string>& avisos) const {
    std::map<int, std::vector<DetalleVenta>> detallesPorVenta;
    for (const auto& fila : leerFilas("detalles_venta.csv", 4, avisos)) {
        const auto& c = fila.second;
        try {
            detallesPorVenta[aEntero(c[0])].emplace_back(c[1], aEntero(c[2]), aDecimal(c[3]));
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("detalles_venta.csv", fila.first, e.what()));
        }
    }
    std::vector<Venta> ventas;
    for (const auto& fila : leerFilas("ventas.csv", 9, avisos)) {
        const auto& c = fila.second;
        try {
            int numero = aEntero(c[0]);
            Venta v(numero, c[1], c[2], aEntero(c[3]));
            for (const auto& d : detallesPorVenta[numero]) v.agregarDetalle(d);
            v.definirPago(c[4], c[5], aDecimal(c[6]), aBool(c[7]));
            v.confirmar();
            ventas.push_back(v);
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("ventas.csv", fila.first, e.what()));
        }
    }
    return ventas;
}

std::vector<Factura> RepositorioArchivo::cargarFacturas(std::vector<std::string>& avisos) const {
    std::map<std::pair<int, std::string>, std::vector<DetalleFactura>> detallesPorFactura;
    for (const auto& fila : leerFilas("detalles_factura.csv", 5, avisos)) {
        const auto& c = fila.second;
        try {
            detallesPorFactura[{aEntero(c[0]), c[1]}].emplace_back(c[2], aEntero(c[3]), aDecimal(c[4]));
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("detalles_factura.csv", fila.first, e.what()));
        }
    }
    std::vector<Factura> facturas;
    for (const auto& fila : leerFilas("facturas.csv", 6, avisos)) {
        const auto& c = fila.second;
        try {
            int proveedor = aEntero(c[3]);
            Factura f(c[0], c[1], c[2], proveedor, c[4], c[5]);
            auto it = detallesPorFactura.find({proveedor, f.getNumero()});
            if (it == detallesPorFactura.end() || it->second.empty()) {
                throw ErrorDominio("la factura no tiene detalles");
            }
            for (const auto& d : it->second) f.agregarDetalle(d);
            facturas.push_back(f);
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("facturas.csv", fila.first, e.what()));
        }
    }
    return facturas;
}

std::vector<Pendiente> RepositorioArchivo::cargarPendientes(std::vector<std::string>& avisos) const {
    std::vector<Pendiente> pendientes;
    for (const auto& fila : leerFilas("pendientes.csv", 11, avisos)) {
        const auto& c = fila.second;
        try {
            Pendiente p(aEntero(c[0]), aEntero(c[1]), textoASentido(c[2]), c[3], aDecimal(c[4]),
                        c[5], aEntero(c[6]), c[7]);
            if (aBool(c[8])) p.saldar(c[9], c[10]);
            pendientes.push_back(p);
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("pendientes.csv", fila.first, e.what()));
        }
    }
    return pendientes;
}

ConfiguracionCierres RepositorioArchivo::cargarConfiguracion(std::vector<std::string>& avisos) const {
    ConfiguracionCierres config;
    for (const auto& fila : leerFilas("config.csv", 2, avisos)) {
        const auto& c = fila.second;
        try {
            if (c[0] == "mesBase") {
                if (!c[1].empty() && !Validaciones::anioMesValido(c[1])) {
                    throw ErrorDominio("mes base invalido: '" + c[1] + "'");
                }
                config.mesBase = c[1];
            } else if (c[0] == "saldoBaseCaja") {
                config.saldoBaseCaja = aDecimal(c[1]);
            } else if (c[0] == "saldoBaseBanco") {
                config.saldoBaseBanco = aDecimal(c[1]);
            }
        } catch (const ErrorDominio& e) {
            avisos.push_back(avisoLinea("config.csv", fila.first, e.what()));
        }
    }
    return config;
}
