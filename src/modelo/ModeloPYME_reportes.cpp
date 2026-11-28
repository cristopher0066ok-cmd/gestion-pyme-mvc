// Seccion REPORTES (y cierres RF-13) de ModeloPYME. Responsable: Limaico.
#include "ModeloPYME.hpp"

ResumenVentas ModeloPYME::generarResumen() const {
    ResumenVentas r;
    for (const auto& v : ventas) {
        if (!v.isConfirmada()) continue;
        r.cantidadVentas += 1;
        r.montoTotal += v.getTotal();
    }
    return r;
}

CierreDiario ModeloPYME::cierreDiario(const std::string& fecha) const {
    if (!Validaciones::fechaValida(fecha)) {
        throw ErrorDominio("La fecha del cierre debe tener el formato AAAA-MM-DD y ser valida.");
    }
    CierreDiario c;
    c.fecha = fecha;
    for (const auto& v : ventas) {
        if (!v.isConfirmada() || v.getFecha() != fecha) continue;
        c.cantidadVentas += 1;
        c.totalVendido += v.getTotal();
        if (v.getMetodoPago() == "efectivo") {
            c.efectivo += v.getTotal();
        } else {
            c.transferencia += v.getTotal();
            c.porBanco[v.getBanco()] += v.getTotal();
        }
    }
    return c;
}

// El saldo inicial de un mes es el saldo final del mes anterior. La cadena parte del mes base
// (cierre conocido, p. ej. el Excel de octubre). Sin mes base, el primer mes empieza en cero.
CierreMensual ModeloPYME::cierreMensual(const std::string& mes) const {
    if (!Validaciones::anioMesValido(mes)) {
        throw ErrorDominio("El mes debe tener el formato AAAA-MM.");
    }
    const std::string& base = configuracionCierres.mesBase;
    std::string actual;
    double caja = 0;
    double banco = 0;
    if (!base.empty()) {
        if (mes <= base) {
            throw ErrorDominio("El mes del cierre debe ser posterior al mes base (" + base + ").");
        }
        actual = Validaciones::siguienteMes(base);
        caja = configuracionCierres.saldoBaseCaja;
        banco = configuracionCierres.saldoBaseBanco;
    } else {
        actual = mes;
        for (const auto& v : ventas) {
            std::string mesVenta = v.getFecha().substr(0, 7);
            if (mesVenta < actual) actual = mesVenta;
        }
    }

    CierreMensual resultado;
    while (true) {
        CierreMensual c;
        c.mes = actual;
        c.saldoInicialCaja = caja;
        c.saldoInicialBanco = banco;
        for (const auto& v : ventas) {
            if (!v.isConfirmada() || v.getFecha().substr(0, 7) != actual) continue;
            if (v.getMetodoPago() == "efectivo") c.ventasEfectivo += v.getTotal();
            else c.ventasBanco += v.getTotal();
        }
        for (const auto& p : pendientes) {
            if (!p.isSaldado() || p.getSentido() != SentidoPendiente::LocalDebe || !p.esDinero()) continue;
            if (p.getFechaSaldado().substr(0, 7) != actual) continue;
            if (p.getMedioSaldado() == "efectivo") c.pagosEfectivo += p.getMonto();
            else c.pagosBanco += p.getMonto();
        }
        c.saldoFinalCaja = c.saldoInicialCaja + c.ventasEfectivo - c.pagosEfectivo;
        c.saldoFinalBanco = c.saldoInicialBanco + c.ventasBanco - c.pagosBanco;
        c.acumulado = c.saldoFinalCaja + c.saldoFinalBanco;
        caja = c.saldoFinalCaja;
        banco = c.saldoFinalBanco;
        resultado = c;
        if (actual == mes) break;
        actual = Validaciones::siguienteMes(actual);
    }
    return resultado;
}

void ModeloPYME::establecerSaldoBase(const std::string& mes, double caja, double banco) {
    if (!Validaciones::anioMesValido(mes)) {
        throw ErrorDominio("El mes base debe tener el formato AAAA-MM.");
    }
    if (caja < 0 || banco < 0) {
        throw ErrorDominio("Los saldos base no pueden ser negativos.");
    }
    configuracionCierres.mesBase = mes;
    configuracionCierres.saldoBaseCaja = caja;
    configuracionCierres.saldoBaseBanco = banco;
}

const ConfiguracionCierres& ModeloPYME::getConfiguracionCierres() const {
    return configuracionCierres;
}
