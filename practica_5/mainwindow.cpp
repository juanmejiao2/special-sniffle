#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QFont>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    configurarInterfaz();
    inicializarEscenario();

    timer=new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarSimulacion);
    connect(btnDisparar, &QPushButton::clicked, this, &MainWindow::onDispararClicked);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::configurarInterfaz() {
    QWidget* central = centralWidget();
    if (!central) {
        central = new QWidget(this);
        setCentralWidget(central);
    }
    vista = central->findChild<QGraphicsView*>();
    if (!vista) {
        vista = new QGraphicsView(central);
    }

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, cajaX * 2 + cajaAncho, cajaY * 2 + cajaAlto);
    vista->setScene(escena);
    vista->setMinimumSize(900, 500);

    lblTurno = new QLabel("Turno: Jugador 1", central);
    lblEstado = new QLabel("Configura ángulo y velocidad para lanzar.", central);

    spinAngulo = new QDoubleSpinBox(central);
    spinAngulo->setRange(-90.0, 180.0);
    spinAngulo->setValue(45.0);
    spinAngulo->setSuffix("°");

    spinVelocidad = new QDoubleSpinBox(central);
    spinVelocidad->setRange(20.0, 800.0);
    spinVelocidad->setValue(320.0);
    spinVelocidad->setSuffix(" u/s");

    btnDisparar = new QPushButton("Lanzar Proyectil", central);

    QHBoxLayout* barraControles = new QHBoxLayout();
    barraControles->addWidget(lblTurno);
    barraControles->addWidget(new QLabel("Ángulo:", central));
    barraControles->addWidget(spinAngulo);
    barraControles->addWidget(new QLabel("Velocidad:", central));
    barraControles->addWidget(spinVelocidad);
    barraControles->addWidget(btnDisparar);
    barraControles->addWidget(lblEstado, 1);

    QVBoxLayout* layoutPrincipal = new QVBoxLayout(central);
    layoutPrincipal->addWidget(vista);
    layoutPrincipal->addLayout(barraControles);
}

void MainWindow::agregarBloque(double x, double y, double w, double h,
                               double resistencia, double e) {
    BloqueVisual bv;
    bv.modelo = obstaculos(x, y, w, h, resistencia, e);

    bv.rectItem = escena->addRect(x, y, w, h,QPen(QColor(120, 80, 30), 2),QBrush(QColor(245, 215, 160)));

    bv.textoItem = escena->addText(QString::number(static_cast<int>(resistencia)));
    bv.textoItem->setDefaultTextColor(Qt::black);
    QFont fuente = bv.textoItem->font();
    fuente.setBold(true);
    bv.textoItem->setFont(fuente);
    bv.textoItem->setPos(x + w * 0.5 - 14.0, y + h * 0.5 - 12.0);

    bloques.append(bv);
}

void MainWindow::inicializarEscenario() {
    escena->clear();
    bloques.clear();

    escena->addRect(cajaX, cajaY, cajaAncho, cajaAlto,QPen(Qt::black, 3), QBrush(Qt::white));

    escena->addRect(cajaX, cajaY + 210, 45, 10, QPen(Qt::black), QBrush(Qt::gray));
    escena->addRect(cajaX + cajaAncho - 45, cajaY + 210, 45, 10, QPen(Qt::black), QBrush(Qt::gray));

    agregarBloque(cajaX + 90,  cajaY + 320, 35, 140, 200.0, 0.55);
    agregarBloque(cajaX + 185, cajaY + 320, 35, 140, 200.0, 0.55);
    agregarBloque(cajaX + 90,  cajaY + 280, 130, 40, 100.0, 0.55);

    rivalJugador1.modelo = enemigos(cajaX + 143, cajaY + 400, 24, 60);
    rivalJugador1.cuerpoItem = escena->addRect(rivalJugador1.modelo.GETX(),rivalJugador1.modelo.GETY(),rivalJugador1.modelo.GETAncho(),rivalJugador1.modelo.GETAlto(),QPen(Qt::blue, 2),QBrush(QColor(170, 205, 255)));
    rivalJugador1.etiquetaItem = escena->addText("Rival 1");
    rivalJugador1.etiquetaItem->setPos(rivalJugador1.modelo.GETX() - 10, rivalJugador1.modelo.GETY() - 22);
    agregarBloque(cajaX + 635, cajaY + 320, 35, 140, 200.0, 0.55);
    agregarBloque(cajaX + 730, cajaY + 320, 35, 140, 200.0, 0.55);
    agregarBloque(cajaX + 635, cajaY + 280, 130, 40, 100.0, 0.55);

    rivalJugador2.modelo = enemigos(cajaX + 688, cajaY + 400, 24, 60);
    rivalJugador2.cuerpoItem = escena->addRect(rivalJugador2.modelo.GETX(),rivalJugador2.modelo.GETY(),rivalJugador2.modelo.GETAncho(),rivalJugador2.modelo.GETAlto(),QPen(Qt::red, 2), QBrush(QColor(255, 180, 180)));
    rivalJugador2.etiquetaItem = escena->addText("Rival 2");
    rivalJugador2.etiquetaItem->setPos(rivalJugador2.modelo.GETX() - 10, rivalJugador2.modelo.GETY() - 22);
    double r = bala.GETRadio();
    itemProyectil = escena->addEllipse(-r, -r, 2.0 * r, 2.0 * r, QPen(Qt::darkRed, 2), QBrush(Qt::red));
    itemProyectil->setVisible(false);

    turnoActual = 1;
    lblTurno->setText("Turno: Jugador 1 (Izquierda)");
    btnDisparar->setEnabled(true);
}

