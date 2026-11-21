# Gestión de PYMES · Chip Chop Pet Shop

Aplicación de consola en C++17 con patrón **MVC**. Proyecto de Programación Orientada a Objetos.
Entrega y defensa: lunes 30 de noviembre de 2026.

## Compilar y ejecutar
```
make          # compila todo y genera ./pyme
make run      # compila y ejecuta
make pruebas  # compila y corre pruebas/prueba_*.cpp
make clean    # borra lo generado
```
Requisito: g++ con soporte C++17. Se compila con `-Wall -Wextra -pedantic` y no debe haber advertencias.

## Estructura
```
src/modelo/        Entidades y reglas de negocio (sin cin/cout). Fachada: ModeloPYME
src/vista/         VistaConsola: único lugar que usa cin/cout
src/controlador/   ControladorPYME: conecta Vista y Modelo
src/main.cpp       Punto de entrada
pruebas/           Pruebas del Modelo (prueba_*.cpp, sin cin/cout)
datos/             Archivos CSV y datos de demostración
scripts/           Utilidades (crear_ramas.sh)
```

## Reglas del MVC
- El **Modelo** no usa `cin`/`cout`, no conoce la Vista ni el Controlador. Las reglas violadas lanzan `ErrorDominio`.
- La **Vista** solo muestra y lee; valida formato (que sea número), nunca reglas de negocio.
- El **Controlador** no imprime y no repite reglas: pide al Modelo y manda a mostrar a la Vista.

## Quién edita qué
| Rol | Persona | Archivos |
|---|---|---|
| Coordinación | Cristopher | Makefile, README, main.cpp, integración |
| Productos | Mateo | Producto.*, ModeloPYME_productos.cpp, Validaciones.hpp |
| Clientes y proveedores | Martin | Cliente.*, Proveedor.*, ModeloPYME_clientes.cpp, _proveedores.cpp |
| Ventas | Sebastián | Venta.*, DetalleVenta.*, Tiempo.hpp, ModeloPYME_ventas.cpp |
| Vista | Jhon | src/vista/* |
| Controlador | Luis | src/controlador/* |
| Persistencia y reportes | Limaico | RepositorioArchivo.*, ModeloPYME_persistencia.cpp, _reportes.cpp, datos/ |
| Calidad y PO | Ariel | matriz de pruebas, UML, manual |

`ModeloPYME.hpp` está dividido en secciones con marcadores `INICIO/FIN`: **cada uno edita solo su sección**.

## Flujo de ramas
1. `git fetch && git checkout dev/<tu-nombre>` (si no existe: `git checkout -b dev/<tu-nombre>`).
2. Trabaja y haz commits pequeños con mensaje claro.
3. Antes de subir: `make` sin advertencias y `make pruebas` en verde.
4. `git push origin dev/<tu-nombre>`. **Nadie sube directo a `main`**: Cristopher integra.
