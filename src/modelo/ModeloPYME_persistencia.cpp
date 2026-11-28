// Seccion PERSISTENCIA de ModeloPYME. Responsable: Limaico.
#include "ModeloPYME.hpp"

namespace {
template <typename Coleccion, typename ObtenerId>
int siguienteId(const Coleccion& coleccion, ObtenerId obtener) {
    int mayor = 0;
    for (const auto& x : coleccion) {
        if (obtener(x) > mayor) mayor = obtener(x);
    }
    return mayor + 1;
}
}

void ModeloPYME::guardarDatos() {
    repositorio.guardarProveedores(proveedores);
    repositorio.guardarClientes(clientes);
    repositorio.guardarProductos(productos);
    repositorio.guardarVentas(ventas);
    repositorio.guardarFacturas(facturas);
    repositorio.guardarPendientes(pendientes);
    repositorio.guardarConfiguracion(configuracionCierres);
}

// Carga todo; un archivo inexistente o una linea danada solo generan avisos, el sistema sigue.
std::vector<std::string> ModeloPYME::cargarDatos() {
    std::vector<std::string> avisos;

    std::vector<std::string> faltantes;
    for (const auto& nombre : RepositorioArchivo::nombresArchivos()) {
        if (!repositorio.existeArchivo(nombre)) faltantes.push_back(nombre);
    }
    if (faltantes.size() == RepositorioArchivo::nombresArchivos().size()) {
        avisos.push_back("No hay datos guardados todavia en '" + repositorio.getDirectorio() +
                         "'. El sistema inicia vacio.");
    } else if (!faltantes.empty()) {
        std::string lista;
        for (size_t i = 0; i < faltantes.size(); ++i) lista += (i ? ", " : "") + faltantes[i];
        avisos.push_back("Archivos no encontrados (se usan datos vacios): " + lista + ".");
    }

    proveedores = repositorio.cargarProveedores(avisos);
    clientes = repositorio.cargarClientes(avisos);
    productos = repositorio.cargarProductos(avisos);
    ventas = repositorio.cargarVentas(avisos);
    facturas = repositorio.cargarFacturas(avisos);
    pendientes = repositorio.cargarPendientes(avisos);
    configuracionCierres = repositorio.cargarConfiguracion(avisos);

    siguienteIdProveedor = siguienteId(proveedores, [](const Proveedor& p) { return p.getId(); });
    siguienteIdCliente = siguienteId(clientes, [](const Cliente& c) { return c.getId(); });
    siguienteNumeroVenta = siguienteId(ventas, [](const Venta& v) { return v.getNumero(); });
    siguienteIdPendiente = siguienteId(pendientes, [](const Pendiente& p) { return p.getId(); });
    return avisos;
}
