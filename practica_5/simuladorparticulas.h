#ifndef SIMULADORPARTICULAS_H
#define SIMULADORPARTICULAS_H

#include "particula.h"
#include <QVector>
#include <QString>

struct obstaculoCuadrado {
    double x;
    double y;
    double lado;
};

struct registroParticula {
    double tiempo;
    QString evento;
    QString particula;
    QString objetivo;
    double x;
    double y;
    double vx;
    double vy;
    double masa;
};

class simuladorparticulas {
private:
    double anchoCaja;
    double altoCaja;
    double tiempo;
    double coefRestitucion;
    QVector<particula> particulas;
    QVector<obstaculoCuadrado> obstaculos;
    QVector<registroParticula> registros;

    void registrarEvento(const QString& evento, const particula& p,
                         const QString& otra = QString());
    void resolverParedes(particula& p);
    void resolverObstaculos(particula& p);
    void fusionarParticulas();

public:
    simuladorparticulas();
    void reiniciar();
    void actualizarPaso(double dt);
    bool exportarCSV(const QString& ruta) const;

    double GETTiempo() const;
    int GETCantidadColisiones() const;
    const QVector<particula>& GETParticulas() const;
    const QVector<obstaculoCuadrado>& GETObstaculos() const;
};

#endif // SIMULADORPARTICULAS_H
