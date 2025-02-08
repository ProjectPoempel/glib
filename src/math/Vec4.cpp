#include "glib/math/Vec4.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glib;

glib::Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

glib::Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vec4 glib::Vec4::operator+(const Vec4& other) {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vec4 glib::Vec4::operator+(float v)
{
    return Vec4(x + v, y + v, z + v, w + v);
}

Vec4 glib::Vec4::operator-(const Vec4& other)
{
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vec4 glib::Vec4::operator-(float v)
{
    return Vec4(x - v, y - v, z - v, w - v);
}

Vec4 glib::Vec4::operator*(const Vec4& other)
{
    return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vec4 glib::Vec4::operator*(float v)
{
    return Vec4(x * v, y * v, z * v, w * v);
}

Vec4 glib::Vec4::operator/(const Vec4& other)
{
    return Vec4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vec4 glib::Vec4::operator/(float v)
{
    return Vec4(x / v, y / v, z / v, w / v);
}

Vec4 glib::Vec4::operator^(const Vec4& other)
{
    return Vec4(powf(x, other.x), powf(y, other.y), powf(z, other.z), powf(w, other.w));
}

Vec4 glib::Vec4::operator^(float v)
{
    return Vec4(powf(x, v), powf(y, v), powf(z, v), powf(w, v));
}

void glib::Vec4::operator+=(const Vec4& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
}

void glib::Vec4::operator+=(float v)
{
    x += v;
    y += v;
    z += v;
    w += v;
}

void glib::Vec4::operator-=(const Vec4& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
}

void glib::Vec4::operator-=(float v)
{
    x -= v;
    y -= v;
    z -= v;
    w -= v;
}

void glib::Vec4::operator*=(const Vec4& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
}

void glib::Vec4::operator*=(float v)
{
    x *= v;
    y *= v;
    z *= v;
    w *= v;
}

void glib::Vec4::operator/=(const Vec4& other)
{
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
}

void glib::Vec4::operator/=(float v)
{
    x /= v;
    y /= v;
    z /= v;
    w /= v;
}

void glib::Vec4::operator^=(const Vec4& other)
{
    x = powf(x, other.x);
    y = powf(y, other.y);
    z = powf(z, other.z);
    w = powf(w, other.w);
}

void glib::Vec4::operator^=(float v)
{
    x = powf(x, v);
    y = powf(y, v);
    z = powf(z, v);
    w = powf(w, v);
}

Vec4 glib::Vec4::operator-()
{
    return Vec4(-x, -y, -z, -w);
}

Vec4 glib::Vec4::Normalize()
{
    float length = sqrt(x * x + y * y + z * z + w * w);
    if (length == 0.0f) return Vec4();
    return Vec4(x / length, y / length, z / length, w / length);
}

float glib::Vec4::Distance(const Vec4& other)
{
    return sqrtf(powf((other.x - x), 2) + powf(other.y - y, 2));
}

Vec2 glib::Vec4::XY()
{
    return Vec2(x, y);
}

Vec2 glib::Vec4::ZW()
{
    return Vec2(z, w);
}
