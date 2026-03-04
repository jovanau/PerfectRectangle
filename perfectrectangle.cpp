#include "perfectrectangle.h"
#include <unordered_set>
#include <limits>
#include <algorithm>

static inline quint64 packPointLL(long long x, long long y) {
    quint64 ux = static_cast<quint32>(static_cast<qint32>(x));
    quint64 uy = static_cast<quint32>(static_cast<qint32>(y));
    return (ux << 32) ^ uy;
}

CheckResult checkPerfectRectangle(const QVector<RectLL>& rects) {
    CheckResult res;
    const int n = rects.size();
    if (n == 0) {
        res.ok = false;
        res.reason = "No rectangles provided.";
        return res;
    }

    long long minX = std::numeric_limits<long long>::max();
    long long minY = std::numeric_limits<long long>::max();
    long long maxX = std::numeric_limits<long long>::min();
    long long maxY = std::numeric_limits<long long>::min();

    long long sumArea = 0;

    std::unordered_set<quint64> S;
    S.reserve(static_cast<size_t>(n) * 4);

    auto toggle = [&](long long x, long long y) {
        quint64 key = packPointLL(x, y);
        auto it = S.find(key);
        if (it == S.end()) S.insert(key);
        else S.erase(it);
    };

    for (int i = 0; i < n; i++) {
        const auto& r = rects[i];

        if (r.x1 >= r.x2 || r.y1 >= r.y2) {
            res.ok = false;
            res.reason = QString("Degenerate rectangle at id=%1 (x1>=x2 or y1>=y2).").arg(i);
            return res;
        }

        minX = std::min(minX, r.x1);
        minY = std::min(minY, r.y1);
        maxX = std::max(maxX, r.x2);
        maxY = std::max(maxY, r.y2);

        const long long w = r.x2 - r.x1;
        const long long h = r.y2 - r.y1;
        sumArea += w * h;

        toggle(r.x1, r.y1);
        toggle(r.x1, r.y2);
        toggle(r.x2, r.y1);
        toggle(r.x2, r.y2);
    }

    res.outer = {minX, minY, maxX, maxY};
    res.sumArea = sumArea;
    res.outerArea = (maxX - minX) * (maxY - minY);

    // odd corners
    res.oddCorners.reserve(static_cast<int>(S.size()));
    for (quint64 key : S) {
        qint32 x = static_cast<qint32>(key >> 32);
        qint32 y = static_cast<qint32>(key & 0xffffffffu);
        res.oddCorners.push_back(QPointF(x, y));
    }

    if (res.sumArea != res.outerArea) {
        res.ok = false;
        res.reason = QString("Area mismatch: sumArea=%1, outerArea=%2 (hole/overlap likely).")
                         .arg(res.sumArea).arg(res.outerArea);
        return res;
    }

    if (S.size() != 4) {
        res.ok = false;
        res.reason = QString("Corner parity mismatch: oddCorners=%1 (expected 4).")
                         .arg(static_cast<int>(S.size()));
        return res;
    }

    const quint64 c1 = packPointLL(minX, minY);
    const quint64 c2 = packPointLL(minX, maxY);
    const quint64 c3 = packPointLL(maxX, minY);
    const quint64 c4 = packPointLL(maxX, maxY);

    if (S.count(c1)==0 || S.count(c2)==0 || S.count(c3)==0 || S.count(c4)==0) {
        res.ok = false;
        res.reason = "Outer corner mismatch: expected bounding rectangle corners not found.";
        return res;
    }

    res.ok = true;
    res.reason = "OK: Perfect rectangle (area + corner parity checks passed).";
    return res;
}
