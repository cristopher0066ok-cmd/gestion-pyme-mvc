#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <string>

// Funciones de utilidad del dominio (sin estado). No hacen entrada/salida.
namespace Validaciones {

std::string recortar(const std::string& texto);
// Devuelve el texto recortado o lanza ErrorDominio si queda vacio.
std::string textoObligatorio(const std::string& texto, const std::string& campo);
std::string aMinusculas(const std::string& texto);
bool contiene(const std::string& texto, const std::string& fragmento); // sin distinguir mayusculas

bool fechaValida(const std::string& fecha);   // AAAA-MM-DD, fecha real
bool horaValida(const std::string& hora);     // HH:MM
bool anioMesValido(const std::string& mes);  // AAAA-MM
long diasDesdeEpoca(const std::string& fecha); // lanza ErrorDominio si la fecha no es valida
std::string siguienteMes(const std::string& mes); // "2026-12" -> "2027-01"

} // namespace Validaciones

#endif // VALIDACIONES_HPP
