// Pruebas del Modelo: clientes y proveedores (RF-02, RF-03, RF-08) y pendientes (RF-14).
#include "mini_test.hpp"
#include "modelo/ModeloPYME.hpp"

int main() {
    ModeloPYME m("build/datos_prueba_cp");

    mini::seccion("Cliente valido recibe id y aparece en el listado");
    int id1 = m.registrarCliente("Ana Perez", "0990000001", "");
    int id2 = m.registrarCliente("Luis Mora", "", "luis@example.com");
    COMPROBAR(id1 == 1 && id2 == 2);
    COMPROBAR(m.getClientes().size() == 2);
    COMPROBAR(m.buscarClientePorId(1) != nullptr && m.buscarClientePorId(2) != nullptr);

    mini::seccion("Buscar inexistente da resultado claro sin romper");
    COMPROBAR(m.buscarClientePorId(99) == nullptr);
    COMPROBAR(m.buscarClientes("zzz").empty());
    COMPROBAR(m.buscarClientes("ana").size() == 1);
    COMPROBAR(m.buscarClientes("example.com").size() == 1);

    mini::seccion("Validaciones: nombre y contacto obligatorios");
    COMPROBAR_ERROR(m.registrarCliente("", "0990000009", ""));
    COMPROBAR_ERROR(m.registrarCliente("Sin Contacto", "", ""));
    COMPROBAR_ERROR(m.registrarCliente("Sin Contacto", "   ", "   "));
    COMPROBAR(m.getClientes().size() == 2);

    mini::seccion("Duplicados por telefono o correo se rechazan; ids no se reutilizan");
    COMPROBAR_ERROR(m.registrarCliente("Otro", "0990000001", ""));
    COMPROBAR_ERROR(m.registrarCliente("Otro", "", "LUIS@example.com"));
    COMPROBAR(m.getClientes().size() == 2);
    COMPROBAR(m.registrarCliente("Tercero", "0990000003", "") == 3);

    mini::seccion("Editar contacto valida duplicados y conserva el original si falla");
    COMPROBAR_ERROR(m.editarCliente(1, "0990000003", ""));       // ya lo tiene el cliente 3
    COMPROBAR(m.buscarClientePorId(1)->getTelefono() == "0990000001");
    COMPROBAR_ERROR(m.editarCliente(1, "", ""));
    COMPROBAR_NO_ERROR(m.editarCliente(1, "0990000100", "ana@example.com"));
    COMPROBAR(m.buscarClientePorId(1)->getCorreo() == "ana@example.com");
    COMPROBAR_ERROR(m.editarCliente(99, "1", ""));

    mini::seccion("Proveedor: nombre del vendedor opcional, contacto obligatorio, duplicados");
    int p1 = m.registrarProveedor("Distribuidora Mascotas", "Carlos", "0981111111", "");
    int p2 = m.registrarProveedor("Agro Pet", "", "", "ventas@agropet.example");
    COMPROBAR(p1 == 1 && p2 == 2 && m.getProveedores().size() == 2);
    COMPROBAR(m.buscarProveedorPorId(1)->getNombreVendedor() == "Carlos");
    COMPROBAR_ERROR(m.registrarProveedor("", "x", "0982222222", ""));
    COMPROBAR_ERROR(m.registrarProveedor("Sin contacto", "x", "", ""));
    COMPROBAR_ERROR(m.registrarProveedor("Duplicado", "", "0981111111", ""));
    COMPROBAR(m.buscarProveedorPorId(99) == nullptr);
    COMPROBAR(m.buscarProveedores("agro").size() == 1);
    COMPROBAR_NO_ERROR(m.editarProveedor(2, "Marta", "0983333333", "ventas@agropet.example"));
    COMPROBAR(m.buscarProveedorPorId(2)->getNombreVendedor() == "Marta");

    mini::seccion("RF-14: pendientes abiertos, saldar y resaltado por dias");
    int d1 = m.registrarPendiente(1, SentidoPendiente::LocalDebe, "Factura pendiente", 100.0, "", 0, "2026-11-01");
    int d2 = m.registrarPendiente(1, SentidoPendiente::ProveedorDebe, "No entrego croquetas", 0, "P001", 5, "2026-11-25");
    COMPROBAR(d1 == 1 && d2 == 2);
    COMPROBAR(m.pendientesAbiertos().size() == 2);
    auto vencidos = m.pendientesVencidos("2026-11-28", 7);
    COMPROBAR(vencidos.size() == 1 && vencidos[0].getId() == 1);
    COMPROBAR_ERROR(m.pendientesVencidos("2026-11-28", -1));
    COMPROBAR_ERROR(m.registrarPendiente(99, SentidoPendiente::LocalDebe, "x", 5.0, "", 0, "2026-11-01")); // proveedor inexistente
    COMPROBAR_ERROR(m.registrarPendiente(1, SentidoPendiente::LocalDebe, "x", 0.0, "", 0, "2026-11-01"));  // ni dinero ni producto
    COMPROBAR_ERROR(m.registrarPendiente(1, SentidoPendiente::LocalDebe, "x", -5.0, "", 0, "2026-11-01"));
    COMPROBAR_ERROR(m.registrarPendiente(1, SentidoPendiente::ProveedorDebe, "x", 0, "P001", 0, "2026-11-01"));
    COMPROBAR_ERROR(m.registrarPendiente(1, SentidoPendiente::LocalDebe, "x", 5.0, "", 0, "01-11-2026"));
    COMPROBAR_ERROR(m.saldarPendiente(1, "2026-11-28", ""));      // deuda del local: exige medio
    COMPROBAR(m.pendientesAbiertos().size() == 2);
    COMPROBAR_NO_ERROR(m.saldarPendiente(1, "2026-11-28", "efectivo"));
    COMPROBAR(m.pendientesAbiertos().size() == 1 && m.pendientesAbiertos()[0].getId() == 2);
    COMPROBAR_ERROR(m.saldarPendiente(1, "2026-11-28", "efectivo")); // ya saldado
    COMPROBAR_ERROR(m.saldarPendiente(99, "2026-11-28", "efectivo"));
    return mini::resumen();
}
