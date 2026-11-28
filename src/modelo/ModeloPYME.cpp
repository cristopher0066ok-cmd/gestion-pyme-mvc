#include "ModeloPYME.hpp"

// Constructor comun. La logica de cada entidad vive en su propio archivo:
//   ModeloPYME_productos.cpp, _clientes.cpp, _proveedores.cpp,
//   _ventas.cpp, _persistencia.cpp, _reportes.cpp
ModeloPYME::ModeloPYME(const std::string& directorioDatos)
    : siguienteIdCliente(1), siguienteIdProveedor(1), siguienteIdPendiente(1),
      siguienteNumeroVenta(1), repositorio(directorioDatos) {
}

int ModeloPYME::indiceProducto(const std::string& codigo) const {
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].getCodigo() == codigo) return static_cast<int>(i);
    }
    return -1;
}
