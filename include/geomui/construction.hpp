#pragma once

#include <geomui/solver.hpp>
#include <iostream>

// some construction prefabs

namespace geomui {

class Point {
public:
    const Var x, y;
    
    Point() : x(), y() {}
    Point(double x, double y) : x(x), y(y) {}
};


static void horizontal(const Point& p1, const Point& p2) {
    p1.y |= p2.y;
}

static void vertical(const Point& p1, const Point& p2) {
    p1.x |= p2.x;
}

static void coincedent(const Point& p1, const Point& p2) {
    p1.x |= p2.x;
    p1.y |= p2.y;
}

class Line {
public:
    const Point p1, p2;

    Line() : p1(), p2() {}
};

static void horizontal(const Line& l) {
    horizontal(l.p1, l.p2);
}

static void vertical(const Line& l) {
    vertical(l.p1, l.p2);
}

static void midpoint(const Line& l, const Point& mid) {
    mid.x |= 0.5 * l.p1.x + 0.5 * l.p2.x;
    mid.y |= 0.5 * l.p1.y + 0.5 * l.p2.y;
}

static void width(const Line& l, const Var& w) {
    w |= l.p2.x - l.p1.x;
}

static void height(const Line& l, const Var& h) {
    h |= l.p1.y - l.p2.y;
}

class Rectangle {
public:
    const Point tl, br;
    const Var wVar, hVar;
    const Line top;

    Rectangle() : tl(), br(), wVar(), hVar(), top() {
        wVar |= br.x - tl.x;
        hVar |= tl.y - br.y;

        coincedent(top.p1, tl);
        top.p2.x |= br.x;
        top.p2.y |= tl.y;
    }
};

} // namespace geomui