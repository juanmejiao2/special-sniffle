#include "enemigos.h"
#include <algorithm>
#include <cmath>

enemigos::enemigos(): x(0.0), y(0.0), ancho(24.0), alto(48.0), vivo(true){
}

enemigos::enemigos(double posicionX, double posicionY, double w, double altura): x(posicionX), y(posicionY), ancho(w), alto(altura), vivo(true){
}

double enemigos::GETX() const {
    return x;
}

double enemigos::GETY() const {
    return y;
}

double enemigos::GETAncho() const {
    return ancho;
}

double enemigos::GETAlto() const {
    return alto;
}

bool enemigos::EstaVivo() const {
    return vivo;
}

void enemigos::RecibirImpacto() {
    vivo = false;
}

bool enemigos::ColisionaConPunto(double px, double py, double radio) const {
    if (!vivo) return false;
    double XMasCercano= std::clamp(px, x, x+ancho);
    double YMasCercano= std::clamp(py, y, y+alto);
    double dx= px-XMasCercano;
    double dy= py-YMasCercano;
    return (dx*dx+dy*dy)<=(radio*radio);
}