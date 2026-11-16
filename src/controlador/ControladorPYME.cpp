#include "ControladorPYME.hpp"
#include "../modelo/ErrorDominio.hpp"
#include <iostream>

ControladorPYME::ControladorPYME(ModeloPYME& mod, VistaConsola& vis) : modelo(mod), vista(vis) {}

void ControladorPYME::ejecutar() {
    int opcion = 0;
    do {
        opcion = vista.mostrarMenuPrincipal();
        try {
            switch (opcion) {
                case 1:
                    gestionarProductos();
                    break;
                case 2:
                    vista.mostrarMensajeError("Modulo de Clientes en desarrollo por el equipo.");
                    break;
                case 3:
                    vista.mostrarMensajeError("Modulo de Proveedores en desarrollo por el equipo.");
                    break;
                case 4:
                    vista.mostrarMensajeError("Modulo de Ventas en desarrollo por el equipo.");
                    break;
                case 5:
                    vista.mostrarMensajeError("Modulo de Reportes en desarrollo por el equipo.");
                    break;
                case 0:
                    vista.mostrarMensajeExito("Saliendo del sistema. ¡Hasta luego!");
                    break;
                default:
                    vista.mostrarMensajeError("Opcion invalida. Intente nuevamente.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        } catch (const std::exception& e) {
            vista.mostrarMensajeError(std::string("Error inesperado: ") + e.what());
        }
        if (opcion != 0) {
            vista.pausar();
        }
    } while (opcion != 0);
}

void ControladorPYME::gestionarProductos() {
    int opcionProd = 0;
    do {
        opcionProd = vista.mostrarMenuProductos();
        try {
            switch (opcionProd) {
                case 1: { // Registrar producto
                    std::string codigo = vista.leerTexto("Ingrese el codigo del producto");
                    std::string nombre = vista.leerTexto("Ingrese el nombre del producto");
                    double precio = vista.leerDecimal("Ingrese el precio unitario");
                    int stock = vista.leerEntero("Ingrese el stock inicial");
                    int stockMinimo = vista.leerEntero("Ingrese el stock minimo de reposicion");

                    Producto nuevoProd(codigo, nombre, precio, stock, stockMinimo);
                    modelo.registrarProducto(nuevoProd);
                    vista.mostrarMensajeExito("Producto registrado exitosamente.");
                    break;
                }
                case 2: { // Listar productos
                    vista.mostrarListaProductos(modelo.getProductos());
                    break;
                }
                case 3: { // Buscar producto
                    std::string codigo = vista.leerTexto("Ingrese el codigo a buscar");
                    Producto* p = modelo.buscarProductoPorCodigo(codigo);
                    if (p != nullptr) {
                        std::cout << "\n[PRODUCTO ENCONTRADO]:\n"
                                  << "Codigo: " << p->getCodigo() << "\n"
                                  << "Nombre: " << p->getNombre() << "\n"
                                  << "Precio: $" << p->getPrecio() << "\n"
                                  << "Stock: " << p->getStock() << "\n"
                                  << "Minimo: " << p->getStockMinimo() << "\n"
                                  << "Estado: " << (p->isActivo() ? "Activo" : "Inactivo") << "\n";
                    } else {
                        vista.mostrarMensajeError("Producto no encontrado.");
                    }
                    break;
                }
                case 4: { // Desactivar producto
                    std::string codigo = vista.leerTexto("Ingrese el codigo del producto a desactivar");
                    modelo.desactivarProducto(codigo);
                    vista.mostrarMensajeExito("Producto desactivado correctamente.");
                    break;
                }
                case 0:
                    break;
                default:
                    vista.mostrarMensajeError("Opcion de productos invalida.");
            }
        } catch (const ErrorDominio& e) {
            vista.mostrarMensajeError(e.what());
        }
        if (opcionProd != 0) {
            vista.pausar();
        }
    } while (opcionProd != 0);
}