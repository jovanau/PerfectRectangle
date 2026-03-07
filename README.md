# Perfect Rectangle - Qt Visualization

A C++ Qt application that verifies whether a set of axis-aligned rectangles forms one perfect rectangle without gaps or overlaps.

This project was developed as part of a **Geometric Algorithms course** and provides both:

- an **algorithmic verification**
- a **graphical visualization**

of the Perfect Rectangle problem.

---

# Problem Description

Given a set of rectangles aligned with the coordinate axes, determine whether they exactly cover a larger rectangle.

A valid configuration must satisfy:

1. **No overlaps**
2. **No gaps**
3. The union of all rectangles must form **one perfect rectangle**

Each rectangle is defined by:

```
(x1, y1) — bottom-left corner  
(x2, y2) — top-right corner
```

---

# Algorithm

The application implements the classical **Perfect Rectangle algorithm** using two main checks.

## 1. Area Check

The sum of the areas of all rectangles must equal the area of the bounding rectangle.

```
sum(rectangle areas) == bounding rectangle area
```

If the areas differ, there must be either:

- a **gap**, or
- an **overlap**

---

## 2. Corner Parity Check

Each rectangle contributes four corner points.

Using a set-based parity rule:

- internal corners appear **even number of times**
- the four outer corners appear **exactly once**

Therefore, the set of remaining points must contain exactly **four corners**, which correspond to the bounding rectangle.

This ensures there are no hidden overlaps or holes.

---

<img width="1191" height="770" alt="Screenshot from 2026-03-07 12-24-07" src="https://github.com/user-attachments/assets/5267fdc3-d133-4551-8925-b63da8b2b107" />
<img width="1200" height="777" alt="Screenshot from 2026-03-07 12-23-41" src="https://github.com/user-attachments/assets/606fbe93-6f30-4a3d-a30e-225887a643ab" />
