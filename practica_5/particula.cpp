#include "particula.h"

particula::particula(): nombre("P"), x(0.0), y(0.0), vx(0.0), vy(0.0), masa(1.0), radio(10.0) {
}

particula::particula(QString id, double posX, double posY, double velocidadX,
                     double velocidadY, double masaInicial, double radioInicial):
    nombre(id), x(posX), y(posY), vx(velocidadX), vy(velocidadY),
    masa(masaInicial), radio(radioInicial) {
}

QString particula::GETNombre() const { return nombre; }
double particula::GETX() const { return x; }
double particula::GETY() const { return y; }
double particula::GETVx() const { return vx; }
double particula::GETVy() const { return vy; }
double particula::GETMasa() const { return masa; }
double particula::GETRadio() const { return radio; }

void particula::actualizarPaso(double dt) {
    x += vx * dt;
    y += vy * dt;
}

void particula::fijarPosicion(double nuevaX, double nuevaY) {
    x = nuevaX;
    y = nuevaY;
}

void particula::fijarVelocidad(double nuevaVx, double nuevaVy) {
    vx = nuevaVx;
    vy = nuevaVy;
}
