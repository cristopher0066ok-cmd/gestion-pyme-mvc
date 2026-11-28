#ifndef REPORTES_HPP
#define REPORTES_HPP

#include <map>
#include <string>

// Resultados de reportes y cierres (RF-06 y RF-13). Son datos simples; la Vista solo los muestra.
struct ResumenVentas {
    int cantidadVentas = 0;
    double montoTotal = 0.0;
};

struct CierreDiario {
    std::string fecha;
    int cantidadVentas = 0;
    double efectivo = 0.0;
    double transferencia = 0.0;
    std::map<std::string, double> porBanco;
    double totalVendido = 0.0;
};

struct CierreMensual {
    std::string mes;
    double saldoInicialCaja = 0.0;
    double saldoInicialBanco = 0.0;
    double ventasEfectivo = 0.0;
    double ventasBanco = 0.0;
    double pagosEfectivo = 0.0;  // pagos de RF-14 saldados en el mes
    double pagosBanco = 0.0;
    double saldoFinalCaja = 0.0;
    double saldoFinalBanco = 0.0;
    double acumulado = 0.0;      // saldo final de caja + banco
};

#endif // REPORTES_HPP
