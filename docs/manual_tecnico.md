# Manual técnico · Sistema de gestión de PYMES

## 1. Arquitectura MVC
| Capa | Carpeta | Responsabilidad | No hace |
|---|---|---|---|
| Modelo | `src/modelo/` | Entidades, reglas de negocio, validaciones y persistencia CSV | Entrada/salida de consola; no conoce Vista ni Controlador |
| Vista | `src/vista/` | Menús, formularios, listados, mensajes y lectura segura | Reglas de negocio; no modifica el Modelo |
| Controlador | `src/controlador/` | Recibe opciones, llama al Modelo, pide a la Vista mostrar resultados | Imprimir, leer teclado o duplicar reglas |

Dependencias: Controlador → Modelo y Vista; Vista → entidades del Modelo (solo lectura de getters); Modelo → nadie.
`main.cpp` crea los tres objetos y llama a `controlador.ejecutar()`.

**Recorrido de una acción** (diagrama completo en `docs/secuencia_venta.png`): Vista lee la opción → Controlador la recibe y
pide datos a la Vista → Controlador llama a `ModeloPYME::procesarVenta` → el Modelo valida y devuelve la `Venta` (o lanza
`ErrorDominio`) → Controlador pide a la Vista mostrar el comprobante o el mensaje de error.

## 2. Clases y responsabilidades
| Clase | Atributos privados principales | Responsabilidad |
|---|---|---|
| `Producto` | codigo, nombre, precio, stock, stockMinimo, activo, categoria, unidadVenta, precioCompra, fechaCaducidad, idProveedor, variante, precioPromocion | Reglas de inventario: vender, ajustar, desactivar, reposición, datos comerciales, promoción |
| `Cliente` | id, nombre, telefono, correo | Validar contacto y actualizarlo |
| `Proveedor` | id, nombreComercial, nombreVendedor, telefono, correo | Igual que Cliente, con vendedor opcional |
| `DetalleVenta` | codigoProducto, cantidad, precioUnitario | Subtotal; guarda el precio vigente al vender |
| `Venta` | numero, fecha, hora, idCliente, detalles, total, metodoPago, banco, montoRecibido, vuelto, confirmada | Total, pago (RF-11) y confirmación |
| `Factura` / `DetalleFactura` | numero, claveAcceso, fecha, idProveedor, atendidoPor, rutaFoto / codigoProducto, cantidad, precioCompra | Factura de compra (RF-12) |
| `Pendiente` | id, idProveedor, sentido, monto, codigoProducto, cantidad, fecha, saldado | Deudas con proveedores (RF-14) |
| `ModeloPYME` | colecciones, contadores de id, repositorio | **Fachada del dominio**: registrar/buscar/listar, `procesarVenta`, facturas, cierres, reportes |
| `RepositorioArchivo` | directorio | Guardar y cargar CSV; no conoce a `ModeloPYME` |
| `Validaciones` | — | `recortar`, `textoObligatorio`, `fechaValida`, `horaValida`, fechas en días |
| `ErrorDominio` | — | Excepción (hereda de `std::runtime_error`) cuando se viola una regla |
| `VistaConsola` | — | Menús, listados, mensajes y lectura (`leerEntero`, `leerDecimal`, `leerTexto`, `solicitarConfirmacion`) |
| `ControladorPYME` | modelo&, vista& | `ejecutar()` y un método privado por módulo |

UML: `docs/uml_clases.png` (fuente Mermaid en `docs/uml_clases.mmd`).
Referencias por clave: `Venta` guarda `idCliente` y `DetalleVenta` guarda `codigoProducto`; así cada objeto conserva su
ciclo de vida y es sencillo guardarlo en CSV. Composición: `ModeloPYME` contiene las colecciones; `Venta` contiene sus detalles (1..*).

## 3. Decisiones de diseño
1. **Fachada `ModeloPYME`:** el Controlador no toca las entidades sueltas; la Vista no puede alterar las colecciones.
2. **Atomicidad:** `procesarVenta` y `registrarFactura` trabajan sobre una **copia** de los productos y la reemplazan solo si todo
   salió bien; si algo falla no cambia el stock ni se guarda nada. `editarProducto` y `editarCliente` también validan sobre copia.
