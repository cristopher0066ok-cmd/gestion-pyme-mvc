# Gestión de PYMES · Chip Chop Pet Shop

Aplicación de consola en **C++17** con patrón **MVC** exclusivo. Proyecto de Programación Orientada a Objetos
(UPS, Computación, segundo nivel). Entrega y defensa: lunes 30 de noviembre de 2026.

Permite administrar **productos, clientes, proveedores y ventas**; controla el inventario, registra facturas de
compra, deudas con proveedores y cierres de caja, genera reportes y guarda los datos en archivos CSV.

## Compilar y ejecutar
Requisito: `g++` con soporte C++17 y `make`.
```
make            # compila todo y genera ./pyme (sin advertencias con -Wall -Wextra -pedantic)
make run        # compila y ejecuta
make demo       # copia los datos de demostración (ficticios) a datos/
make pruebas    # compila y corre las pruebas del Modelo (207 comprobaciones)
make clean      # borra lo generado
```
Sin `make`: `g++ -std=c++17 -Wall -Wextra -pedantic -Isrc src/main.cpp src/modelo/*.cpp src/vista/*.cpp src/controlador/*.cpp -o gestion_pyme`

**Primera demostración rápida:** `make demo && make run`.

## Estructura
```
src/modelo/        Entidades, reglas de negocio y persistencia CSV (sin entrada/salida de consola)
src/vista/         VistaConsola: único lugar que usa cin/cout
src/controlador/   ControladorPYME: conecta Vista y Modelo
src/main.cpp       Crea Modelo, Vista y Controlador y arranca
pruebas/           Pruebas del Modelo (prueba_*.cpp), sin entrada/salida de consola
datos/             Archivos CSV generados al usar el programa; datos/demo/ trae datos de ejemplo
docs/              UML, manuales, matriz de pruebas, evidencias, contribuciones, anexo de IA
scripts/           Utilidades (crear_ramas.sh)
```

## Reglas del MVC
- **Modelo:** no usa `cin`/`cout`, no conoce la Vista ni el Controlador. Las reglas violadas lanzan `ErrorDominio`.
- **Vista:** solo muestra y lee; valida el formato (que sea número), nunca reglas de negocio.
- **Controlador:** no imprime ni repite reglas: pide al Modelo y manda a mostrar a la Vista.

## Requisitos cubiertos
RF-01 a RF-09 del enunciado base y RF-10 a RF-15 del Anexo A (atributos ampliados del producto, hora y método de pago,
facturas de compra, cierre diario y mensual, deudas y pendientes con proveedores, promociones por caducidad).
Ver `docs/matriz_pruebas.md` y `docs/checklist_entrega.md`.

## Documentación
`docs/manual_usuario.md` · `docs/manual_tecnico.md` · `docs/uml_clases.png` · `docs/secuencia_venta.png` ·
`docs/matriz_pruebas.md` · `docs/evidencias/` · `docs/contribuciones.md` · `docs/anexo_ia.md` · `docs/guia_defensa.md`

## Flujo de ramas
`git checkout dev/<tu-nombre>` → commits pequeños → `make` y `make pruebas` en verde → `git push origin dev/<tu-nombre>`.
Solo la coordinación integra en `main`.
