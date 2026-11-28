# Anexo: uso de IA generativa

El enunciado (sección 14) exige registrar la herramienta usada, su propósito, las partes incorporadas y los cambios o verificaciones realizados.

| Campo | Detalle |
|---|---|
| Herramienta | Claude (Anthropic), asistente de IA generativa, usado por el coordinador del equipo (Cristopher Quisilema) |
| Propósito | Completar la implementación de los módulos pendientes, escribir pruebas, generar diagramas UML y redactar la documentación técnica y de usuario |
| Partes incorporadas | Clases del Modelo (`Validaciones`, ampliación de `Producto`, `Cliente`, `Proveedor`, `DetalleVenta`, `Venta`, `Factura`, `Pendiente`, `Reportes`, `RepositorioArchivo`, secciones de `ModeloPYME`); ampliación de `VistaConsola` y `ControladorPYME`; datos de demostración ficticios; pruebas `pruebas/*.cpp`; `Makefile`; diagramas Mermaid; documentos de `docs/` |
| Base previa del equipo | Estructura MVC, `Producto`, `ErrorDominio`, `ModeloPYME` básico, `VistaConsola` y `ControladorPYME` iniciales (commits de Cristopher Quisilema) |
| Verificaciones realizadas | Compilación con `-std=c++17 -Wall -Wextra -pedantic` sin advertencias; 207 comprobaciones automáticas del Modelo; sesiones de consola guionadas (`docs/evidencias/`); búsqueda de `cin/cout` en el Modelo y el Controlador (sin coincidencias); revisión de los diagramas generados |
| Cambios hechos por el equipo | *(completar: qué revisó, cambió o corrigió cada integrante después de recibir el código)* |
| Límites conocidos | Los supuestos de `manual_tecnico.md` (sección 6) deben confirmarse con el Product Owner. Las fotos de facturas no se procesan. No se probó en Windows |

Ningún dato personal real se incluye: los clientes, proveedores y saldos de `datos/demo/` son ficticios.
El equipo es responsable de comprender y poder explicar todo el código en la defensa.
