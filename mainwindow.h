#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QGraphicsScene;

#include "graphicsview.h"

struct RectLL {
    long long x1, y1, x2, y2;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAdd();
    void onRemoveSelected();
    void onClear();
    void onStepChanged(int k);

private:
    Ui::MainWindow *ui = nullptr;

    QVector<RectLL> m_rects;
    QGraphicsScene* m_scene = nullptr;

    RectLL readInputs() const;
    void addRowToTable(const RectLL& r);
    void redrawScene(int k);
};

#endif // MAINWINDOW_H
