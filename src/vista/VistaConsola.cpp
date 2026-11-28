#include "VistaConsola.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

std::string dinero(double valor) {
    std::ostringstream flujo;
    flujo << "$" << std::fixed << std::setprecision(2) << valor;
    return flujo.str();
}

std::string cortar(const std::string& texto, size_t ancho) {
    if (texto.size() <= ancho) return texto;
    return texto.substr(0, ancho - 1) + ".";
}

std::string guion(const std::string& texto) {
    return texto.empty() ? "-" : texto;
}

void linea(char c = '-', int n = 72) {
    std::cout << std::string(static_cast<size_t>(n), c) << "\n";
}

} // namespace

VistaConsola::VistaConsola() {}

// ---------------------------------------------------------------- Entrada

std::string VistaConsola::leerLinea(const std::string& etiqueta) const {
    std::cout << etiqueta << ": " << std::flush;
    std::string texto;
    if (!std::getline(std::cin, texto)) {
        throw FinDeEntrada();
    }
    if (!texto.empty() && texto.back() == '\r') texto.pop_back();
    return texto;
}

std::string VistaConsola::leerTexto(const std::string& etiqueta) const {
    return leerLinea(etiqueta);
}

int VistaConsola::leerEntero(const std::string& etiqueta) const {
    while (true) {
        std::string texto = leerLinea(etiqueta);
        try {
            size_t usados = 0;
            int valor = std::stoi(texto, &usados);
            while (usados < texto.size() && texto[usados] == ' ') ++usados;
            if (usados == texto.size()) return valor;
        } catch (const std::exception&) {
        }
        mostrarMensajeError("Entrada invalida. Por favor ingrese un numero entero.");
    }
}

double VistaConsola::leerDecimal(const std::string& etiqueta) const {
    while (true) {
        std::string texto = leerLinea(etiqueta);
        std::replace(texto.begin(), texto.end(), ',', '.'); // se acepta coma decimal
        try {
            size_t usados = 0;
            double valor = std::stod(texto, &usados);
            while (usados < texto.size() && texto[usados] == ' ') ++usados;
            if (usados == texto.size() && std::isfinite(valor)) return valor;
        } catch (const std::exception&) {
        }
        mostrarMensajeError("Entrada invalida. Por favor ingrese un numero (ejemplo: 12.50).");
    }
}

bool VistaConsola::solicitarConfirmacion(const std::string& pregunta) const {
    std::string r = leerLinea(pregunta + " (s/n)");
    return !r.empty() && (r[0] == 's' || r[0] == 'S');
}

void VistaConsola::pausar() const {
    leerLinea("\nPresione Enter para continuar");
}

// ---------------------------------------------------------------- Menus

int VistaConsola::mostrarMenu(const std::string& titulo, const std::vector<std::string>& opciones) const {
    std::cout << "\n";
    linea('=');
    std::cout << "  " << titulo << "\n";
    linea('=');
    for (size_t i = 0; i < opciones.size(); ++i) {
        std::cout << " " << (i + 1) << ". " << opciones[i] << "\n";
    }
    std::cout << " 0. " << "Volver / Salir" << "\n";
    linea();
    return leerEntero("Seleccione una opcion");
}

int VistaConsola::mostrarMenuPrincipal() {
    return mostrarMenu("SISTEMA DE GESTION DE PYMES - Chip Chop Pet Shop",
        {"Gestion de productos", "Gestion de clientes", "Gestion de proveedores",
         "Ventas", "Compras: facturas de proveedores", "Deudas y pendientes con proveedores",
         "Cierres de caja", "Consultas y reportes"});
}

int VistaConsola::mostrarMenuProductos() {
    return mostrarMenu("GESTION DE PRODUCTOS",
        {"Registrar producto", "Listar productos", "Buscar por codigo o nombre",
         "Editar producto", "Desactivar producto", "Productos por caducar",
         "Asignar precio de promocion"});
}

int VistaConsola::mostrarMenuClientes() {
    return mostrarMenu("GESTION DE CLIENTES",
        {"Registrar cliente", "Listar clientes", "Buscar clientes", "Editar contacto"});
}

int VistaConsola::mostrarMenuProveedores() {
    return mostrarMenu("GESTION DE PROVEEDORES",
        {"Registrar proveedor", "Listar proveedores", "Buscar proveedores", "Editar proveedor"});
}

