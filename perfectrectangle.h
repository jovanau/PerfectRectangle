#pragma once
#include <QVector>
#include <QPointF>
#include <QString>

struct RectLL {
    long long x1, y1, x2, y2;
};

struct CheckResult {
    bool ok = false;
    RectLL outer{0,0,0,0};
    long long sumArea = 0;
    long long outerArea = 0;
    QVector<QPointF> oddCorners; // за Дан 3 (сад само попуњавамо)
    QString reason;
};

CheckResult checkPerfectRectangle(const QVector<RectLL>& rects);
