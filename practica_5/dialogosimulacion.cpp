#include "dialogosimulacion.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QBrush>
#include <QPen>

dialogosimulacion::dialogosimulacion(QWidget *parent): QDialog(parent) {
    setWindowTitle("Simulación de colisiones múltiples");
    resize(940, 720);

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 900, 600);
    vista = new QGraphicsView(escena, this);
    vista->setMinimumSize(910, 610);

    lblEstado = new QLabel(this);
    btnIniciar = new QPushButton("Iniciar", this);
    QPushButton* btnPausar = new QPushButton("Pausar", this);
    QPushButton* btnReiniciar = new QPushButton("Reiniciar", this);
    QPushButton* btnGuardar = new QPushButton("Guardar datos CSV", this);

    QHBoxLayout* barra = new QHBoxLayout();
    barra->addWidget(btnIniciar);
    barra->addWidget(btnPausar);
    barra->addWidget(btnReiniciar);
    barra->addWidget(btnGuardar);
    barra->addWidget(lblEstado, 1);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(vista);
    layout->addLayout(barra);

    timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &dialogosimulacion::actualizarSimulacion);
    connect(btnIniciar, &QPushButton::clicked, timer, qOverload<>(&QTimer::start));
    connect(btnPausar, &QPushButton::clicked, timer, &QTimer::stop);
    connect(btnReiniciar, &QPushButton::clicked, this, &dialogosimulacion::reiniciarSimulacion);
    connect(btnGuardar, &QPushButton::clicked, this, &dialogosimulacion::exportarDatos);

    dibujarEscenario();
    refrescarParticulas();
}

void dialogosimulacion::dibujarEscenario() {
    escena->clear();
    escena->addRect(0, 0, 900, 600, QPen(Qt::black, 3), QBrush(Qt::white));

    for (const obstaculoCuadrado& obstaculo : simulador.GETObstaculos()) {
        escena->addRect(obstaculo.x, obstaculo.y, obstaculo.lado, obstaculo.lado,
                        QPen(QColor(100, 75, 45), 2), QBrush(QColor(205, 170, 120)));
    }

    itemsParticulas.clear();
    etiquetasParticulas.clear();
}

void dialogosimulacion::refrescarParticulas() {
    const QVector<particula>& particulas = simulador.GETParticulas();
    const QColor colores[] = {Qt::red, Qt::blue, Qt::darkGreen, QColor(230, 130, 0), Qt::magenta};

    while (itemsParticulas.size() < particulas.size()) {
        int indice = itemsParticulas.size();
        double radio = particulas[indice].GETRadio();
        QGraphicsEllipseItem* item = escena->addEllipse(-radio, -radio, radio * 2.0, radio * 2.0,
                                                        QPen(Qt::black, 1), QBrush(colores[indice % 5]));
        QGraphicsSimpleTextItem* etiqueta = escena->addSimpleText(particulas[indice].GETNombre());
        etiqueta->setBrush(QBrush(Qt::black));
        itemsParticulas.append(item);
        etiquetasParticulas.append(etiqueta);
    }
    while (itemsParticulas.size() > particulas.size()) {
        QGraphicsEllipseItem* item = itemsParticulas.takeLast();
        QGraphicsSimpleTextItem* etiqueta = etiquetasParticulas.takeLast();
        escena->removeItem(item);
        escena->removeItem(etiqueta);
        delete item;
        delete etiqueta;
    }

    for (int i = 0; i < particulas.size(); ++i) {
        const particula& p = particulas[i];
        double radio = p.GETRadio();
        itemsParticulas[i]->setRect(-radio, -radio, radio * 2.0, radio * 2.0);
        itemsParticulas[i]->setPos(p.GETX(), p.GETY());
        etiquetasParticulas[i]->setText(p.GETNombre());
        etiquetasParticulas[i]->setPos(p.GETX() + radio, p.GETY() - radio - 16.0);
    }

    lblEstado->setText(QString("Tiempo: %1 s | Partículas: %2 | Colisiones: %3")
                       .arg(simulador.GETTiempo(), 0, 'f', 2)
                       .arg(particulas.size())
                       .arg(simulador.GETCantidadColisiones()));
}

void dialogosimulacion::actualizarSimulacion() {
    simulador.actualizarPaso(0.016);
    refrescarParticulas();
}

void dialogosimulacion::reiniciarSimulacion() {
    timer->stop();
    simulador.reiniciar();
    dibujarEscenario();
    refrescarParticulas();
}

void dialogosimulacion::exportarDatos() {
    QString ruta = QFileDialog::getSaveFileName(this, "Guardar datos de simulación",
                                                 "simulacion_colisiones.csv", "CSV (*.csv)");
    if (ruta.isEmpty()) return;
    if (simulador.exportarCSV(ruta)) {
        QMessageBox::information(this, "Datos guardados", "La simulación se guardó en el archivo CSV.");
    } else {
        QMessageBox::warning(this, "No se pudo guardar", "No fue posible escribir el archivo seleccionado.");
    }
}
