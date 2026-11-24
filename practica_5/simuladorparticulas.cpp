#include "simuladorparticulas.h"
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <cmath>

simuladorparticulas::simuladorparticulas(): anchoCaja(900.0), altoCaja(600.0), tiempo(0.0), coefRestitucion(0.65) {
    reiniciar();
}

void simuladorparticulas::reiniciar() {
    tiempo = 0.0;
    particulas.clear();
    obstaculos.clear();
    registros.clear();

    obstaculos.append(obstaculoCuadrado{250.0, 140.0, 75.0});
    obstaculos.append(obstaculoCuadrado{575.0, 140.0, 75.0});
    obstaculos.append(obstaculoCuadrado{250.0, 385.0, 75.0});
    obstaculos.append(obstaculoCuadrado{575.0, 385.0, 75.0});

    particulas.append(particula("P1", 95.0, 105.0, 145.0, 92.0, 2.0, 13.0));
    particulas.append(particula("P2", 805.0, 105.0, -135.0, 112.0, 3.0, 15.0));
    particulas.append(particula("P3", 95.0, 495.0, 150.0, -95.0, 2.5, 14.0));
    particulas.append(particula("P4", 805.0, 495.0, -142.0, -105.0, 1.5, 12.0));
}

void simuladorparticulas::registrarEvento(const QString& evento, const particula& p,
                                           const QString& otra) {
    registros.append({tiempo, evento, p.GETNombre(), otra, p.GETX(), p.GETY(),
                      p.GETVx(), p.GETVy(), p.GETMasa()});
}

void simuladorparticulas::resolverParedes(particula& p) {
    double x = p.GETX();
    double y = p.GETY();
    double vx = p.GETVx();
    double vy = p.GETVy();
    double r = p.GETRadio();
    bool colision = false;
    QString paredes;

    if (x - r < 0.0) {
        x = r;
        vx = std::abs(vx);
        colision = true;
        paredes += "izquierda";
    } else if (x + r > anchoCaja) {
        x = anchoCaja - r;
        vx = -std::abs(vx);
        colision = true;
        paredes += "derecha";
    }
    if (y - r < 0.0) {
        y = r;
        vy = std::abs(vy);
        colision = true;
        if (!paredes.isEmpty()) paredes += ";";
        paredes += "superior";
    } else if (y + r > altoCaja) {
        y = altoCaja - r;
        vy = -std::abs(vy);
        colision = true;
        if (!paredes.isEmpty()) paredes += ";";
        paredes += "inferior";
    }

    if (colision) {
        p.fijarPosicion(x, y);
        p.fijarVelocidad(vx, vy);
        registrarEvento("pared_elastica", p, paredes);
    }
}

void simuladorparticulas::resolverObstaculos(particula& p) {
    for (int i = 0; i < obstaculos.size(); ++i) {
        const obstaculoCuadrado& obstaculo = obstaculos[i];
        double cercaX = std::clamp(p.GETX(), obstaculo.x, obstaculo.x + obstaculo.lado);
        double cercaY = std::clamp(p.GETY(), obstaculo.y, obstaculo.y + obstaculo.lado);
        double dx = p.GETX() - cercaX;
        double dy = p.GETY() - cercaY;
        double distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia > p.GETRadio()) continue;

        double nx = 0.0;
        double ny = 0.0;
        if (distancia > 0.0001) {
            nx = dx / distancia;
            ny = dy / distancia;
        } else {
            double izquierda = p.GETX() - obstaculo.x;
            double derecha = obstaculo.x + obstaculo.lado - p.GETX();
            double arriba = p.GETY() - obstaculo.y;
            double abajo = obstaculo.y + obstaculo.lado - p.GETY();
            double menor = std::min(std::min(izquierda, derecha), std::min(arriba, abajo));
            if (menor == izquierda) { nx = -1.0; cercaX = obstaculo.x; cercaY = p.GETY(); }
            else if (menor == derecha) { nx = 1.0; cercaX = obstaculo.x + obstaculo.lado; cercaY = p.GETY(); }
            else if (menor == arriba) { ny = -1.0; cercaX = p.GETX(); cercaY = obstaculo.y; }
            else { ny = 1.0; cercaX = p.GETX(); cercaY = obstaculo.y + obstaculo.lado; }
        }

        p.fijarPosicion(cercaX + nx * p.GETRadio(), cercaY + ny * p.GETRadio());
        double velocidadNormal = p.GETVx() * nx + p.GETVy() * ny;
        if (velocidadNormal < 0.0) {
            double cambio = (1.0 + coefRestitucion) * velocidadNormal;
            p.fijarVelocidad(p.GETVx() - cambio * nx, p.GETVy() - cambio * ny);
            registrarEvento("obstaculo_inelastico", p, QString("O%1").arg(i + 1));
        }
    }
}

