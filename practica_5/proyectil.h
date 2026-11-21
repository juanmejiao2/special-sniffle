#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "obstaculos.h"

class proyectil{
private:
    double x;
    double y;
    double vx;
    double vy;
    double masa;
    double radio;
    bool activo;
    double clampValor(double valor, double minimo, double maximo) const;
public:
    proyectil();
    void lanzar(double x0, double y0, double anguloGrados, double velocidadInicial, double masaProyectil, double radioProyectil = 8.0);
    void actualizarPaso(double dt, double gravedad);
    void resolverColisionCaja(double minX, double maxX, double minY, double maxY);
    double resolverColisionObstaculo(obstaculos& obs, double factorConstante);
    double GETX() const;
    double GETY() const;
    double GETVx() const;
    double GETVy() const;
    double GETMasa() const;
    double GETRadio() const;
    bool estaActivo() const;
    void Desactivar();
};

#endif // PROYECTIL_H