int VistaConsola::mostrarMenuVentas() {
    return mostrarMenu("VENTAS", {"Registrar nueva venta", "Listar ventas registradas"});
}

int VistaConsola::mostrarMenuFacturas() {
    return mostrarMenu("FACTURAS DE COMPRA",
        {"Registrar factura de proveedor", "Buscar facturas", "Listar todas las facturas"});
}

int VistaConsola::mostrarMenuPendientes() {
    return mostrarMenu("DEUDAS Y PENDIENTES CON PROVEEDORES",
        {"Registrar pendiente", "Listar pendientes abiertos", "Marcar pendiente como saldado"});
}

int VistaConsola::mostrarMenuCierres() {
    return mostrarMenu("CIERRES DE CAJA",
        {"Cierre diario", "Cierre mensual", "Definir saldo base (mes anterior al primer cierre)"});
}

int VistaConsola::mostrarMenuReportes() {
    return mostrarMenu("CONSULTAS Y REPORTES",
        {"Inventario disponible", "Productos con stock bajo", "Resumen de ventas"});
}

// ---------------------------------------------------------------- Mensajes

void VistaConsola::mostrarMensajeExito(const std::string& mensaje) const {
    std::cout << "\n[EXITO] " << mensaje << "\n";
}

void VistaConsola::mostrarMensajeError(const std::string& mensaje) const {
    std::cout << "\n[ERROR] " << mensaje << "\n";
}

void VistaConsola::mostrarMensajeInfo(const std::string& mensaje) const {
    std::cout << mensaje << "\n";
}

void VistaConsola::mostrarAvisos(const std::vector<std::string>& avisos) const {
    for (const auto& a : avisos) {
        std::cout << "[AVISO] " << a << "\n";
    }
}

void VistaConsola::mostrarTotal(const std::string& etiqueta, double valor) const {
    std::cout << etiqueta << ": " << dinero(valor) << "\n";
}

// ---------------------------------------------------------------- Productos

void VistaConsola::mostrarProducto(const Producto& p) const {
    std::cout << "\n[PRODUCTO]\n"
              << "  Codigo:           " << p.getCodigo() << "\n"
              << "  Nombre:           " << p.getNombre() << "\n"
              << "  Precio:           " << dinero(p.getPrecio());
    if (p.tienePromocion()) std::cout << "  (promocion: " << dinero(p.getPrecioPromocion()) << ")";
    std::cout << "\n"
              << "  Stock / minimo:   " << p.getStock() << " / " << p.getStockMinimo()
              << (p.requiereReposicion() ? "  (REPONER)" : "") << "\n"
              << "  Estado:           " << (p.isActivo() ? "Activo" : "Inactivo") << "\n"
              << "  Categoria:        " << guion(p.getCategoria()) << "\n"
              << "  Unidad de venta:  " << p.getUnidadVenta() << "\n"
              << "  Precio de compra: " << dinero(p.getPrecioCompra()) << "\n"
              << "  Caducidad:        " << guion(p.getFechaCaducidad()) << "\n"
              << "  Proveedor (ID):   " << (p.getIdProveedor() > 0 ? std::to_string(p.getIdProveedor()) : "-") << "\n"
              << "  Variante:         " << guion(p.getVariante()) << "\n";
}

void VistaConsola::mostrarListaProductos(const std::vector<Producto>& productos, const std::string& titulo) const {
    std::cout << "\n--- " << titulo << " ---\n";
    if (productos.empty()) {
        std::cout << "No hay productos para mostrar.\n";
        return;
    }
    std::cout << std::left << std::setw(7) << "Cod." << std::setw(28) << "Nombre" << std::right
              << std::setw(9) << "Precio" << std::setw(7) << "Stock" << std::setw(5) << "Min"
              << "  " << std::left << std::setw(10) << "Estado" << std::setw(11) << "Caduca"
              << "Categoria\n";
    linea();
    for (const auto& p : productos) {
        std::cout << std::left << std::setw(7) << cortar(p.getCodigo(), 7)
                  << std::setw(28) << cortar(p.getNombre(), 27) << std::right
                  << std::setw(9) << dinero(p.getPrecioVigente())
                  << std::setw(7) << p.getStock() << std::setw(5) << p.getStockMinimo()
                  << "  " << std::left << std::setw(10) << (p.isActivo() ? "Activo" : "Inactivo")
                  << std::setw(11) << guion(p.getFechaCaducidad())
                  << guion(p.getCategoria()) << (p.tienePromocion() ? " (promo)" : "") << "\n";
    }
    std::cout << "Total: " << productos.size() << " producto(s).\n";
}

