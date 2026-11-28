// Pruebas del Modelo: persistencia en CSV (RF-07) y datos de demostracion (RF-10 del enunciado base).
#include "mini_test.hpp"
#include "modelo/ModeloPYME.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

static void limpiar(const std::string& dir) {
    std::error_code ec;
    fs::remove_all(dir, ec);
}

static void reemplazar(const std::string& archivo, const std::string& contenido) {
    std::ofstream f(archivo, std::ios::trunc);
    f << contenido;
}

int main() {
    const std::string dir = "build/datos_prueba_persistencia";

    mini::seccion("Guardar, cerrar y volver a abrir: los datos se recuperan");
    {
        limpiar(dir);
        {
            ModeloPYME m(dir);
            m.registrarCliente("Ana Perez", "0990000001", "ana@example.com");
            m.registrarProveedor("Distribuidora", "Carlos", "0981111111", "");
            DatosComerciales d;
            d.categoria = "seco"; d.unidadVenta = "bulto"; d.precioCompra = 3.2;
            d.fechaCaducidad = "2027-01-31"; d.idProveedor = 1; d.variante = "pollo; con arroz"; // ';' se sanea
            Producto a("A", "Croquetas", 10.0, 10, 2); a.definirDatosComerciales(d);
            m.registrarProducto(a);
            m.registrarProducto(Producto("B", "Juguete", 2.5, 4, 1));
            m.asignarPromocion("A", 8.0);
            m.desactivarProducto("B");
            m.procesarVenta(1, {{"A", 3}}, "2026-11-28", "10:30", "transferencia", "Pichincha", 0, true);
            m.registrarFactura("F1", "CLAVE", "2026-11-20", 1, "Carlos", "fotos/f1.jpg", {{"A", 2, 6.0}});
            int pid = m.registrarPendiente(1, SentidoPendiente::LocalDebe, "Deuda", 50.0, "", 0, "2026-11-01");
            m.saldarPendiente(pid, "2026-11-10", "efectivo");
            m.registrarPendiente(1, SentidoPendiente::ProveedorDebe, "Falto producto", 0, "A", 4, "2026-11-12");
            m.establecerSaldoBase("2026-10", 450.00, 300.00);
            m.guardarDatos();
        }
        ModeloPYME r(dir);
        std::vector<std::string> avisos = r.cargarDatos();
        COMPROBAR(avisos.empty());
        COMPROBAR(r.getClientes().size() == 1 && r.getClientes()[0].getCorreo() == "ana@example.com");
        COMPROBAR(r.getProveedores().size() == 1 && r.getProveedores()[0].getNombreVendedor() == "Carlos");
        COMPROBAR(r.getProductos().size() == 2);
        const Producto* a = r.buscarProductoPorCodigo("A");
        COMPROBAR(a != nullptr && a->getStock() == 9 && a->getCategoria() == "seco");
        COMPROBAR(a != nullptr && a->getUnidadVenta() == "bulto" && CERCA(a->getPrecioCompra(), 6.0));
        COMPROBAR(a != nullptr && a->getFechaCaducidad() == "2027-01-31" && a->getIdProveedor() == 1);
        COMPROBAR(a != nullptr && a->getVariante() == "pollo, con arroz");
        COMPROBAR(a != nullptr && CERCA(a->getPrecioPromocion(), 8.0));
        COMPROBAR(!r.buscarProductoPorCodigo("B")->isActivo());
        COMPROBAR(r.getVentas().size() == 1);
        COMPROBAR(r.getVentas()[0].getBanco() == "Pichincha" && CERCA(r.getVentas()[0].getTotal(), 24.0));
        COMPROBAR(r.getVentas()[0].getHora() == "10:30" && r.getVentas()[0].getDetalles().size() == 1);
        COMPROBAR(r.getFacturas().size() == 1 && r.getFacturas()[0].getDetalles().size() == 1);
        COMPROBAR(r.getPendientes().size() == 2 && r.getPendientes()[0].isSaldado());
        COMPROBAR(r.getConfiguracionCierres().mesBase == "2026-10");
        COMPROBAR(CERCA(r.getConfiguracionCierres().saldoBaseCaja, 450.00));
        // Los contadores continuan despues de lo guardado: los ids no se repiten
        COMPROBAR(r.registrarCliente("Nuevo", "0990000002", "") == 2);
        COMPROBAR(r.registrarProveedor("Otro", "", "0981111112", "") == 2);
        COMPROBAR(r.registrarPendiente(1, SentidoPendiente::LocalDebe, "x", 5.0, "", 0, "2026-11-15") == 3);
        COMPROBAR(r.procesarVenta(1, {{"A", 1}}, "2026-11-29", "09:00", "efectivo", "", 10.0, false).getNumero() == 2);
    }
    mini::seccion("Archivos inexistentes: avisa y el sistema sigue funcionando");
    {
        limpiar(dir);
        ModeloPYME m(dir);
        std::vector<std::string> avisos = m.cargarDatos();
        COMPROBAR(avisos.size() == 1);
        COMPROBAR(m.getProductos().empty() && m.getClientes().empty());
        COMPROBAR_NO_ERROR(m.registrarCliente("Ana", "0990000001", ""));
        // faltan solo algunos archivos
        m.guardarDatos();
        fs::remove(fs::path(dir) / "ventas.csv");
        ModeloPYME m2(dir);
        COMPROBAR(m2.cargarDatos().size() == 1 && m2.getClientes().size() == 1);
    }
    mini::seccion("Archivo invalido: se omiten las lineas danadas y se conserva el resto");
    {
        limpiar(dir);
        fs::create_directories(dir);
        reemplazar(dir + "/productos.csv",
            "codigo;nombre;precio;stock;stockMinimo;activo;categoria;unidadVenta;precioCompra;fechaCaducidad;idProveedor;variante;precioPromocion\n"
            "A;Bueno;5.00;3;1;1;seco;unidad;2.00;;0;;0.00\n"
            "B;Precio malo;abc;3;1;1;seco;unidad;2.00;;0;;0.00\n"
            "C;Stock negativo;5.00;-3;1;1;seco;unidad;2.00;;0;;0.00\n"
            "D;Pocas columnas;5.00\n"
            "E;Fecha mala;5.00;3;1;1;seco;unidad;2.00;2026-02-30;0;;0.00\n"
            "F;Otro bueno;1.00;1;1;1;;unidad;0.00;;0;;0.00\n");
        reemplazar(dir + "/clientes.csv", "id;nombre;telefono;correo\n1;Ana;0990000001;\nbasura sin formato\n");
        ModeloPYME m(dir);
        std::vector<std::string> avisos = m.cargarDatos();
        COMPROBAR(m.getProductos().size() == 2);
        COMPROBAR(m.buscarProductoPorCodigo("A") != nullptr && m.buscarProductoPorCodigo("F") != nullptr);
        COMPROBAR(m.getClientes().size() == 1);
        COMPROBAR(avisos.size() >= 5);          // 4 productos danados + 1 cliente + archivos faltantes
        COMPROBAR_NO_ERROR(m.registrarProducto(Producto("Z", "Nuevo", 1.0, 1, 1)));
    }
    mini::seccion("Datos de demostracion se cargan sin avisos");
    {
        ModeloPYME m("datos/demo");
        std::vector<std::string> avisos = m.cargarDatos();
        COMPROBAR(avisos.empty());
        COMPROBAR(m.getProductos().size() >= 8 && m.getClientes().size() >= 3 && m.getProveedores().size() >= 2);
        COMPROBAR(m.getVentas().size() >= 2 && !m.getFacturas().empty() && !m.getPendientes().empty());
        COMPROBAR(!m.productosConStockBajo().empty());
        COMPROBAR(!m.productosPorCaducar("2026-11-28", 30).empty());
        // Cada venta guardada se reconcilia con su total
        for (const auto& v : m.getVentas()) {
            double suma = 0;
            for (const auto& d : v.getDetalles()) suma += d.calcularSubtotal();
            COMPROBAR(CERCA(suma, v.getTotal()));
        }
    }
    limpiar(dir);
    return mini::resumen();
}
