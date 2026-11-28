#ifndef TIEMPO_HPP
#define TIEMPO_HPP

#include <ctime>
#include <string>

// Fecha y hora actuales como texto (AAAA-MM-DD y HH:MM). Sin entrada/salida de consola.
namespace Tiempo {

inline std::string formatear(const char* formato) {
    std::time_t ahora = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof buffer, formato, std::localtime(&ahora));
    return buffer;
}

inline std::string fechaActual() { return formatear("%Y-%m-%d"); }
inline std::string horaActual() { return formatear("%H:%M"); }

} // namespace Tiempo

#endif // TIEMPO_HPP
