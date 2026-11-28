#include "ControladorPYME.hpp"

ControladorPYME::ControladorPYME(ModeloPYME& mod, VistaConsola& vis) : modelo(mod), vista(vis) {}

std::string ControladorPYME::fechaONuevaHoy(const std::string& texto) const {
    std::string t = Validaciones::recortar(texto);
    return t.empty() ? Tiempo::fechaActual() : t;
}

void ControladorPYME::guardar() {
    try {
        modelo.guardarDatos();
    } catch (const ErrorDominio& e) {
        vista.mostrarMensajeError(std::string("No se pudieron guardar los datos: ") + e.what());
    }
}

void ControladorPYME::ejecutar() {
    try {
        vista.mostrarAvisos(modelo.cargarDatos());
    } catch (const std::exception& e) {
        vista.mostrarMensajeError(std::string("No se pudieron cargar los datos: ") + e.what());
    }

    int opcion = 0;
    try {
        do {
            opcion = vista.mostrarMenuPrincipal();
            try {
                switch (opcion) {
                    case 1: gestionarProductos(); break;
                    case 2: gestionarClientes(); break;
                    case 3: gestionarProveedores(); break;
                    case 4: gestionarVentas(); break;
                    case 5: gestionarFacturas(); break;
                    case 6: gestionarPendientes(); break;
                    case 7: gestionarCierres(); break;
                    case 8: consultarReportes(); break;
                    case 0: break;
                    default: vista.mostrarMensajeError("Opcion invalida. Intente nuevamente.");
                }
            } catch (const FinDeEntrada&) {
                throw;
            } catch (const ErrorDominio& e) {
                vista.mostrarMensajeError(e.what());
            } catch (const std::exception& e) {
                vista.mostrarMensajeError(std::string("Error inesperado: ") + e.what());
            }
        } while (opcion != 0);
    } catch (const FinDeEntrada&) {
        // La entrada termino: se cierra de forma ordenada.
    }
    guardar();
    vista.mostrarMensajeExito("Datos guardados. Saliendo del sistema. Hasta luego!");
}

// ---------------------------------------------------------------- Productos

namespace {
DatosComerciales leerDatosComerciales(const VistaConsola& v) {
    DatosComerciales d;
    d.categoria = v.leerTexto("Categoria (ej: seco, humedo, accesorio, golosina)");
    d.unidadVenta = v.leerTexto("Unidad de venta (unidad/bulto)");
    d.precioCompra = v.leerDecimal("Precio de compra");
    d.fechaCaducidad = v.leerTexto("Fecha de caducidad AAAA-MM-DD (Enter si no aplica)");
    d.idProveedor = v.leerEntero("ID del proveedor (0 si no tiene)");
    d.variante = v.leerTexto("Variante: sabor, color o aroma (Enter si no aplica)");
    return d;
}
} // namespace