void simuladorparticulas::fusionarParticulas() {
    for (int i = 0; i < particulas.size(); ++i) {
        for (int j = i + 1; j < particulas.size(); ++j) {
            const particula& p1 = particulas[i];
            const particula& p2 = particulas[j];
            double dx = p2.GETX() - p1.GETX();
            double dy = p2.GETY() - p1.GETY();
            double sumaRadios = p1.GETRadio() + p2.GETRadio();
            if (dx * dx + dy * dy > sumaRadios * sumaRadios) continue;

            double masa = p1.GETMasa() + p2.GETMasa();
            double x = (p1.GETX() * p1.GETMasa() + p2.GETX() * p2.GETMasa()) / masa;
            double y = (p1.GETY() * p1.GETMasa() + p2.GETY() * p2.GETMasa()) / masa;
            double vx = (p1.GETVx() * p1.GETMasa() + p2.GETVx() * p2.GETMasa()) / masa;
            double vy = (p1.GETVy() * p1.GETMasa() + p2.GETVy() * p2.GETMasa()) / masa;
            QString nombre = p1.GETNombre() + "+" + p2.GETNombre();
            double radio = std::sqrt(p1.GETRadio() * p1.GETRadio() + p2.GETRadio() * p2.GETRadio());
            particula fusionada(nombre, x, y, vx, vy, masa, radio);
            registrarEvento("fusion_inelastica", fusionada, p2.GETNombre());
            particulas[i] = fusionada;
            particulas.removeAt(j);
            --j;
        }
    }
}

void simuladorparticulas::actualizarPaso(double dt) {
    if (dt <= 0.0) return;
    tiempo += dt;

    for (particula& p : particulas) {
        p.actualizarPaso(dt);
        resolverParedes(p);
        resolverObstaculos(p);
    }
    fusionarParticulas();

    for (const particula& p : particulas) {
        registrarEvento("estado", p);
    }
}

bool simuladorparticulas::exportarCSV(const QString& ruta) const {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream salida(&archivo);
    salida << "tiempo,evento,particula,objetivo,x,y,vx,vy,masa\n";
    for (const registroParticula& registro : registros) {
        salida << QString::number(registro.tiempo, 'f', 4) << ','
               << registro.evento << ',' << registro.particula << ','
               << registro.objetivo << ','
               << QString::number(registro.x, 'f', 3) << ','
               << QString::number(registro.y, 'f', 3) << ','
               << QString::number(registro.vx, 'f', 3) << ','
               << QString::number(registro.vy, 'f', 3) << ','
               << QString::number(registro.masa, 'f', 3) << '\n';
    }
    return true;
}

double simuladorparticulas::GETTiempo() const { return tiempo; }
int simuladorparticulas::GETCantidadColisiones() const {
    int cantidad = 0;
    for (const registroParticula& r : registros) {
        if (r.evento != "estado") ++cantidad;
    }
    return cantidad;
}
const QVector<particula>& simuladorparticulas::GETParticulas() const { return particulas; }
const QVector<obstaculoCuadrado>& simuladorparticulas::GETObstaculos() const { return obstaculos; }
