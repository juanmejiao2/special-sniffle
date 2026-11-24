#ifndef DIALOGOSIMULACION_H
#define DIALOGOSIMULACION_H

#include "simuladorparticulas.h"
#include <QDialog>
#include <QVector>

class QGraphicsEllipseItem;
class QGraphicsScene;
class QGraphicsSimpleTextItem;
class QGraphicsView;
class QLabel;
class QPushButton;
class QTimer;

class dialogosimulacion : public QDialog {
    Q_OBJECT

private:
    simuladorparticulas simulador;
    QGraphicsScene* escena = nullptr;
    QGraphicsView* vista = nullptr;
    QTimer* timer = nullptr;
    QLabel* lblEstado = nullptr;
    QPushButton* btnIniciar = nullptr;
    QVector<QGraphicsEllipseItem*> itemsParticulas;
    QVector<QGraphicsSimpleTextItem*> etiquetasParticulas;

    void dibujarEscenario();
    void refrescarParticulas();
    void actualizarSimulacion();

private slots:
    void exportarDatos();
    void reiniciarSimulacion();

public:
    explicit dialogosimulacion(QWidget *parent = nullptr);
};

#endif // DIALOGOSIMULACION_H
