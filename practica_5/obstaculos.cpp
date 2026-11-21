#include "obstaculos.h"

obstaculos::obstaculos(): x(0.0), y(0.0), ancho(40.0), Resistencia(100.0), CoeficienteRestitucion(0.6) {
}

obstaculos::obstaculos(double posX, double posY, double w, double h, double resInicial, double e): x(posX), y(posY), ancho(w), Alto(h),Resistencia(resInicial), CoeficienteRestitucion(e) {
}

double obstaculos::GETX() const {
    return x;
}
double obstaculos::GETY() const {
    return y;
}
double obstaculos::GETAncho() const {
    return ancho;
}
double obstaculos::GETAlto() const {
    return Alto;
}
double obstaculos::GETResistencia() const {
    return Resistencia;
}
double obstaculos::GETCoefRestitucion() const {
    return CoeficienteRestitucion;
}

bool obstaculos::estaDestruido() const {
    return Resistencia <= 0.0;
}

void obstaculos::aplicarDano(double dano) {
    if (dano > 0.0 && !estaDestruido()) {
        Resistencia -= dano;
        if (Resistencia < 0.0) {
            Resistencia = 0.0;
        }
    }
}