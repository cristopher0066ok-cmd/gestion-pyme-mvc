// Pruebas del Modelo: ventas e inventario (RF-04, RF-05, RF-11) y facturas de compra (RF-12).
#include "mini_test.hpp"
#include "modelo/ModeloPYME.hpp"

static ModeloPYME modeloBase() {
    ModeloPYME m("build/datos_prueba_ventas");
    m.registrarCliente("Ana Perez", "0990000001", "");
    m.registrarProveedor("Distribuidora Mascotas", "Carlos", "0981111111", "");
    m.registrarProducto(Producto("A", "Croquetas", 10.0, 10, 2));
    m.registrarProducto(Producto("B", "Juguete", 2.50, 4, 1));
    return m;
}

int main() {
    mini::seccion("Venta con varios productos: subtotales, total y stock descontado");
    {
        ModeloPYME m = modeloBase();
        Venta v = m.procesarVenta(1, {{"A", 3}, {"B", 2}}, "2026-11-28", "10:30", "efectivo", "", 50.0, false);
        COMPROBAR(v.getNumero() == 1 && v.isConfirmada());
        COMPROBAR(v.getDetalles().size() == 2);
        COMPROBAR(CERCA(v.getDetalles()[0].calcularSubtotal(), 30.0));
        COMPROBAR(CERCA(v.getDetalles()[1].calcularSubtotal(), 5.0));
        COMPROBAR(CERCA(v.getTotal(), 35.0));
        COMPROBAR(CERCA(v.getVuelto(), 15.0));
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 7);
        COMPROBAR(m.buscarProductoPorCodigo("B")->getStock() == 2);
        COMPROBAR(m.getVentas().size() == 1);
        COMPROBAR(v.getHora() == "10:30" && v.getMetodoPago() == "efectivo");
        // El detalle conserva el precio vigente aunque el precio cambie despues
        m.buscarProductoPorCodigo("A")->actualizarPrecio(99.0);
        COMPROBAR(CERCA(m.getVentas()[0].getDetalles()[0].getPrecioUnitario(), 10.0));
    }
    mini::seccion("Vender mas de lo disponible se rechaza sin dejar datos parciales");
    {
        ModeloPYME m = modeloBase();
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 11}}, "2026-11-28", "10:30", "efectivo", "", 500.0, false));
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 10 && m.getVentas().empty());
        // el primer producto es valido pero el segundo falla: ninguno se descuenta
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 3}, {"B", 99}}, "2026-11-28", "10:30", "efectivo", "", 500.0, false));
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 10);
        COMPROBAR(m.buscarProductoPorCodigo("B")->getStock() == 4);
        COMPROBAR(m.getVentas().empty());
        // el mismo producto repetido se suma contra el stock
        COMPROBAR_ERROR(m.procesarVenta(1, {{"B", 3}, {"B", 3}}, "2026-11-28", "10:30", "efectivo", "", 500.0, false));
        COMPROBAR(m.buscarProductoPorCodigo("B")->getStock() == 4);
        // el numero de venta no se consume al fallar
        COMPROBAR(m.procesarVenta(1, {{"A", 1}}, "2026-11-28", "10:30", "efectivo", "", 10.0, false).getNumero() == 1);
    }
    mini::seccion("Cantidad invalida, producto desactivado, cliente inexistente, sin detalles");
    {
        ModeloPYME m = modeloBase();
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 0}}, "2026-11-28", "10:30", "efectivo", "", 50.0, false));
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", -2}}, "2026-11-28", "10:30", "efectivo", "", 50.0, false));
        COMPROBAR_ERROR(m.procesarVenta(1, {{"ZZZ", 1}}, "2026-11-28", "10:30", "efectivo", "", 50.0, false));
        COMPROBAR_ERROR(m.procesarVenta(99, {{"A", 1}}, "2026-11-28", "10:30", "efectivo", "", 50.0, false));
        COMPROBAR_ERROR(m.procesarVenta(1, {}, "2026-11-28", "10:30", "efectivo", "", 50.0, false));
        m.desactivarProducto("B");
        COMPROBAR_ERROR(m.procesarVenta(1, {{"B", 1}}, "2026-11-28", "10:30", "efectivo", "", 50.0, false));
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 1}}, "2026-13-40", "10:30", "efectivo", "", 50.0, false));
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 1}}, "2026-11-28", "25:61", "efectivo", "", 50.0, false));
        COMPROBAR(m.getVentas().empty() && m.buscarProductoPorCodigo("A")->getStock() == 10);
    }
    mini::seccion("RF-11: pago en efectivo y por transferencia");
    {
        ModeloPYME m = modeloBase();
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 2}}, "2026-11-28", "10:30", "efectivo", "", 19.99, false));
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 10);   // el stock no cambia
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 2}}, "2026-11-28", "10:30", "transferencia", "", 0, true));          // sin banco
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 2}}, "2026-11-28", "10:30", "transferencia", "Pichincha", 0, false)); // no recibida
        COMPROBAR_ERROR(m.procesarVenta(1, {{"A", 2}}, "2026-11-28", "10:30", "cheque", "", 100, false));
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 10 && m.getVentas().empty());
        Venta t = m.procesarVenta(1, {{"A", 2}}, "2026-11-28", "11:05", "transferencia", "Pichincha", 0, true);
        COMPROBAR(t.getBanco() == "Pichincha" && t.isTransferenciaRecibida() && CERCA(t.getVuelto(), 0.0));
        Venta e = m.procesarVenta(1, {{"B", 1}}, "2026-11-28", "11:10", "efectivo", "", 20.0, false);
        COMPROBAR(CERCA(e.getVuelto(), 17.5));
        COMPROBAR(e.getNumero() == 2);
    }
    mini::seccion("Resumen de ventas: cantidad y monto total");
    {
        ModeloPYME m = modeloBase();
        COMPROBAR(m.generarResumen().cantidadVentas == 0 && CERCA(m.generarResumen().montoTotal, 0.0));
        m.procesarVenta(1, {{"A", 1}}, "2026-11-28", "10:00", "efectivo", "", 10.0, false);
        m.procesarVenta(1, {{"B", 2}}, "2026-11-28", "10:10", "efectivo", "", 5.0, false);
        ResumenVentas r = m.generarResumen();
        COMPROBAR(r.cantidadVentas == 2 && CERCA(r.montoTotal, 15.0));
    }
    mini::seccion("Promocion: la venta usa el precio de promocion vigente");
    {
        ModeloPYME m = modeloBase();
        m.asignarPromocion("A", 8.0);
        Venta v = m.procesarVenta(1, {{"A", 2}}, "2026-11-28", "10:00", "efectivo", "", 16.0, false);
        COMPROBAR(CERCA(v.getTotal(), 16.0));
        COMPROBAR(CERCA(m.calcularTotalPrevio({{"A", 3}}), 24.0));
        COMPROBAR_ERROR(m.calcularTotalPrevio({{"ZZZ", 1}}));
    }
    mini::seccion("RF-12: factura aumenta stock y actualiza precio de compra");
    {
        ModeloPYME m = modeloBase();
        m.registrarFactura("001-001-000123", "CLAVE123", "2026-11-20", 1, "Carlos", "fotos/f1.jpg",
                           {{"A", 5, 6.0}, {"B", 10, 1.2}});
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 15);
        COMPROBAR(m.buscarProductoPorCodigo("B")->getStock() == 14);
        COMPROBAR(CERCA(m.buscarProductoPorCodigo("A")->getPrecioCompra(), 6.0));
        COMPROBAR(m.getFacturas().size() == 1);
        COMPROBAR(CERCA(m.getFacturas()[0].calcularTotal(), 5 * 6.0 + 10 * 1.2));
        // duplicada: mismo proveedor y numero
        COMPROBAR_ERROR(m.registrarFactura("001-001-000123", "", "2026-11-21", 1, "", "", {{"A", 1, 1.0}}));
        // si un detalle falla no se suma stock ni se guarda la factura
        COMPROBAR_ERROR(m.registrarFactura("F2", "", "2026-11-21", 1, "", "", {{"A", 5, 6.0}, {"B", 0, 1.0}}));
        COMPROBAR_ERROR(m.registrarFactura("F3", "", "2026-11-21", 1, "", "", {{"A", 5, -1.0}}));
        COMPROBAR_ERROR(m.registrarFactura("F4", "", "2026-11-21", 1, "", "", {{"ZZZ", 5, 1.0}}));
        COMPROBAR_ERROR(m.registrarFactura("F5", "", "2026-11-21", 99, "", "", {{"A", 5, 1.0}}));
        COMPROBAR_ERROR(m.registrarFactura("F6", "", "2026-11-21", 1, "", "", {}));
        COMPROBAR_ERROR(m.registrarFactura("F7", "", "2026-21-21", 1, "", "", {{"A", 1, 1.0}}));
        COMPROBAR(m.buscarProductoPorCodigo("A")->getStock() == 15 && m.getFacturas().size() == 1);
        // el mismo numero en otro proveedor SI se permite
        m.registrarProveedor("Agro Pet", "", "0982222222", "");
        COMPROBAR_NO_ERROR(m.registrarFactura("001-001-000123", "", "2026-11-22", 2, "", "", {{"A", 1, 1.0}}));
    }
    mini::seccion("RF-12: busqueda de facturas por proveedor, fechas, numero y producto");
    {
        ModeloPYME m = modeloBase();
        m.registrarProveedor("Agro Pet", "", "0982222222", "");
        m.registrarFactura("F1", "", "2026-11-05", 1, "", "", {{"A", 1, 1.0}});
        m.registrarFactura("F2", "", "2026-11-15", 2, "", "", {{"B", 1, 1.0}});
        m.registrarFactura("F3", "", "2026-11-25", 1, "", "", {{"B", 1, 1.0}});
        COMPROBAR(m.buscarFacturas(0, "", "", "", "").size() == 3);
        COMPROBAR(m.buscarFacturas(1, "", "", "", "").size() == 2);
        COMPROBAR(m.buscarFacturas(0, "2026-11-10", "2026-11-30", "", "").size() == 2);
        COMPROBAR(m.buscarFacturas(0, "", "", "f2", "").size() == 1);
        COMPROBAR(m.buscarFacturas(0, "", "", "", "B").size() == 2);
        COMPROBAR(m.buscarFacturas(1, "2026-11-10", "", "", "B").size() == 1);
        COMPROBAR_ERROR(m.buscarFacturas(0, "2026-12-01", "2026-11-01", "", ""));
        COMPROBAR_ERROR(m.buscarFacturas(0, "x", "", "", ""));
    }
    mini::seccion("RF-13: cierre diario y cierre mensual encadenado");
    {
        ModeloPYME m = modeloBase();
        m.registrarProducto(Producto("C", "Collar", 20.0, 100, 1));
        m.procesarVenta(1, {{"A", 1}}, "2026-11-10", "09:00", "efectivo", "", 10.0, false);              // 10 efectivo
        m.procesarVenta(1, {{"C", 2}}, "2026-11-10", "10:00", "transferencia", "Pichincha", 0, true);    // 40 Pichincha
        m.procesarVenta(1, {{"C", 1}}, "2026-11-10", "11:00", "transferencia", "Produbanco", 0, true);   // 20 Produbanco
        m.procesarVenta(1, {{"C", 1}}, "2026-12-03", "11:00", "efectivo", "", 20.0, false);              // 20 efectivo (diciembre)
        CierreDiario d = m.cierreDiario("2026-11-10");
        COMPROBAR(d.cantidadVentas == 3 && CERCA(d.efectivo, 10.0) && CERCA(d.transferencia, 60.0));
        COMPROBAR(CERCA(d.totalVendido, 70.0));
        COMPROBAR(CERCA(d.porBanco["Pichincha"], 40.0) && CERCA(d.porBanco["Produbanco"], 20.0));
        COMPROBAR(CERCA(d.totalVendido, d.efectivo + d.transferencia));
        COMPROBAR(m.cierreDiario("2026-11-11").cantidadVentas == 0);
        COMPROBAR_ERROR(m.cierreDiario("2026-02-30"));

        m.establecerSaldoBase("2026-10", 450.00, 300.00);
        m.registrarProveedor("Agro Pet", "", "0982222222", "");
        int pago = m.registrarPendiente(1, SentidoPendiente::LocalDebe, "Pago a proveedor", 100.0, "", 0, "2026-11-01");
        m.saldarPendiente(pago, "2026-11-20", "banco");
        CierreMensual nov = m.cierreMensual("2026-11");
        COMPROBAR(CERCA(nov.saldoInicialCaja, 450.00) && CERCA(nov.saldoInicialBanco, 300.00));
        COMPROBAR(CERCA(nov.ventasEfectivo, 10.0) && CERCA(nov.ventasBanco, 60.0));
        COMPROBAR(CERCA(nov.pagosBanco, 100.0) && CERCA(nov.pagosEfectivo, 0.0));
        COMPROBAR(CERCA(nov.saldoFinalCaja, 460.00) && CERCA(nov.saldoFinalBanco, 260.00));
        COMPROBAR(CERCA(nov.acumulado, 720.00));
        CierreMensual dic = m.cierreMensual("2026-12");
        COMPROBAR(CERCA(dic.saldoInicialCaja, nov.saldoFinalCaja));      // encadenado
        COMPROBAR(CERCA(dic.saldoInicialBanco, nov.saldoFinalBanco));
        COMPROBAR(CERCA(dic.saldoFinalCaja, 480.00));
        COMPROBAR_ERROR(m.cierreMensual("2026-10"));                     // no posterior al mes base
        COMPROBAR_ERROR(m.cierreMensual("2026-1"));
        COMPROBAR_ERROR(m.establecerSaldoBase("2026-10", -1, 0));
    }
    return mini::resumen();
}
