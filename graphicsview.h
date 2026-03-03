#pragma once
#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit GraphicsView(QWidget* parent=nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;
};