void ControladorPYME::gestionarProductos() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuProductos();
        try {
            switch (opcion) {
                case 1: {
                    std::string codigo = vista.leerTexto("Codigo del producto");
                    std::string nombre = vista.leerTexto("Nombre del producto");
                    double precio = vista.leerDecimal("Precio de venta");
                    int stock = vista.leerEntero("Stock inicial");
                    int stockMinimo = vista.leerEntero("Stock minimo de reposicion");
                    Producto nuevo(codigo, nombre, precio, stock, stockMinimo);
                    nuevo.definirDatosComerciales(leerDatosComerciales(vista));
                    modelo.registrarProducto(nuevo);
                    guardar();
                    vista.mostrarMensajeExito("Producto registrado exitosamente.");
                    break;
                }
                case 2:
                    vista.mostrarListaProductos(modelo.getProductos());
                    break;
                case 3: {
                    std::string texto = vista.leerTexto("Codigo o nombre a buscar");
                    std::vector<Producto> encontrados = modelo.buscarProductos(texto);
                    if (encontrados.empty()) {
                        vista.mostrarMensajeError("No se encontraron productos con ese codigo o nombre.");
                    } else if (encontrados.size() == 1) {
                        vista.mostrarProducto(encontrados[0]);
                    } else {
                        vista.mostrarListaProductos(encontrados, "RESULTADO DE LA BUSQUEDA");
                    }
                    break;
                }
                case 4: {
                    std::string codigo = vista.leerTexto("Codigo del producto a editar");
                    Producto* actual = modelo.buscarProductoPorCodigo(codigo);
                    if (actual == nullptr) {
                        vista.mostrarMensajeError("Producto no encontrado.");
                        break;
                    }
                    vista.mostrarProducto(*actual);
                    vista.mostrarMensajeInfo("Ingrese los nuevos valores (el codigo no cambia).");
                    std::string nombre = vista.leerTexto("Nombre");
                    double precio = vista.leerDecimal("Precio de venta");
                    int stock = vista.leerEntero("Stock actual");
                    int stockMinimo = vista.leerEntero("Stock minimo");
                    DatosComerciales datos = leerDatosComerciales(vista);
                    modelo.editarProducto(codigo, nombre, precio, stock, stockMinimo, datos);
                    guardar();
                    vista.mostrarMensajeExito("Producto actualizado correctamente.");
                    break;
                }
                case 5: {
                    std::string codigo = vista.leerTexto("Codigo del producto a desactivar");
                    modelo.desactivarProducto(codigo);
                    guardar();
                    vista.mostrarMensajeExito("Producto desactivado correctamente (conserva su historial).");
                    break;
                }
                case 6: {
                    int dias = vista.leerEntero("Mostrar productos que caducan en los proximos N dias. N");
                    std::vector<Producto> lista = modelo.productosPorCaducar(Tiempo::fechaActual(), dias);
                    vista.mostrarListaProductos(lista, "PRODUCTOS POR CADUCAR (proximos " + std::to_string(dias) + " dias)");
                    break;
                }
                case 7: {
                    std::string codigo = vista.leerTexto("Codigo del producto");
                    Producto* p = modelo.buscarProductoPorCodigo(codigo);
                    if (p == nullptr) {
                        vista.mostrarMensajeError("Producto no encontrado.");
                        break;
                    }
                    vista.mostrarProducto(*p);
                    double precio = vista.leerDecimal("Precio de promocion");
                    modelo.asignarPromocion(codigo, precio);
                    guardar();
                    vista.mostrarMensajeExito("Precio de promocion asignado.");
                    break;
                }
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de productos invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Clientes

void ControladorPYME::gestionarClientes() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuClientes();
        try {
            switch (opcion) {
                case 1: {
                    std::string nombre = vista.leerTexto("Nombre del cliente");
                    std::string telefono = vista.leerTexto("Telefono (Enter si no tiene)");
                    std::string correo = vista.leerTexto("Correo (Enter si no tiene)");
                    int id = modelo.registrarCliente(nombre, telefono, correo);
                    guardar();
                    vista.mostrarMensajeExito("Cliente registrado con ID " + std::to_string(id) + ".");
                    break;
                }
                case 2:
                    vista.mostrarListaClientes(modelo.getClientes());
                    break;
                case 3: {
                    std::string texto = vista.leerTexto("Nombre, telefono o correo a buscar");
                    std::vector<Cliente> encontrados = modelo.buscarClientes(texto);
                    if (encontrados.empty()) vista.mostrarMensajeError("No se encontraron clientes.");
                    else vista.mostrarListaClientes(encontrados);
                    break;
                }
                case 4: {
                    int id = vista.leerEntero("ID del cliente");
                    Cliente* c = modelo.buscarClientePorId(id);
                    if (c == nullptr) {
                        vista.mostrarMensajeError("Cliente no encontrado.");
                        break;
                    }
                    vista.mostrarMensajeInfo(c->datosResumen());
                    std::string telefono = vista.leerTexto("Nuevo telefono (Enter para dejarlo vacio)");
                    std::string correo = vista.leerTexto("Nuevo correo (Enter para dejarlo vacio)");
                    modelo.editarCliente(id, telefono, correo);
                    guardar();
                    vista.mostrarMensajeExito("Contacto actualizado.");
                    break;
                }
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de clientes invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Proveedores

void ControladorPYME::gestionarProveedores() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuProveedores();
        try {
            switch (opcion) {
                case 1: {
                    std::string nombre = vista.leerTexto("Nombre comercial del proveedor");
                    std::string vendedor = vista.leerTexto("Nombre de quien atiende (Enter si no se sabe)");
                    std::string telefono = vista.leerTexto("Telefono (Enter si no tiene)");
                    std::string correo = vista.leerTexto("Correo (Enter si no tiene)");
                    int id = modelo.registrarProveedor(nombre, vendedor, telefono, correo);
                    guardar();
                    vista.mostrarMensajeExito("Proveedor registrado con ID " + std::to_string(id) + ".");
                    break;
                }
                case 2:
                    vista.mostrarListaProveedores(modelo.getProveedores());
                    break;
                case 3: {
                    std::string texto = vista.leerTexto("Nombre, vendedor, telefono o correo a buscar");
                    std::vector<Proveedor> encontrados = modelo.buscarProveedores(texto);
                    if (encontrados.empty()) vista.mostrarMensajeError("No se encontraron proveedores.");
                    else vista.mostrarListaProveedores(encontrados);
                    break;
                }
                case 4: {
                    int id = vista.leerEntero("ID del proveedor");
                    Proveedor* p = modelo.buscarProveedorPorId(id);
                    if (p == nullptr) {
                        vista.mostrarMensajeError("Proveedor no encontrado.");
                        break;
                    }
                    vista.mostrarMensajeInfo(p->datosResumen());
                    std::string vendedor = vista.leerTexto("Nombre de quien atiende (Enter para dejarlo vacio)");
                    std::string telefono = vista.leerTexto("Nuevo telefono (Enter para dejarlo vacio)");
                    std::string correo = vista.leerTexto("Nuevo correo (Enter para dejarlo vacio)");
                    modelo.editarProveedor(id, vendedor, telefono, correo);
                    guardar();
                    vista.mostrarMensajeExito("Proveedor actualizado.");
                    break;
                }
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de proveedores invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Ventas

void ControladorPYME::registrarVenta() {
    int idCliente = vista.leerEntero("ID del cliente");
    Cliente* cliente = modelo.buscarClientePorId(idCliente);
    if (cliente == nullptr) {
        vista.mostrarMensajeError("Cliente no encontrado. Registre el cliente primero.");
        return;
    }
    vista.mostrarMensajeInfo("Cliente: " + cliente->getNombre());

    std::vector<LineaVenta> lineas;
    bool otro = true;
    while (otro) {
        std::string codigo = vista.leerTexto("Codigo del producto");
        int cantidad = vista.leerEntero("Cantidad");
        lineas.push_back({codigo, cantidad});
        try {
            double parcial = modelo.calcularTotalPrevio(lineas);
            vista.mostrarTotal("Total parcial", parcial);
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
            lineas.pop_back(); // la linea con problema no se conserva
        }
        otro = vista.solicitarConfirmacion("Agregar otro producto?");
    }
    if (lineas.empty()) {
        vista.mostrarMensajeError("La venta no tiene productos. Venta cancelada.");
        return;
    }

    vista.mostrarTotal("TOTAL A PAGAR", modelo.calcularTotalPrevio(lineas));

    int metodo = vista.leerEntero("Metodo de pago (1 = efectivo, 2 = transferencia)");
    std::string metodoPago;
    std::string banco;
    double montoRecibido = 0;
    bool recibida = false;
    if (metodo == 1) {
        metodoPago = "efectivo";
        montoRecibido = vista.leerDecimal("Monto recibido");
    } else if (metodo == 2) {
        metodoPago = "transferencia";
        banco = vista.leerTexto("Banco");
        recibida = vista.solicitarConfirmacion("La transferencia ya fue recibida?");
    } else {
        vista.mostrarMensajeError("Metodo de pago invalido. Venta cancelada.");
        return;
    }

    if (!vista.solicitarConfirmacion("Confirmar la venta?")) {
        vista.mostrarMensajeInfo("Venta cancelada. No se modifico el inventario.");
        return;
    }
    Venta venta = modelo.procesarVenta(idCliente, lineas, Tiempo::fechaActual(), Tiempo::horaActual(),
                                       metodoPago, banco, montoRecibido, recibida);
    guardar();
    vista.mostrarMensajeExito("Venta confirmada y stock descontado.");
    vista.mostrarVenta(venta);
}

void ControladorPYME::gestionarVentas() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuVentas();
        try {
            switch (opcion) {
                case 1: registrarVenta(); break;
                case 2: vista.mostrarListaVentas(modelo.getVentas()); break;
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de ventas invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Facturas (RF-12)

void ControladorPYME::registrarFactura() {
    std::string numero = vista.leerTexto("Numero de factura");
    std::string clave = vista.leerTexto("Clave de acceso (Enter si no tiene)");
    std::string fecha = fechaONuevaHoy(vista.leerTexto("Fecha AAAA-MM-DD (Enter = hoy)"));
    int idProveedor = vista.leerEntero("ID del proveedor");
    std::string atendio = vista.leerTexto("Nombre de quien atendio (Enter si no se sabe)");
    std::string ruta = vista.leerTexto("Ruta del archivo de la foto (Enter si no hay)");

    std::vector<LineaFactura> lineas;
    bool otro = true;
    while (otro) {
        std::string codigo = vista.leerTexto("Codigo del producto");
        int cantidad = vista.leerEntero("Cantidad");
        double precio = vista.leerDecimal("Precio de compra unitario");
        lineas.push_back({codigo, cantidad, precio});
        otro = vista.solicitarConfirmacion("Agregar otro producto a la factura?");
    }
    if (!vista.solicitarConfirmacion("Confirmar la factura (aumenta el stock)?")) {
        vista.mostrarMensajeInfo("Factura cancelada. No se modifico el inventario.");
        return;
    }
    modelo.registrarFactura(numero, clave, fecha, idProveedor, atendio, ruta, lineas);
    guardar();
    vista.mostrarMensajeExito("Factura registrada: el stock y el precio de compra fueron actualizados.");
}

void ControladorPYME::gestionarFacturas() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuFacturas();
        try {
            switch (opcion) {
                case 1: registrarFactura(); break;
                case 2: {
                    vista.mostrarMensajeInfo("Deje en blanco (o 0) los filtros que no necesite.");
                    int idProveedor = vista.leerEntero("ID del proveedor (0 = todos)");
                    std::string desde = vista.leerTexto("Fecha desde AAAA-MM-DD");
                    std::string hasta = vista.leerTexto("Fecha hasta AAAA-MM-DD");
                    std::string numero = vista.leerTexto("Numero de factura");
                    std::string codigo = vista.leerTexto("Codigo de producto");
                    vista.mostrarListaFacturas(modelo.buscarFacturas(idProveedor, desde, hasta, numero, codigo));
                    break;
                }
                case 3: vista.mostrarListaFacturas(modelo.getFacturas()); break;
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de facturas invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Pendientes (RF-14)

void ControladorPYME::gestionarPendientes() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuPendientes();
        try {
            switch (opcion) {
                case 1: {
                    int idProveedor = vista.leerEntero("ID del proveedor");
                    int sentido = vista.leerEntero("Sentido (1 = le debemos al proveedor, 2 = el proveedor nos debe)");
                    if (sentido != 1 && sentido != 2) {
                        vista.mostrarMensajeError("Sentido invalido.");
                        break;
                    }
                    std::string descripcion = vista.leerTexto("Descripcion");
                    int tipo = vista.leerEntero("Tipo (1 = dinero, 2 = producto)");
                    double monto = 0;
                    std::string codigo;
                    int cantidad = 0;
                    if (tipo == 1) {
                        monto = vista.leerDecimal("Monto");
                    } else if (tipo == 2) {
                        codigo = vista.leerTexto("Codigo del producto");
                        cantidad = vista.leerEntero("Cantidad");
                    } else {
                        vista.mostrarMensajeError("Tipo invalido.");
                        break;
                    }
                    std::string fecha = fechaONuevaHoy(vista.leerTexto("Fecha AAAA-MM-DD (Enter = hoy)"));
                    int id = modelo.registrarPendiente(idProveedor,
                        sentido == 1 ? SentidoPendiente::LocalDebe : SentidoPendiente::ProveedorDebe,
                        descripcion, monto, codigo, cantidad, fecha);
                    guardar();
                    vista.mostrarMensajeExito("Pendiente registrado con numero " + std::to_string(id) + ".");
                    break;
                }
                case 2: {
                    int dias = vista.leerEntero("Resaltar los pendientes que superan N dias. N");
                    std::vector<Pendiente> abiertos = modelo.pendientesAbiertos();
                    std::vector<int> vencidos;
                    for (const auto& p : modelo.pendientesVencidos(Tiempo::fechaActual(), dias)) {
                        vencidos.push_back(p.getId());
                    }
                    vista.mostrarListaPendientes(abiertos, vencidos, "PENDIENTES ABIERTOS");
                    break;
                }
                case 3: {
                    int id = vista.leerEntero("Numero del pendiente a saldar");
                    std::string medio = vista.leerTexto("Medio de pago si el local pago dinero (efectivo/banco; Enter si no aplica)");
                    std::string fecha = fechaONuevaHoy(vista.leerTexto("Fecha AAAA-MM-DD (Enter = hoy)"));
                    modelo.saldarPendiente(id, fecha, medio);
                    guardar();
                    vista.mostrarMensajeExito("Pendiente marcado como saldado.");
                    break;
                }
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de pendientes invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Cierres (RF-13)

void ControladorPYME::gestionarCierres() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuCierres();
        try {
            switch (opcion) {
                case 1: {
                    std::string fecha = fechaONuevaHoy(vista.leerTexto("Fecha AAAA-MM-DD (Enter = hoy)"));
                    vista.mostrarCierreDiario(modelo.cierreDiario(fecha));
                    break;
                }
                case 2: {
                    std::string mes = Validaciones::recortar(vista.leerTexto("Mes AAAA-MM (Enter = mes actual)"));
                    if (mes.empty()) mes = Tiempo::fechaActual().substr(0, 7);
                    vista.mostrarCierreMensual(modelo.cierreMensual(mes));
                    break;
                }
                case 3: {
                    std::string mes = vista.leerTexto("Mes base AAAA-MM (el mes cuyo cierre ya se conoce)");
                    double caja = vista.leerDecimal("Saldo final de caja de ese mes");
                    double banco = vista.leerDecimal("Saldo final de banco de ese mes");
                    modelo.establecerSaldoBase(mes, caja, banco);
                    guardar();
                    vista.mostrarMensajeExito("Saldo base guardado. El cierre del mes siguiente parte de estos saldos.");
                    break;
                }
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de cierres invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}

// ---------------------------------------------------------------- Reportes (RF-06)

void ControladorPYME::consultarReportes() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuReportes();
        try {
            switch (opcion) {
                case 1: vista.mostrarListaProductos(modelo.getProductos(), "INVENTARIO DISPONIBLE"); break;
                case 2: vista.mostrarListaProductos(modelo.productosConStockBajo(), "PRODUCTOS CON STOCK BAJO"); break;
                case 3: vista.mostrarResumenVentas(modelo.generarResumen()); break;
                case 0: break;
                default: vista.mostrarMensajeError("Opcion de reportes invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcion != 0) vista.pausar();
    } while (opcion != 0);
}
