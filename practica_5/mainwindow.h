#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>

#include "proyectil.h"
#include "obstaculos.h"
#include "enemigos.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onDispararClicked();
    void actualizarSimulacion();

private:
    struct BloqueVisual {
        obstaculos modelo;
        QGraphicsRectItem* rectItem = nullptr;
        QGraphicsTextItem* textoItem = nullptr;
    };

    struct RivalVisual {
        enemigos modelo;
        QGraphicsRectItem* cuerpoItem = nullptr;
        QGraphicsTextItem* etiquetaItem = nullptr;
    };

    Ui::MainWindow *ui;

    QGraphicsScene* escena = nullptr;
    QGraphicsView* vista = nullptr;
    QTimer* timer = nullptr;
    QDoubleSpinBox* spinAngulo = nullptr;
    QDoubleSpinBox* spinVelocidad = nullptr;
    QPushButton* btnDisparar = nullptr;
    QLabel* lblTurno = nullptr;
    QLabel* lblEstado = nullptr;
    QGraphicsEllipseItem* itemProyectil = nullptr;
    proyectil bala;
    QVector<BloqueVisual> bloques;
    RivalVisual rivalJugador1;
    RivalVisual rivalJugador2;
    int turnoActual = 1;
    int pasosSinMovimiento = 0;
    const double cajaX = 20.0;
    const double cajaY = 20.0;
    const double cajaAncho = 860.0;
    const double cajaAlto = 460.0;
    const double dt = 0.016;
    const double gravedad = 180.0;
    const double masaProyectil = 2.0;
    const double factorConstanteDano = 0.15;
    void configurarInterfaz();
    void inicializarEscenario();
    void agregarBloque(double x, double y, double w, double h, double resistencia, double e);
    void refrescarGraficos();
    void finalizarTurno();
    void verificarWC();
};
#endif // MAINWINDOW_H
