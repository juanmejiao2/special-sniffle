#include "proyectil.h"
#include <cmath>

proyectil::proyectil(): x(0.0), y(0.0), vx(0.0), vy(0.0), masa(1.0), radio(8.0), activo(false) {
}

double proyectil::clampValor(double valor, double minimo, double maximo) const {
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

void proyectil::lanzar(double x0, double y0, double anguloGrados, double velocidadInicial, double masaProyectil, double radioProyectil) {
    const double pi = std::acos(-1.0);
    double rad = anguloGrados * (pi / 180.0);

    x = x0;
    y = y0;
    vx = velocidadInicial * std::cos(rad);
    vy = -velocidadInicial * std::sin(rad);
    masa = masaProyectil;
    radio = radioProyectil;
    activo = true;
}

void proyectil::actualizarPaso(double dt, double gravedad) {
    if (!activo) return;
    x += vx * dt;
    y += vy * dt + 0.5 * gravedad * dt * dt;
    vy += gravedad * dt;
}

void proyectil::resolverColisionCaja(double minX, double maxX, double minY, double maxY) {
    if (!activo) return;
    if (x - radio < minX) {
        x = minX + radio;
        vx = -vx;
    } else if (x + radio > maxX) {
        x = maxX - radio;
        vx = -vx;
    }
    if (y - radio < minY) {
        y = minY + radio;
        vy = -vy;
    } else if (y + radio > maxY) {
        y = maxY - radio;
        vy = -vy;
    }
}
double proyectil::resolverColisionObstaculo(obstaculos& obs, double factorConstante) {
    if (!activo || obs.estaDestruido()) return 0.0;

    double ox = obs.GETX();
    double oy = obs.GETY();
    double ow = obs.GETAncho();
    double oh = obs.GETAlto();

    double closestX = clampValor(x, ox, ox + ow);
    double closestY = clampValor(y, oy, oy + oh);
    double dx = x - closestX;
    double dy = y - closestY;

    if ((dx * dx + dy * dy) > (radio * radio)) {
        return 0.0;
    }
    double vImpacto = std::sqrt(vx * vx + vy * vy);
    double e = obs.GETCoefRestitucion();
    double overlapLeft = (x + radio) - ox;
    double overlapRight = (ox + ow) - (x - radio);
    double overlapTop = (y + radio) - oy;
    double overlapBottom = (oy + oh) - (y - radio);

    double minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
    double minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

    if (minOverlapX < minOverlapY) {
        if (overlapLeft < overlapRight) {
            x = ox - radio;
        } else {
            x = ox + ow + radio;
        }
        vx = -e * vx;
    } else {
        if (overlapTop < overlapBottom) {
            y = oy - radio;
        } else {
            y = oy + oh + radio;
        }
        vy = -e * vy;
    }

    double dano = factorConstante * masa * vImpacto;
    obs.aplicarDano(dano);
    return dano;
}

double proyectil::GETX() const { return x; }
double proyectil::GETY() const { return y; }
double proyectil::GETVx() const { return vx; }
double proyectil::GETVy() const { return vy; }
double proyectil::GETMasa() const { return masa; }
double proyectil::GETRadio() const { return radio; }
bool proyectil::estaActivo() const { return activo; }
void proyectil::Desactivar() { activo = false; }