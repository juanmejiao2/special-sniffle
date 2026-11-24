#ifndef PARTICULA_H
#define PARTICULA_H

#include <QString>

class particula {
private:
    QString nombre;
    double x;
    double y;
    double vx;
    double vy;
    double masa;
    double radio;

public:
    particula();
    particula(QString nombre, double x, double y, double vx, double vy,
              double masa, double radio);

    QString GETNombre() const;
    double GETX() const;
    double GETY() const;
    double GETVx() const;
    double GETVy() const;
    double GETMasa() const;
    double GETRadio() const;

    void actualizarPaso(double dt);
    void fijarPosicion(double nuevaX, double nuevaY);
    void fijarVelocidad(double nuevaVx, double nuevaVy);
};

#endif // PARTICULA_H
