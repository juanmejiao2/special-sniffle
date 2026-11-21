#ifndef OBSTACULOS_H
#define OBSTACULOS_H

class obstaculos{
private:
    double x;
    double y;
    double ancho;
    double Alto;
    double Resistencia;
    double CoeficienteRestitucion;
public:
    obstaculos();
    obstaculos(double posicionX, double posicionY, double w, double h, double resistenciaInicial, double e=0.6);

    double GETX() const;
    double GETY() const;
    double GETAncho() const;
    double GETAlto() const;
    double GETResistencia() const;
    double GETCoefRestitucion() const;
    bool estaDestruido() const;
    void aplicarDano(double dano);
};

#endif // OBSTACULOS_H
