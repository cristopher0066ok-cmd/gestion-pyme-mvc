#include "VistaConsola.hpp"
#include <iostream>
#include <limits>

VistaConsola::VistaConsola() {}

int VistaConsola::mostrarMenuPrincipal() {
    std::cout << "\n========================================\n";
    std::cout << "    SISTEMA DE GESTION DE PYMES (MVC)   \n";
    std::cout << "========================================\n";
    std::cout << "1. Gestion de Productos\n";
    std::cout << "2. Gestion de Clientes (Proximamente)\n";
    std::cout << "3. Gestion de Proveedores (Proximamente)\n";
    std::cout << "4. Registrar Venta (Proximamente)\n";
    std::cout << "5. Consultas y Reportes\n";
    std::cout << "0. Salir de la aplicacion\n";
    std::cout << "----------------------------------------\n";
    return leerEntero("Seleccione una opcion");
}

int VistaConsola::mostrarMenuProductos() {
    std::cout << "\n--- GESTION DE PRODUCTOS ---\n";
    std::cout << "1. Registrar nuevo producto\n";
    std::cout << "2. Listar productos\n";
    std::cout << "3. Buscar producto por codigo\n";
    std::cout << "4. Desactivar producto\n";
    std::cout << "0. Volver al menu principal\n";
    return leerEntero("Seleccione una opcion");
}

void VistaConsola::mostrarMensajeExito(const std::string& mensaje) const {
    std::cout << "\n[EXITO]: " << mensaje << "\n";
}

void VistaConsola::mostrarMensajeError(const std::string& mensaje) const {
    std::cerr << "\n[ERROR]: " << mensaje << "\n";
}

void VistaConsola::mostrarListaProductos(const std::vector<Producto>& productos) const {
    std::cout << "\n--- LISTADO DE PRODUCTOS ---\n";
    if (productos.empty()) {
        std::cout << "No hay productos registrados en el inventario.\n";
        return;
    }
    for (const auto& p : productos) {
        std::cout << "Codigo: " << p.getCodigo()
                  << " | Nombre: " << p.getNombre()
                  << " | Precio: $" << p.getPrecio()
                  << " | Stock: " << p.getStock()
                  << " | Min: " << p.getStockMinimo()
                  << " | Estado: " << (p.isActivo() ? "Activo" : "Inactivo") << "\n";
    }
}

std::string VistaConsola::leerTexto(const std::string& etiqueta) const {
    std::string valor;
    std::cout << etiqueta << ": ";
    std::cin >> ws; // Limpiar espacios previos
    std::getline(std::cin, valor);
    return valor;
}

double VistaConsola::leerDecimal(const std::string& etiqueta) const {
    double valor;
    while (true) {
        std::cout << etiqueta << ": ";
        if (std::cin >> valor) {
            return valor;
        } else {
            mostrarMensajeError("Entrada invalida. Por favor ingrese un numero decimal.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int VistaConsola::leerEntero(const std::string& etiqueta) const {
    int valor;
    while (true) {
        std::cout << etiqueta << ": ";
        if (std::cin >> valor) {
            return valor;
        } else {
            mostrarMensajeError("Entrada invalida. Por favor ingrese un numero entero.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void VistaConsola::pausar() const {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}