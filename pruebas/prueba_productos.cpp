// Pruebas del Modelo: productos (RF-01, RF-05, RF-06, RF-08, RF-10 ampliado, RF-15).
#include "mini_test.hpp"
#include "modelo/ModeloPYME.hpp"

static ModeloPYME nuevoModelo() { return ModeloPYME("build/datos_prueba_productos"); }

int main() {
    mini::seccion("Producto valido aparece en la lista y se encuentra por codigo y nombre");
    {
        ModeloPYME m = nuevoModelo();
        m.registrarProducto(Producto("P001", "Croquetas adulto 1kg", 5.50, 20, 5));
        COMPROBAR(m.getProductos().size() == 1);
        COMPROBAR(m.buscarProductoPorCodigo("P001") != nullptr);
        COMPROBAR(m.buscarProductos("croquetas").size() == 1);   // por nombre, sin mayusculas
        COMPROBAR(m.buscarProductos("p001").size() == 1);        // por codigo
        COMPROBAR(m.buscarProductoPorCodigo("NOEXISTE") == nullptr);
    }
    mini::seccion("Codigo duplicado se rechaza y el inventario no cambia");
    {
        ModeloPYME m = nuevoModelo();
        m.registrarProducto(Producto("P001", "A", 1.0, 10, 1));
        COMPROBAR_ERROR(m.registrarProducto(Producto("P001", "B", 2.0, 5, 1)));
        COMPROBAR(m.getProductos().size() == 1);
        COMPROBAR(m.getProductos()[0].getNombre() == "A");
    }
    mini::seccion("Campos vacios, precio negativo y stock negativo se rechazan");
    {
        COMPROBAR_ERROR(Producto("", "A", 1.0, 1, 1));
        COMPROBAR_ERROR(Producto("P1", "   ", 1.0, 1, 1));
        COMPROBAR_ERROR(Producto("P1", "A", -1.0, 1, 1));
        COMPROBAR_ERROR(Producto("P1", "A", 1.0, -1, 1));
        COMPROBAR_ERROR(Producto("P1", "A", 1.0, 1, -1));
    }
    mini::seccion("Producto desactivado sigue en la lista como inactivo y no se puede vender");
    {
        ModeloPYME m = nuevoModelo();
        m.registrarProducto(Producto("P001", "A", 1.0, 10, 1));
        m.desactivarProducto("P001");
        COMPROBAR(m.getProductos().size() == 1);
        COMPROBAR(!m.getProductos()[0].isActivo());
        COMPROBAR_ERROR(m.buscarProductoPorCodigo("P001")->vender(1));
        COMPROBAR_ERROR(m.desactivarProducto("NOEXISTE"));
    }
    mini::seccion("Stock bajo: solo productos con stock <= minimo");
    {
        ModeloPYME m = nuevoModelo();
        m.registrarProducto(Producto("A", "Alto", 1.0, 10, 5));
        m.registrarProducto(Producto("B", "Igual", 1.0, 5, 5));
        m.registrarProducto(Producto("C", "Bajo", 1.0, 2, 5));
        auto bajos = m.productosConStockBajo();
        COMPROBAR(bajos.size() == 2);
        COMPROBAR(bajos[0].getCodigo() == "B" && bajos[1].getCodigo() == "C");
    }
    mini::seccion("RF-10: datos comerciales validos y rechazo de invalidos");
    {
        ModeloPYME m = nuevoModelo();
        m.registrarProveedor("Distribuidora Mascotas", "", "0991111111", "");
        DatosComerciales d;
        d.categoria = "seco"; d.unidadVenta = "bulto"; d.precioCompra = 3.2;
        d.fechaCaducidad = "2027-02-28"; d.idProveedor = 1; d.variante = "pollo";
        Producto p("P001", "Croquetas", 5.5, 10, 2);
        p.definirDatosComerciales(d);
        m.registrarProducto(p);
        const Producto& g = m.getProductos()[0];
        COMPROBAR(g.getCategoria() == "seco" && g.getUnidadVenta() == "bulto");
        COMPROBAR(CERCA(g.getPrecioCompra(), 3.2) && g.getFechaCaducidad() == "2027-02-28");
        COMPROBAR(g.getIdProveedor() == 1 && g.getVariante() == "pollo");

        Producto q("P002", "X", 1.0, 1, 1);
        DatosComerciales malo = d;
        malo.precioCompra = -1;           COMPROBAR_ERROR(q.definirDatosComerciales(malo));
        malo = d; malo.fechaCaducidad = "2026-02-30";  COMPROBAR_ERROR(q.definirDatosComerciales(malo));
        malo = d; malo.fechaCaducidad = "28/02/2027";  COMPROBAR_ERROR(q.definirDatosComerciales(malo));
        malo = d; malo.unidadVenta = "caja";           COMPROBAR_ERROR(q.definirDatosComerciales(malo));
        COMPROBAR(q.getCategoria().empty());            // no quedo a medias
        COMPROBAR_NO_ERROR(q.definirDatosComerciales(d));
        DatosComerciales bisiesto = d; bisiesto.fechaCaducidad = "2028-02-29";
        COMPROBAR_NO_ERROR(q.definirDatosComerciales(bisiesto));

        Producto r("P003", "Y", 1.0, 1, 1);
        DatosComerciales sinProv = d; sinProv.idProveedor = 99;
        r.definirDatosComerciales(sinProv);
        COMPROBAR_ERROR(m.registrarProducto(r)); // proveedor inexistente
    }
    mini::seccion("Editar producto: valida sobre copia y conserva el original si falla");
    {
        ModeloPYME m = nuevoModelo();
        m.registrarProducto(Producto("P001", "Original", 5.0, 10, 2));
        DatosComerciales d;
        COMPROBAR_ERROR(m.editarProducto("P001", "Nuevo", -3.0, 10, 2, d));
        COMPROBAR(m.getProductos()[0].getNombre() == "Original");
        COMPROBAR_NO_ERROR(m.editarProducto("P001", "Nuevo", 6.0, 12, 3, d));
        COMPROBAR(m.getProductos()[0].getNombre() == "Nuevo" && m.getProductos()[0].getStock() == 12);
        COMPROBAR_ERROR(m.editarProducto("NOEXISTE", "X", 1.0, 1, 1, d));
    }
    mini::seccion("RF-15: por caducar y precio de promocion");
    {
        ModeloPYME m = nuevoModelo();
        DatosComerciales d; d.precioCompra = 2.0;
        Producto a("A", "Proximo", 5.0, 5, 1);  d.fechaCaducidad = "2026-12-10"; a.definirDatosComerciales(d);
        Producto b("B", "Lejano", 5.0, 5, 1);   d.fechaCaducidad = "2027-06-01"; b.definirDatosComerciales(d);
        Producto c("C", "SinFecha", 5.0, 5, 1);
        Producto e("E", "Pasado", 5.0, 5, 1);   d.fechaCaducidad = "2026-11-01"; e.definirDatosComerciales(d);
        m.registrarProducto(a); m.registrarProducto(b); m.registrarProducto(c); m.registrarProducto(e);
        auto lista = m.productosPorCaducar("2026-11-28", 30);
        COMPROBAR(lista.size() == 1 && lista[0].getCodigo() == "A");
        COMPROBAR_ERROR(m.productosPorCaducar("2026-11-28", -1));
        COMPROBAR_ERROR(m.asignarPromocion("A", 1.5));   // menor al precio de compra
        COMPROBAR_ERROR(m.asignarPromocion("A", 6.0));   // mayor al precio normal
        COMPROBAR_NO_ERROR(m.asignarPromocion("A", 3.0));
        COMPROBAR(CERCA(m.buscarProductoPorCodigo("A")->getPrecioVigente(), 3.0));
        m.desactivarProducto("B");
        COMPROBAR_ERROR(m.asignarPromocion("B", 3.0));   // solo productos activos
    }
    return mini::resumen();
}
