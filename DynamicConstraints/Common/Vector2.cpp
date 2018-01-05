#include "Vector2.h"
#include <math.h>


const Vector2& Vector2::Zero = Vector2(0, 0);
const Vector2& Vector2::One = Vector2(1, 1);

const Vector2& Vector2::Up = Vector2(0, -1);
const Vector2& Vector2::Right = Vector2(1, 0);
const Vector2& Vector2::Down = Vector2(0, 1);
const Vector2& Vector2::Left = Vector2(-1, 0);


Vector2 operator*(const Vector2& vector, float scalar)
{
    return Vector2(vector.x * scalar, vector.y * scalar);
}

Vector2 operator/(const Vector2& vector, float scalar)
{
    return Vector2(vector.x / scalar, vector.y / scalar);
}


Vector2::
Vector2()
: Vector2(0, 0)
{
}

    
Vector2::
Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2::
Vector2(const Vector2& other)
{
    this->x = other.x;
    this->y = other.y;
}

Vector2::
~Vector2()
{
    
}

float
Vector2::
getSqrMagnitude() const
{
    return Vector2::Dot(*this, *this);
}

float
Vector2::
getMagnitude() const
{
    return sqrt(getSqrMagnitude());
}

void
Vector2::
Normalize()
{
    float magnitude = getMagnitude();
    this->x /= magnitude;
    this->y /= magnitude;
}

void 
Vector2::
Rotate(float angle)
{
    x = x * cos(angle) - y * sin(angle);
    y = x * sin(angle) + y * cos(angle);
}

void
Vector2::
Reset()
{
    x = 0;
    y = 0;
}

Vector2 
Vector2::
Normalize(const Vector2& v)
{
    return Vector2(v / v.getMagnitude());
}

Vector2
Vector2::
Scale(const Vector2& a, const Vector2& b)
{
    return Vector2(a.x * b.x, a.y * b.y);
}

float 
Vector2::
Dot(const Vector2& a, const Vector2& b)
{
    return (a.x * b.x) + (a.y * b.y);
}
    
float 
Vector2::
SqrDistance(const Vector2& a, const Vector2& b)
{
    float dx = (a.x - b.x);
    float dy = (a.y - b.y);

    return (dx * dx) + (dy * dy);
}

float
Vector2::
Distance(const Vector2& a, const Vector2& b)
{
    return sqrt(SqrDistance(a, b));
}

float
Vector2::
Angle(const Vector2& a, const Vector2& b)
{
    return atan2(a.y, a.x) - atan2(b.y, b.x);
}

Vector2
Vector2::
operator+(const Vector2& other)
{
    return Vector2(x + other.x, y + other.y);
}
    
Vector2 
Vector2::
operator-(const Vector2& other)
{
    return Vector2(x - other.x, y - other.y);
}

Vector2&
Vector2::
operator+=(const Vector2& other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vector2&
Vector2::
operator-=(const Vector2& other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}


bool 
Vector2::
operator==(const Vector2& other)
{
    return x == other.x && y == other.y;
}
    
bool 
Vector2::
operator!=(const Vector2& other)
{
    return !(*this == other);
}