// ---------------------------------------------------------------- Clientes / proveedores

void VistaConsola::mostrarListaClientes(const std::vector<Cliente>& clientes) const {
    std::cout << "\n--- CLIENTES ---\n";
    if (clientes.empty()) {
        std::cout << "No hay clientes para mostrar.\n";
        return;
    }
    for (const auto& c : clientes) std::cout << c.datosResumen() << "\n";
    std::cout << "Total: " << clientes.size() << " cliente(s).\n";
}

void VistaConsola::mostrarListaProveedores(const std::vector<Proveedor>& proveedores) const {
    std::cout << "\n--- PROVEEDORES ---\n";
    if (proveedores.empty()) {
        std::cout << "No hay proveedores para mostrar.\n";
        return;
    }
    for (const auto& p : proveedores) std::cout << p.datosResumen() << "\n";
    std::cout << "Total: " << proveedores.size() << " proveedor(es).\n";
}

// ---------------------------------------------------------------- Ventas

void VistaConsola::mostrarVenta(const Venta& v) const {
    std::cout << "\n--- VENTA No. " << v.getNumero() << " ---\n"
              << "Fecha: " << v.getFecha() << "   Hora: " << v.getHora()
              << "   Cliente (ID): " << v.getIdCliente() << "\n";
    std::cout << std::left << std::setw(8) << "Cod." << std::right << std::setw(6) << "Cant."
              << std::setw(12) << "P. unit." << std::setw(12) << "Subtotal" << "\n";
    for (const auto& d : v.getDetalles()) {
        std::cout << std::left << std::setw(8) << cortar(d.getCodigoProducto(), 8) << std::right
                  << std::setw(6) << d.getCantidad() << std::setw(12) << dinero(d.getPrecioUnitario())
                  << std::setw(12) << dinero(d.calcularSubtotal()) << "\n";
    }
    std::cout << "TOTAL: " << dinero(v.getTotal()) << "\n"
              << "Pago: " << v.getMetodoPago();
    if (v.getMetodoPago() == "transferencia") {
        std::cout << " (" << v.getBanco() << ", recibida)";
    } else {
        std::cout << "  Recibido: " << dinero(v.getMontoRecibido()) << "  Vuelto: " << dinero(v.getVuelto());
    }
    std::cout << "\n";
}

void VistaConsola::mostrarListaVentas(const std::vector<Venta>& ventas) const {
    std::cout << "\n--- VENTAS REGISTRADAS ---\n";
    if (ventas.empty()) {
        std::cout << "No hay ventas registradas.\n";
        return;
    }
    std::cout << std::left << std::setw(6) << "No." << std::setw(12) << "Fecha" << std::setw(7) << "Hora"
              << std::setw(9) << "Cliente" << std::setw(28) << "Pago" << std::right << std::setw(10) << "Total" << "\n";
    linea();
    for (const auto& v : ventas) {
        std::string pago = v.getMetodoPago() + (v.getBanco().empty() ? "" : " / " + v.getBanco());
        std::cout << std::left << std::setw(6) << v.getNumero() << std::setw(12) << v.getFecha()
                  << std::setw(7) << v.getHora() << std::setw(9) << v.getIdCliente()
                  << std::setw(28) << cortar(pago, 27) << std::right << std::setw(10) << dinero(v.getTotal()) << "\n";
    }
}

// ---------------------------------------------------------------- Facturas

void VistaConsola::mostrarFactura(const Factura& f) const {
    std::cout << "\n--- FACTURA " << f.getNumero() << " ---\n"
              << "Fecha: " << f.getFecha() << "   Proveedor (ID): " << f.getIdProveedor()
              << "   Atendio: " << guion(f.getAtendidoPor()) << "\n"
              << "Clave de acceso: " << guion(f.getClaveAcceso()) << "\n"
              << "Foto (ruta): " << guion(f.getRutaFoto()) << "\n";
    for (const auto& d : f.getDetalles()) {
        std::cout << "  " << std::left << std::setw(8) << cortar(d.getCodigoProducto(), 8) << std::right
                  << std::setw(5) << d.getCantidad() << " x " << std::setw(9) << dinero(d.getPrecioCompra())
                  << " = " << std::setw(10) << dinero(d.calcularSubtotal()) << "\n";
    }
    std::cout << "TOTAL: " << dinero(f.calcularTotal()) << "\n";
}

