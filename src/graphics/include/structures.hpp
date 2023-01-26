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

    Point operator&(const Point rhs)
    {
        return Point(this->y * rhs.z - this->z * rhs.y, this->z * rhs.x - this->x * rhs.z, this->x * rhs.y - this->y * rhs.x);
    }
    double operator|(const Point rhs)
    {
        return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
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

struct Quaternion
{
public:
    double r, i, j, k;

    Quaternion() : r(0.0), i(0.0), j(0.0), k(0.0) {}
    Quaternion(const double r) : r(r), i(0.0), j(0.0), k(0.0) {}
    Quaternion(const double i, const double j, const double k) : r(0.0), i(i), j(j), k(k) {}
    Quaternion(const double r, const double i, const double j, const double k) : r(r), i(i), j(j), k(k) {}

    Quaternion operator*(const Quaternion rhs)
    {
        double real = r * rhs.r - i * rhs.i - j * rhs.j - k * rhs.k;
        double iImg = r * rhs.i + i * rhs.r - j * rhs.k + k * rhs.j;
        double jImg = r * rhs.j + i * rhs.k + j * rhs.r - k * rhs.i;
        double kImg = r * rhs.k - i * rhs.j + j * rhs.i + k * rhs.r;
        return Quaternion(real, iImg, jImg, kImg);
    }
    Quaternion &operator*=(const Quaternion rhs)
    {
        double real = r * rhs.r - i * rhs.i - j * rhs.j - k * rhs.k;
        double iImg = r * rhs.i + i * rhs.r - j * rhs.k + k * rhs.j;
        double jImg = r * rhs.j + i * rhs.k + j * rhs.r - k * rhs.i;
        double kImg = r * rhs.k - i * rhs.j + j * rhs.i + k * rhs.r;
        r = real;
        i = iImg;
        j = jImg;
        k = kImg;
        return *this;
    }
    Quaternion operator!()
    {
        return Quaternion(r,-i,-j,-k);
    }
};