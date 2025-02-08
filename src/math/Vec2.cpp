#include "glib/math/Vec2.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace glib;

glib::Vec2::Vec2() : x(0.0f), y(0.0f)
{
}

glib::Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

glib::Vec2::Vec2(float v) : x(v), y(v)
{
}

Vec2 glib::Vec2::operator+(const Vec2& other) {
    return Vec2(x + other.x, y + other.y);
}

Vec2 glib::Vec2::operator+(float v)
{
    return Vec2(x + v, y + v);
}

Vec2 glib::Vec2::operator-(const Vec2& other)
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 glib::Vec2::operator-(float v)
{
    return Vec2(x - v, y - v);
}

Vec2 glib::Vec2::operator*(const Vec2& other)
{
    return Vec2(x * other.x, y * other.y);
}

Vec2 glib::Vec2::operator*(float v)
{
    return Vec2(x * v, y * v);
}

Vec2 glib::Vec2::operator/(const Vec2& other)
{
    return Vec2(x / other.x, y / other.y);
}

Vec2 glib::Vec2::operator/(float v)
{
    return Vec2(x / v, y / v);
}

Vec2 glib::Vec2::operator^(const Vec2& other)
{
    return Vec2(powf(x, other.x), powf(y, other.y));
}

Vec2 glib::Vec2::operator^(float v)
{
    return Vec2(powf(x, v), powf(y, v));
}

void glib::Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
}

void glib::Vec2::operator+=(float v)
{
    x += v;
    y += v;
}

void glib::Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
}

void glib::Vec2::operator-=(float v)
{
    x -= v;
    y -= v;
}

void glib::Vec2::operator*=(const Vec2& other)
{
    x *= other.x;
    y *= other.y;
}

void glib::Vec2::operator*=(float v)
{
    x *= v;
    y *= v;
}

void glib::Vec2::operator/=(const Vec2& other)
{
    x /= other.x;
    y /= other.y;
}

void glib::Vec2::operator/=(float v)
{
    x /= v;
    y /= v;
}

void glib::Vec2::operator^=(const Vec2& other)
{
    x = powf(x, other.x);
    y = powf(y, other.y);
}

void glib::Vec2::operator^=(float v)
{
    x = powf(x, v);
    y = powf(y, v);
}

bool glib::Vec2::operator==(const Vec2& other)
{
    return x == other.x && y == other.y;
}

Vec2 glib::Vec2::Normalize()
{
    float length = std::sqrt(x * x + y * y);

    if (length == 0.0f) {
        return Vec2(0.0f, 0.0f);
    }

    return Vec2(x / length, y / length);
}

float glib::Vec2::Distance(const Vec2& other)
{
    return sqrtf(powf((other.x - x), 2) + powf(other.y - y, 2));
}

Vec2 glib::Vec2::Absolute()
{
    return { abs(x), abs(y) };
}

std::string glib::Vec2::ToString()
{
    return "(" + std::to_string(x) + " | " + std::to_string(y) + ")";
}
