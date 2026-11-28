# Lista de control de entrega (Anexo A del enunciado)

| ✔ | Elemento | Estado | Cómo comprobarlo |
|---|---|---|---|
| ☑ | Compila con C++17 y las instrucciones del manual | Verificado el 28-nov (g++ 13.3, 0 advertencias) | `make clean && make` |
| ☑ | MVC exclusivo comprobado; Modelo sin entrada/salida de consola | Verificado | `grep -rnE "iostream\|cin\|cout" src/modelo src/controlador` sin coincidencias |
| ☐ | UML actualizado y consistente con el código | Generado: `docs/uml_clases.png` y `docs/secuencia_venta.png` | Ariel revisa contra el código |
| ☑ | Requisitos funcionales probados y matriz de pruebas completa | 34 casos, todos aprobados | `make pruebas` y `docs/matriz_pruebas.md` |
| ☑ | Persistencia verificada en una ejecución nueva | Verificado | `docs/evidencias/11_…` y `prueba_persistencia` |
| ☑ | Manual técnico y manual de usuario incluidos | Redactados | `docs/manual_*.md` |
| ☐ | Evidencias y presentación incluidas | Evidencias de consola listas; falta la presentación (PDF o PPTX) y, si se desea, capturas de pantalla reales | Ariel / Jhon |
| ☐ | Roles y contribuciones de los ocho estudiantes documentados | Plantilla lista; **cada integrante debe completar su fila** | `docs/contribuciones.md` |
| ☐ | Anexo de IA generativa incluido | Redactado; completar «Cambios hechos por el equipo» | `docs/anexo_ia.md` |
| ☐ | Paquete final probado en copia limpia y listo para el 30-nov | Hacer el 29-nov | `git clone` en otra carpeta → `make` → `make pruebas` → `make demo && make run` |

Nombre recomendado del archivo: `POO2_PYME_MVC_GrupoXX_2026-11-30.zip`.
Contenido: código fuente (`src/`), `Makefile`, `README.md`, `pruebas/`, `datos/demo/`, `docs/`. Sin binarios ni la carpeta `build/`.
Formato del informe (si el docente lo pide): PDF A4, Arial 10–11, interlineado 1,15, portada institucional, referencias APA 7.
