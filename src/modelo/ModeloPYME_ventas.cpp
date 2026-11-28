// Seccion VENTAS (y facturas RF-12) de ModeloPYME. Responsable: Sebastian.
#include "ModeloPYME.hpp"

double ModeloPYME::calcularTotalPrevio(const std::vector<LineaVenta>& lineas) const {
    double total = 0;
    for (const auto& l : lineas) {
        int i = indiceProducto(Validaciones::recortar(l.codigoProducto));
        if (i < 0) {
            throw ErrorDominio("Producto no encontrado: " + l.codigoProducto);
        }
        if (l.cantidad <= 0) {
            throw ErrorDominio("La cantidad de cada detalle debe ser positiva.");
        }
        total += l.cantidad * productos[static_cast<size_t>(i)].getPrecioVigente();
    }
    return total;
}

// Atomica: todo se valida y descuenta sobre una copia de los productos. Si cualquier paso lanza
// ErrorDominio, ni el stock ni la lista de ventas se modifican.
Venta ModeloPYME::procesarVenta(int idCliente, const std::vector<LineaVenta>& lineas,
                                const std::string& fecha, const std::string& hora,
                                const std::string& metodoPago, const std::string& banco,
                                double montoRecibido, bool transferenciaRecibida) {
    if (buscarClientePorId(idCliente) == nullptr) {
        throw ErrorDominio("La venta debe pertenecer a un cliente existente.");
    }
    if (lineas.empty()) {
        throw ErrorDominio("Una venta debe tener al menos un detalle.");
    }

    Venta venta(siguienteNumeroVenta, fecha, hora, idCliente);
    std::vector<Producto> copia = productos;
    for (const auto& l : lineas) {
        std::string codigo = Validaciones::recortar(l.codigoProducto);
        int i = indiceProducto(codigo);
        if (i < 0) {
            throw ErrorDominio("Producto no encontrado: " + codigo);
        }
        Producto& p = copia[static_cast<size_t>(i)];
        p.vender(l.cantidad); // valida: activo, cantidad positiva y stock suficiente
        venta.agregarDetalle(DetalleVenta(codigo, l.cantidad, p.getPrecioVigente()));
    }
    venta.definirPago(metodoPago, banco, montoRecibido, transferenciaRecibida);
    venta.confirmar();

    productos.swap(copia);
    ventas.push_back(venta);
    ++siguienteNumeroVenta;
    return venta;
}

const std::vector<Venta>& ModeloPYME::getVentas() const {
    return ventas;
}

// ---------------------------------------------------------------- RF-12

void ModeloPYME::registrarFactura(const std::string& numero, const std::string& claveAcceso,
                                  const std::string& fecha, int idProveedor,
                                  const std::string& atendidoPor, const std::string& rutaFoto,
                                  const std::vector<LineaFactura>& lineas) {
    if (buscarProveedorPorId(idProveedor) == nullptr) {
        throw ErrorDominio("La factura debe pertenecer a un proveedor existente.");
    }
    Factura factura(numero, claveAcceso, fecha, idProveedor, atendidoPor, rutaFoto);
    for (const auto& f : facturas) {
        if (f.getIdProveedor() == idProveedor && f.getNumero() == factura.getNumero()) {
            throw ErrorDominio("La factura " + factura.getNumero() + " de ese proveedor ya esta registrada.");
        }
    }
    if (lineas.empty()) {
        throw ErrorDominio("Una factura debe tener al menos un detalle.");
    }

    std::vector<Producto> copia = productos;
    for (const auto& l : lineas) {
        std::string codigo = Validaciones::recortar(l.codigoProducto);
        int i = indiceProducto(codigo);
        if (i < 0) {
            throw ErrorDominio("Producto no encontrado: " + codigo);
        }
        DetalleFactura detalle(codigo, l.cantidad, l.precioCompra); // valida cantidad y precio
        Producto& p = copia[static_cast<size_t>(i)];
        p.agregarStock(detalle.getCantidad());
        p.actualizarPrecioCompra(detalle.getPrecioCompra());
        factura.agregarDetalle(detalle);
    }
    productos.swap(copia);
    facturas.push_back(factura);
}

const std::vector<Factura>& ModeloPYME::getFacturas() const {
    return facturas;
}

std::vector<Factura> ModeloPYME::buscarFacturas(int idProveedor, const std::string& desde,
                                                const std::string& hasta, const std::string& numero,
                                                const std::string& codigoProducto) const {
    std::string d = Validaciones::recortar(desde);
    std::string h = Validaciones::recortar(hasta);
    std::string n = Validaciones::recortar(numero);
    std::string c = Validaciones::recortar(codigoProducto);
    if (!d.empty() && !Validaciones::fechaValida(d)) {
        throw ErrorDominio("La fecha 'desde' debe tener el formato AAAA-MM-DD y ser valida.");
    }
    if (!h.empty() && !Validaciones::fechaValida(h)) {
        throw ErrorDominio("La fecha 'hasta' debe tener el formato AAAA-MM-DD y ser valida.");
    }
    if (!d.empty() && !h.empty() && d > h) {
        throw ErrorDominio("La fecha 'desde' no puede ser posterior a la fecha 'hasta'.");
    }
    std::vector<Factura> resultado;
    for (const auto& f : facturas) {
        if (idProveedor > 0 && f.getIdProveedor() != idProveedor) continue;
        if (!d.empty() && f.getFecha() < d) continue;
        if (!h.empty() && f.getFecha() > h) continue;
        if (!n.empty() && !Validaciones::contiene(f.getNumero(), n)) continue;
        if (!c.empty() && !f.incluyeProducto(c)) continue;
        resultado.push_back(f);
    }
    return resultado;
}
