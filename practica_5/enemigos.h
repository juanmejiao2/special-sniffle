#ifndef ENEMIGOS_H
#define ENEMIGOS_H

class enemigos{
private:
    double x;
    double y;
    double ancho;
    double alto;
    bool vivo;

public:
    enemigos();
    enemigos(double posicionX, double posicionY, double w=24.0, double altura=48.0);
    double GETX() const;
    double GETY() const;
    double GETAncho() const;
    double GETAlto() const;
    bool EstaVivo() const;
    void RecibirImpacto();
    bool ColisionaConPunto(double px, double py, double radio=0.0) const;
};

#endif // ENEMIGOS_H
