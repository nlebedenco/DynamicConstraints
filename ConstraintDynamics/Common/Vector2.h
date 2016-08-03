#pragma once

class Vector2
{
    friend Vector2 operator*(const Vector2& vector, float scalar);
    friend Vector2 operator/(const Vector2& vector, float scalar);

public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2& other);
    ~Vector2();

    float x;
    float y;

    float getSqrMagnitude() const;
    float getMagnitude() const;

    void Normalize();
    void Rotate(float angle);
    void Reset();

    static const Vector2& Zero;
    static const Vector2& One;

    static const Vector2& Up;
    static const Vector2& Right;
    static const Vector2& Down;
    static const Vector2& Left;

    static Vector2 Normalize(const Vector2& v);
    static Vector2 Scale(const Vector2& a, const Vector2& b);
    static float Dot(const Vector2& a, const Vector2& b);
    static float SqrDistance(const Vector2& a, const Vector2& b);
    static float Distance(const Vector2& a, const Vector2& b);
    static float Angle(const Vector2& a, const Vector2& b);

    Vector2 operator+(const Vector2& other);
    Vector2 operator-(const Vector2& other);

    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);

    bool operator==(const Vector2& other);
    bool operator!=(const Vector2& other); 
};