3. **Errores de negocio con `ErrorDominio`:** el Modelo nunca imprime; el Controlador captura y la Vista muestra el mensaje.
4. **Precio vigente:** el detalle guarda el precio al confirmar (promoción si existe), por lo que un cambio posterior no altera ventas pasadas.
5. **Persistencia:** CSV con `;`, encabezado en la primera fila. Se escribe en un archivo temporal y se renombra para no dejar archivos
   a medias. Líneas dañadas se omiten con aviso; archivos inexistentes se informan y el sistema inicia vacío.
6. **Contadores:** al cargar, los ids siguientes se recalculan (máximo + 1), así no se repiten.
7. **Fin de entrada:** `VistaConsola` lanza `FinDeEntrada` si se cierra la entrada; el Controlador guarda y sale (sin bucle infinito).

## 4. Persistencia: archivos en `datos/`
| Archivo | Columnas |
|---|---|
| productos.csv | codigo;nombre;precio;stock;stockMinimo;activo;categoria;unidadVenta;precioCompra;fechaCaducidad;idProveedor;variante;precioPromocion |
| clientes.csv | id;nombre;telefono;correo |
| proveedores.csv | id;nombreComercial;nombreVendedor;telefono;correo |
| ventas.csv | numero;fecha;hora;idCliente;metodoPago;banco;montoRecibido;transferenciaRecibida;total |
| detalles_venta.csv | numeroVenta;codigoProducto;cantidad;precioUnitario |
| facturas.csv / detalles_factura.csv | numero;claveAcceso;fecha;idProveedor;atendidoPor;rutaFoto / idProveedor;numeroFactura;codigoProducto;cantidad;precioCompra |
| pendientes.csv | id;idProveedor;sentido;descripcion;monto;codigoProducto;cantidad;fecha;saldado;fechaSaldado;medio |
| config.csv | clave;valor (mesBase, saldoBaseCaja, saldoBaseBanco) |

Los `;` dentro de un texto se reemplazan por `,` al guardar.

## 5. Dónde vive cada regla de negocio
| Regla | Clase / método |
|---|---|
| Código único y obligatorio; nombre obligatorio | `ModeloPYME::registrarProducto`, `Producto::Producto` |
| Precio, precio de compra, stock y stock mínimo no negativos | `Producto` |
| Unidad solo «unidad» o «bulto»; caducidad AAAA-MM-DD válida | `Producto::definirDatosComerciales` |
| Reposición cuando stock ≤ mínimo | `Producto::requiereReposicion`, `ModeloPYME::productosConStockBajo` |
| Producto desactivado no se vende | `Producto::vender` |
| Contacto obligatorio, sin duplicados, id del sistema | `Cliente`, `Proveedor`, `ModeloPYME::verificarContactoUnico` |
| Venta con ≥1 detalle y cliente existente; cantidad positiva y ≤ stock | `ModeloPYME::procesarVenta`, `Producto::vender` |
| Subtotal y total; no queda nada a medias | `DetalleVenta`, `Venta`, `procesarVenta` (copia + reemplazo) |
| Efectivo: monto ≥ total y vuelto; transferencia: banco y recibida | `Venta::definirPago` |
| Factura: proveedor existente, no repetida, detalles válidos | `ModeloPYME::registrarFactura` |
| Saldo inicial del mes = saldo final del anterior | `ModeloPYME::cierreMensual` |
| Pendiente: proveedor existente, monto o cantidad positivos | `Pendiente`, `ModeloPYME::registrarPendiente` |
| Promoción entre precio de compra y precio normal, solo activos | `Producto::asignarPromocion` |

## 6. Supuestos asumidos (confirmar con el Product Owner)
- *Acumulado* del cierre mensual = saldo final de caja + saldo final de banco.
- El cierre mensual resta solo los pagos de RF-14 saldados dentro del mes (no las compras por factura, tal como indica el enunciado).
- «Próximos N días» de caducidad incluye hoy y excluye los ya caducados. N se ingresa al consultar.
- Una venta usa el precio de promoción si el producto lo tiene.
- Sin mes base definido, el primer cierre mensual parte de saldo cero.

## 7. Pruebas
`make pruebas` compila y ejecuta cuatro programas de prueba del Modelo (`pruebas/prueba_*.cpp`, 207 comprobaciones) que no usan
la Vista ni entrada/salida de consola. Matriz: `docs/matriz_pruebas.md`. Evidencias de la consola: `docs/evidencias/`.
