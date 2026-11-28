# Manual de usuario · Sistema de gestión de PYMES (Chip Chop Pet Shop)

## 1. Requisitos e instalación
- Computador con `g++` (C++17) y `make`. Funciona en Linux, macOS y Windows (MinGW o WSL).
- Descomprima el paquete, abra una terminal en la carpeta del proyecto y ejecute:
  ```
  make
  ```
  Se crea el programa `pyme` (sin advertencias).
- Para probar con datos de ejemplo (ficticios): `make demo` y luego `make run`.
- Para empezar desde cero, borre los archivos `datos/*.csv` (la carpeta `datos/demo/` queda intacta).

## 2. Cómo se usa
Al iniciar, el sistema carga los datos de la carpeta `datos/`. Si no existen, avisa y arranca vacío.
Todas las opciones se eligen escribiendo el **número** y pulsando **Enter**. En cada menú, **0** vuelve o sale.
Cada operación indica `[EXITO]` o `[ERROR]` con el motivo. Los datos se guardan automáticamente después de cada
cambio y también al salir.

Si escribe letras donde se espera un número, el sistema lo avisa y vuelve a preguntar. Los decimales se pueden
escribir con punto o con coma (12.50 o 12,50). Las fechas usan el formato `AAAA-MM-DD` y los meses `AAAA-MM`.

### Menú principal
1 Productos · 2 Clientes · 3 Proveedores · 4 Ventas · 5 Compras (facturas) · 6 Deudas y pendientes · 7 Cierres de caja · 8 Reportes · 0 Salir

## 3. Operaciones

### Productos (menú 1)
- **Registrar:** código (único), nombre, precio, stock inicial, stock mínimo, categoría, unidad de venta
  (`unidad` o `bulto`), precio de compra, fecha de caducidad (opcional), ID del proveedor (0 si no tiene) y variante (opcional).
- **Listar / Buscar:** busque por código o por parte del nombre. Un solo resultado se muestra completo.
- **Editar:** se muestran los datos actuales y se ingresan los nuevos (el código no cambia).
- **Desactivar:** el producto queda como *Inactivo*, sigue en la lista y conserva su historial, pero no se puede vender.
- **Productos por caducar:** indique N y verá los que caducan dentro de los próximos N días.
- **Precio de promoción:** no puede ser menor al precio de compra ni mayor al precio normal. Se usa al vender.

### Clientes (menú 2) y Proveedores (menú 3)
Registrar (el sistema asigna el ID), listar, buscar por nombre, teléfono o correo y editar el contacto.
Se necesita nombre y al menos un medio de contacto (teléfono o correo). No se permiten teléfonos ni correos repetidos.
En proveedores se puede anotar el nombre de quien atiende.

### Ventas (menú 4)
1. Ingrese el ID del cliente (debe existir).
2. Agregue productos con su cantidad; verá el total parcial y puede agregar más.
3. Elija el método de pago: **efectivo** (escriba el monto recibido; se calcula el vuelto) o
   **transferencia** (escriba el banco y confirme que fue recibida).
4. Confirme. Se descuenta el stock y se muestra el comprobante con subtotales, total, hora y pago.

Si algo falla (stock insuficiente, producto inactivo, pago incompleto) la venta **no se guarda** y el stock no cambia.

### Compras: facturas de proveedores (menú 5)
Registre la factura (número, clave de acceso, fecha, proveedor, quién atendió, ruta de la foto) y sus productos con
cantidad y precio de compra. Al confirmar sube el stock y se actualiza el precio de compra. No se repite la combinación
proveedor + número. Puede buscar por proveedor, rango de fechas, número o producto.

### Deudas y pendientes con proveedores (menú 6)
Registre lo que el local le debe a un proveedor o lo que el proveedor le debe al local (dinero o producto no entregado).
En el listado de abiertos indique N: los que superan N días aparecen marcados con **(!)** para reclamarlos a tiempo.
Al saldar una deuda en dinero del local, indique si se pagó en efectivo o banco.

### Cierres de caja (menú 7)
- **Diario:** efectivo, transferencias por banco y total vendido de una fecha (Enter = hoy).
- **Mensual:** saldo inicial (cierre del mes anterior) + ventas − pagos a proveedores = saldo final.
  La primera vez defina el **saldo base** (opción 3): el mes anterior al primer cierre con sus saldos de caja y banco.

### Reportes (menú 8)
Inventario disponible, productos con stock bajo (stock menor o igual al mínimo) y resumen de ventas (cantidad y monto total).

## 4. Limitaciones conocidas
- Aplicación de consola académica: no es un sistema contable ni tributario y no procesa pagos reales.
- La foto de la factura solo se guarda como ruta de archivo (no se lee ni se muestra).
- No hay notificaciones al celular ni interfaz gráfica (fuera de alcance acordado).
- Los textos de la consola no llevan tildes para evitar problemas de codificación en distintas terminales.
- Los valores de N días (promociones y recordatorio de pendientes) se ingresan en cada consulta.
- Una venta siempre requiere un cliente registrado (la decisión sobre un cliente «Consumidor final» sigue pendiente del Product Owner; en los datos de demostración existe como cliente normal).
