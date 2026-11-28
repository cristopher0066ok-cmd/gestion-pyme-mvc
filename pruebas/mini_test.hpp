#ifndef MINI_TEST_HPP
#define MINI_TEST_HPP

// Mini marco de pruebas para el Modelo. No usa entrada/salida de iostream: solo printf.
#include <cstdio>
#include <string>
#include "modelo/ErrorDominio.hpp"

namespace mini {
inline int total = 0;
inline int fallos = 0;
inline const char* seccionActual = "";

inline void registrar(bool ok, const char* descripcion, const char* archivo, int linea) {
    ++total;
    if (!ok) {
        ++fallos;
        std::printf("  [FALLA] %s (%s:%d)\n", descripcion, archivo, linea);
    }
}
inline void seccion(const char* nombre) {
    seccionActual = nombre;
    std::printf("- %s\n", nombre);
}
inline int resumen() {
    std::printf("== %d comprobaciones, %d fallas\n", total, fallos);
    return fallos == 0 ? 0 : 1;
}
} // namespace mini

#define COMPROBAR(cond) mini::registrar((cond), #cond, __FILE__, __LINE__)
// Comprueba que la expresion lanza ErrorDominio (la regla de negocio se hizo cumplir).
#define COMPROBAR_ERROR(expr)                                                         \
    do {                                                                              \
        bool lanzo = false;                                                           \
        try { expr; } catch (const ErrorDominio&) { lanzo = true; }                   \
        mini::registrar(lanzo, "debe lanzar ErrorDominio: " #expr, __FILE__, __LINE__); \
    } while (0)
#define COMPROBAR_NO_ERROR(expr)                                                      \
    do {                                                                              \
        bool lanzo = false;                                                           \
        try { expr; } catch (const ErrorDominio&) { lanzo = true; }                   \
        mini::registrar(!lanzo, "no debe lanzar ErrorDominio: " #expr, __FILE__, __LINE__); \
    } while (0)
#define CERCA(a, b) (((a) - (b)) < 0.0001 && ((b) - (a)) < 0.0001)

#endif // MINI_TEST_HPP