void MainWindow::onDispararClicked() {
    if (bala.estaActivo()) return;

    double angulo = spinAngulo->value();
    double velocidad = spinVelocidad->value();

    double x0 = (turnoActual == 1) ? (cajaX + 52.0) : (cajaX + cajaAncho - 52.0);
    double y0 = cajaY + 195.0;
    double anguloEfectivo = (turnoActual == 1) ? angulo : (180.0 - angulo);

    bala.lanzar(x0, y0, anguloEfectivo, velocidad, masaProyectil, 8.0);
    itemProyectil->setVisible(true);
    btnDisparar->setEnabled(false);
    pasosSinMovimiento = 0;
    lblEstado->setText("Proyectil en vuelo...");

    timer->start(static_cast<int>(dt * 1000.0));
}

void MainWindow::actualizarSimulacion() {
    if (!bala.estaActivo()) {
        timer->stop();
        return;
    }

    bala.actualizarPaso(dt, gravedad);
    bala.resolverColisionCaja(cajaX, cajaX + cajaAncho, cajaY, cajaY + cajaAlto);

    double danioTotalTurno = 0.0;
    for (int i = 0; i < bloques.size(); ++i) {
        if (!bloques[i].modelo.estaDestruido()) {
            double d = bala.resolverColisionObstaculo(bloques[i].modelo, factorConstanteDano);
            if (d > 0.0) {
                danioTotalTurno += d;
                if (bloques[i].modelo.estaDestruido()) {
                    bloques[i].rectItem->setVisible(false);
                    bloques[i].textoItem->setVisible(false);
                } else {
                    bloques[i].textoItem->setPlainText(
                        QString::number(static_cast<int>(std::ceil(bloques[i].modelo.GETResistencia()))));
                }
            }
        }
    }

    if (turnoActual == 1 && rivalJugador2.modelo.EstaVivo()) {
        if (rivalJugador2.modelo.ColisionaConPunto(bala.GETX(), bala.GETY(), bala.GETRadio())) {
            rivalJugador2.modelo.RecibirImpacto();
            rivalJugador2.cuerpoItem->setBrush(QBrush(Qt::darkGray));
            bala.Desactivar();
        }
    } else if (turnoActual == 2 && rivalJugador1.modelo.EstaVivo()) {
        if (rivalJugador1.modelo.ColisionaConPunto(bala.GETX(), bala.GETY(), bala.GETRadio())) {
            rivalJugador1.modelo.RecibirImpacto();
            rivalJugador1.cuerpoItem->setBrush(QBrush(Qt::darkGray));
            bala.Desactivar();
        }
    }

    refrescarGraficos();

    if (danioTotalTurno > 0.0) {
        lblEstado->setText(QString("Impacto! Daño infligido: %1").arg(QString::number(danioTotalTurno, 'f', 1)));
    }

    double rapidez = std::hypot(bala.GETVx(), bala.GETVy());
    if (rapidez < 12.0) {
        pasosSinMovimiento++;
    } else {
        pasosSinMovimiento = 0;
    }

    if (pasosSinMovimiento > 25) {
        bala.Desactivar();
    }

    if (!bala.estaActivo()) {
        timer->stop();
        itemProyectil->setVisible(false);
        verificarWC();
        if (btnDisparar->isEnabled() == false &&
            rivalJugador1.modelo.EstaVivo() && rivalJugador2.modelo.EstaVivo()) {
            finalizarTurno();
        }
    }
}

void MainWindow::refrescarGraficos() {
    double r = bala.GETRadio();
    itemProyectil->setPos(bala.GETX() - r, bala.GETY() - r);
}

void MainWindow::finalizarTurno() {
    turnoActual = (turnoActual == 1) ? 2 : 1;
    lblTurno->setText((turnoActual == 1) ? "Turno: Jugador 1 (Izquierda)": "Turno: Jugador 2 (Derecha)");
    btnDisparar->setEnabled(true);
}

void MainWindow::verificarWC() {
    if (!rivalJugador2.modelo.EstaVivo()) {
        lblEstado->setText("¡Victoria del Jugador 1! El Rival 2 fue vulnerado.");
        btnDisparar->setEnabled(false);
    } else if (!rivalJugador1.modelo.EstaVivo()) {
        lblEstado->setText("¡Victoria del Jugador 2! El Rival 1 fue vulnerado.");
        btnDisparar->setEnabled(false);
    }
}