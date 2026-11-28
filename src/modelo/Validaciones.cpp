#include "Validaciones.hpp"
#include "ErrorDominio.hpp"
#include <algorithm>
#include <cctype>

namespace Validaciones {

std::string recortar(const std::string& texto) {
    size_t i = 0;
    size_t j = texto.size();
    while (i < j && std::isspace(static_cast<unsigned char>(texto[i]))) ++i;
    while (j > i && std::isspace(static_cast<unsigned char>(texto[j - 1]))) --j;
    return texto.substr(i, j - i);
}

std::string textoObligatorio(const std::string& texto, const std::string& campo) {
    std::string r = recortar(texto);
    if (r.empty()) {
        throw ErrorDominio("El campo '" + campo + "' es obligatorio.");
    }
    return r;
}

std::string aMinusculas(const std::string& texto) {
    std::string r = texto;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return r;
}

bool contiene(const std::string& texto, const std::string& fragmento) {
    return aMinusculas(texto).find(aMinusculas(fragmento)) != std::string::npos;
}

static bool soloDigitos(const std::string& s, size_t inicio, size_t cantidad) {
    for (size_t i = inicio; i < inicio + cantidad; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

static int diasDelMes(int anio, int mes) {
    static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
    return (mes == 2 && bisiesto) ? 29 : dias[mes - 1];
}

bool fechaValida(const std::string& f) {
    if (f.size() != 10 || f[4] != '-' || f[7] != '-') return false;
    if (!soloDigitos(f, 0, 4) || !soloDigitos(f, 5, 2) || !soloDigitos(f, 8, 2)) return false;
    int anio = std::stoi(f.substr(0, 4));
    int mes = std::stoi(f.substr(5, 2));
    int dia = std::stoi(f.substr(8, 2));
    if (anio < 1900 || anio > 2200 || mes < 1 || mes > 12) return false;
    return dia >= 1 && dia <= diasDelMes(anio, mes);
}

bool horaValida(const std::string& h) {
    if (h.size() != 5 || h[2] != ':') return false;
    if (!soloDigitos(h, 0, 2) || !soloDigitos(h, 3, 2)) return false;
    int hora = std::stoi(h.substr(0, 2));
    int minuto = std::stoi(h.substr(3, 2));
    return hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59;
}

bool anioMesValido(const std::string& m) {
    if (m.size() != 7 || m[4] != '-') return false;
    if (!soloDigitos(m, 0, 4) || !soloDigitos(m, 5, 2)) return false;
    int anio = std::stoi(m.substr(0, 4));
    int mes = std::stoi(m.substr(5, 2));
    return anio >= 1900 && anio <= 2200 && mes >= 1 && mes <= 12;
}

long diasDesdeEpoca(const std::string& fecha) {
    if (!fechaValida(fecha)) {
        throw ErrorDominio("Fecha invalida: '" + fecha + "'. Use el formato AAAA-MM-DD.");
    }
    int y = std::stoi(fecha.substr(0, 4));
    const int m = std::stoi(fecha.substr(5, 2));
    const int d = std::stoi(fecha.substr(8, 2));
    y -= (m <= 2) ? 1 : 0;
    const long era = (y >= 0 ? y : y - 399) / 400;
    const long yoe = y - era * 400;
    const long doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

std::string siguienteMes(const std::string& mes) {
    if (!anioMesValido(mes)) {
        throw ErrorDominio("Mes invalido: '" + mes + "'. Use el formato AAAA-MM.");
    }
    int anio = std::stoi(mes.substr(0, 4));
    int m = std::stoi(mes.substr(5, 2));
    if (m == 12) { anio += 1; m = 1; } else { m += 1; }
    std::string textoMes = std::to_string(m);
    if (textoMes.size() < 2) textoMes = "0" + textoMes;
    return std::to_string(anio) + "-" + textoMes;
}

} // namespace Validaciones