void VistaConsola::mostrarListaFacturas(const std::vector<Factura>& facturas) const {
    std::cout << "\n--- FACTURAS DE COMPRA ---\n";
    if (facturas.empty()) {
        std::cout << "No hay facturas que coincidan.\n";
        return;
    }
    for (const auto& f : facturas) mostrarFactura(f);
    std::cout << "\nTotal: " << facturas.size() << " factura(s).\n";
}

// ---------------------------------------------------------------- Pendientes

void VistaConsola::mostrarListaPendientes(const std::vector<Pendiente>& pendientes,
                                          const std::vector<int>& vencidos, const std::string& titulo) const {
    std::cout << "\n--- " << titulo << " ---\n";
    if (pendientes.empty()) {
        std::cout << "No hay pendientes para mostrar.\n";
        return;
    }
    for (const auto& p : pendientes) {
        bool resaltado = std::find(vencidos.begin(), vencidos.end(), p.getId()) != vencidos.end();
        std::cout << (resaltado ? "(!) " : "    ") << "No. " << p.getId() << " | Proveedor " << p.getIdProveedor()
                  << " | " << (p.getSentido() == SentidoPendiente::LocalDebe ? "Le debemos" : "Nos debe")
                  << " | " << p.getDescripcion() << " | ";
        if (p.esDinero()) std::cout << dinero(p.getMonto());
        else std::cout << p.getCantidad() << " x " << p.getCodigoProducto();
        std::cout << " | Desde " << p.getFecha();
        if (p.isSaldado()) std::cout << " | Saldado " << p.getFechaSaldado();
        std::cout << "\n";
    }
    if (!vencidos.empty()) {
        std::cout << "(!) = supera el plazo de recordatorio: conviene reclamarlo.\n";
    }
}

// ---------------------------------------------------------------- Reportes y cierres

void VistaConsola::mostrarResumenVentas(const ResumenVentas& r) const {
    std::cout << "\n--- RESUMEN DE VENTAS ---\n"
              << "Ventas registradas: " << r.cantidadVentas << "\n"
              << "Monto total:        " << dinero(r.montoTotal) << "\n";
}

void VistaConsola::mostrarCierreDiario(const CierreDiario& c) const {
    std::cout << "\n--- CIERRE DIARIO " << c.fecha << " ---\n"
              << "Ventas del dia:  " << c.cantidadVentas << "\n"
              << "Efectivo:        " << dinero(c.efectivo) << "\n"
              << "Transferencias:  " << dinero(c.transferencia) << "\n";
    for (const auto& par : c.porBanco) {
        std::cout << "   " << std::left << std::setw(16) << par.first << std::right << dinero(par.second) << "\n";
    }
    std::cout << "TOTAL VENDIDO:   " << dinero(c.totalVendido) << "\n";
}

void VistaConsola::mostrarCierreMensual(const CierreMensual& c) const {
    std::cout << "\n--- CIERRE MENSUAL " << c.mes << " ---\n"
              << std::left << std::setw(22) << "" << std::right << std::setw(12) << "Caja" << std::setw(12) << "Banco" << "\n"
              << std::left << std::setw(22) << "Saldo inicial" << std::right << std::setw(12) << dinero(c.saldoInicialCaja)
              << std::setw(12) << dinero(c.saldoInicialBanco) << "\n"
              << std::left << std::setw(22) << "(+) Ventas" << std::right << std::setw(12) << dinero(c.ventasEfectivo)
              << std::setw(12) << dinero(c.ventasBanco) << "\n"
              << std::left << std::setw(22) << "(-) Pagos a proveedores" << std::right << std::setw(12) << dinero(c.pagosEfectivo)
              << std::setw(12) << dinero(c.pagosBanco) << "\n"
              << std::left << std::setw(22) << "SALDO FINAL" << std::right << std::setw(12) << dinero(c.saldoFinalCaja)
              << std::setw(12) << dinero(c.saldoFinalBanco) << "\n"
              << "Acumulado (caja + banco): " << dinero(c.acumulado) << "\n";
}
