# Guía de defensa

## Recorrido que todos deben poder explicar: Vista → Controlador → Modelo → Controlador → Vista
Ejemplo: **registrar una venta** (diagrama: `secuencia_venta.png`).
1. `VistaConsola::mostrarMenuPrincipal` lee la opción 4 y devuelve un `int`. No decide nada del negocio.
2. `ControladorPYME::ejecutar` llama a `gestionarVentas` → `registrarVenta`. Pide a la Vista el cliente, los productos, las cantidades y el pago.
3. El Controlador llama a `ModeloPYME::procesarVenta(...)`. El Modelo valida **todo** sobre una copia de los productos (cliente existente, stock, producto activo, pago) y, solo si todo está bien, reemplaza el inventario y guarda la venta.
4. Si una regla falla, `Producto::vender` o `Venta::definirPago` lanzan `ErrorDominio`; el Controlador lo captura y pide a la Vista mostrar `[ERROR]`.
5. Si sale bien, el Controlador pide a la Vista `mostrarVenta` y guarda los datos.
**Dónde viven las reglas:** en el Modelo. La Vista solo comprueba que lo escrito sea un número; el Controlador no repite reglas.

## Preguntas probables por rol (respuesta corta)
| Rol | Archivos | Pregunta | Idea de respuesta |
|---|---|---|---|
| Coordinación | Makefile, README, main.cpp | ¿Cómo evitaron conflictos al integrar? | `ModeloPYME.hpp` por secciones y un `.cpp` por entidad; una rama por persona; integración de a una con `make` en cada paso |
| Productos | Producto.* | ¿Por qué `definirDatosComerciales` valida antes de asignar? | Para que un dato inválido no deje el producto a medias |
| Productos | Producto.* | ¿Qué pasa con una promoción si cambia el precio? | Si queda por encima del precio normal o por debajo de la compra, se quita |
| Clientes/proveedores | Cliente.*, Proveedor.* | ¿Quién asigna el id y cómo no se repite? | El Modelo con un contador; al cargar se recalcula como máximo + 1 |
| Clientes/proveedores | ModeloPYME_clientes.cpp | ¿Cómo detectan duplicados? | Por teléfono o correo no vacíos, ignorando mayúsculas en el correo |
| Ventas | Venta.*, ModeloPYME_ventas.cpp | ¿Cómo garantizan que no quede una venta a medias? | Se descuenta sobre una copia y se reemplaza solo si todo salió bien |
| Ventas | DetalleVenta.* | ¿Por qué el detalle guarda el precio? | Para que un cambio de precio posterior no altere ventas pasadas |
| Vista | VistaConsola.* | ¿Qué hace si escribo letras en un número? | Avisa y vuelve a preguntar; si la entrada se cierra lanza `FinDeEntrada` |
| Controlador | ControladorPYME.* | ¿Por qué el Controlador no usa `cout`? | Mostrar es trabajo de la Vista; así se respeta MVC |
| Persistencia | RepositorioArchivo.* | ¿Qué pasa si un CSV está dañado o no existe? | Se omiten las líneas dañadas con aviso, o se inicia vacío; el sistema sigue |
| Persistencia | RepositorioArchivo.* | ¿Por qué `;` y un archivo temporal? | Los nombres pueden llevar comas; el temporal evita archivos a medias |
| Calidad | docs/* | ¿Cómo comprueban el MVC? | `grep` de `cin`/`cout` en el Modelo y el Controlador; pruebas del Modelo sin la Vista |

## Demostración mínima (sección 13 del enunciado)
`make demo && make run` → 1) registrar un producto (menú 1), 2) una venta (menú 4: efectivo y vuelto), 3) una consulta
(menú 8: stock bajo) y 4) un caso rechazado (vender más que el stock). Luego cerrar, volver a abrir y mostrar que los datos siguen.
