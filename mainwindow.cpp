#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTableWidgetItem>
#include <QBrush>
#include <QPen>
#include <algorithm>

static QRectF toSceneRect(const RectLL& r) {
    // invert Y so that (x,y) is mathematical (upwards)
    const double w = double(r.x2 - r.x1);
    const double h = double(r.y2 - r.y1);
    return QRectF(double(r.x1), double(-r.y2), w, h);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Scene setup
    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    // Table setup (if not already set in Designer)
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"x1","y1","x2","y2"});
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Step slider init
    ui->sliderStep->setRange(0, 0);
    ui->sliderStep->setValue(0);
    ui->labelStep->setText("0/0");

    // Connect signals
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(ui->btnRemove, &QPushButton::clicked, this, &MainWindow::onRemoveSelected);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClear);
    connect(ui->sliderStep, &QSlider::valueChanged, this, &MainWindow::onStepChanged);

    redrawScene(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

RectLL MainWindow::readInputs() const {
    return RectLL{
        ui->spinX1->value(),
        ui->spinY1->value(),
        ui->spinX2->value(),
        ui->spinY2->value()
    };
}

void MainWindow::addRowToTable(const RectLL& r) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    auto setCell = [&](int col, long long v) {
        auto* it = new QTableWidgetItem(QString::number(v));
        it->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, col, it);
    };

    setCell(0, r.x1);
    setCell(1, r.y1);
    setCell(2, r.x2);
    setCell(3, r.y2);
}

void MainWindow::onAdd() {
    RectLL r = readInputs();
    m_rects.push_back(r);
    addRowToTable(r);

    ui->sliderStep->setRange(0, m_rects.size());
    ui->sliderStep->setValue(m_rects.size()); // jump to full view

    redrawScene(ui->sliderStep->value());
}

void MainWindow::onRemoveSelected() {
    auto sel = ui->tableWidget->selectionModel()->selectedRows();
    if (sel.isEmpty()) return;

    int row = sel.first().row();
    if (row >= 0 && row < m_rects.size())
        m_rects.removeAt(row);

    ui->tableWidget->removeRow(row);

    ui->sliderStep->setRange(0, m_rects.size());
    ui->sliderStep->setValue(m_rects.size());

    redrawScene(ui->sliderStep->value());
}

void MainWindow::onClear() {
    m_rects.clear();
    ui->tableWidget->setRowCount(0);

    ui->sliderStep->setRange(0, 0);
    ui->sliderStep->setValue(0);
    ui->labelStep->setText("0/0");

    redrawScene(0);
}

void MainWindow::onStepChanged(int k) {
    ui->labelStep->setText(QString("%1/%2").arg(k).arg(m_rects.size()));
    redrawScene(k);
}

void MainWindow::redrawScene(int k) {
    m_scene->clear();

    k = std::clamp(k, 0, (int)m_rects.size());
    if (k == 0) {
        m_scene->setSceneRect(QRectF(-10, -10, 20, 20));
        ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
        return;
    }

    QPen pen(Qt::black);
    pen.setWidthF(0.0);
    QColor fill(80, 130, 255, 70);

    for (int i = 0; i < k; i++) {
        const auto& r = m_rects[i];
        QRectF sr = toSceneRect(r);
        m_scene->addRect(sr, pen, QBrush(fill));
    }

    QRectF bounds = m_scene->itemsBoundingRect();
    bounds.adjust(-1, -1, 1, 1);
    m_scene->setSceneRect(bounds);
    // Не fit-ујемо сваки пут да не “ломи” zoom; имаш reset по потреби касније.
}
