#pragma once
#include <cmath>

struct Point
{
public:
    double x, y, z;

    Point() : x(0.0), y(0.0), z(0.0) {}
    Point(const double x, const double y, const double z) : x(x), y(y), z(z) {}
    Point operator+(const double rhs)
    {
        return Point(x + rhs, y + rhs, z + rhs);
    }
    Point operator+(const Point rhs)
    {
        return Point(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Point operator-(const double rhs)
    {
        return Point(x - rhs, y - rhs, z - rhs);
    }
    Point operator-(const Point rhs)
    {
        return Point(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Point operator*(const double rhs)
    {
        return Point(x * rhs, y * rhs, z * rhs);
    }
    Point &operator*=(const double rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
};

static Point operator+(const double lhs, const Point rhs)
{
    return Point(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
}
static Point operator-(const double lhs, const Point rhs)
{
    return Point(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
}
static Point operator*(const double lhs, const Point rhs)
{
    return Point(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

struct Quartenion
{
public:
    double r, i, j, k;

    Quartenion() : r(0.0), i(0.0), j(0.0), k(0.0) {}
    Quartenion(const double r) : r(r), i(0.0), j(0.0), k(0.0) {}
    Quartenion(const double i, const double j, const double k) : r(0.0), i(i), j(j), k(k) {}
    Quartenion(const double r, const double i, const double j, const double k) : r(r), i(i), j(j), k() {}

    Quartenion operator*(const Quartenion rhs)
    {
        double real = r * rhs.r - i * rhs.i - j * rhs.j - k * rhs.k;
        double iIm = (i * rhs.r + r * rhs.i) + (j * rhs.k - k * rhs.j);
        double jIm = (j * rhs.r + r * rhs.j) + (k * rhs.i - i * rhs.k);
        double kIm = (k * rhs.r + r * rhs.k) + (i * rhs.j - j * rhs.i);
        return Quartenion(real, iIm, jIm, kIm);
    }
    Quartenion &operator*=(const Quartenion rhs)
    {
        double real = r * rhs.r - i * rhs.i - j * rhs.j - k * rhs.k;
        double iIm = (i * rhs.r + r * rhs.i) + (j * rhs.k - k * rhs.j);
        double jIm = (j * rhs.r + r * rhs.j) + (k * rhs.i - i * rhs.k);
        double kIm = (k * rhs.r + r * rhs.k) + (i * rhs.j - j * rhs.i);
        r = real;
        i = iIm;
        j = jIm;
        k = kIm;
        return *this;
    }
    Quartenion &operator!()
    {
        r = r;
        i = -i;
        j = -j;
        k = -k;
        return *this;
    }
};