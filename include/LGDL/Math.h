#pragma once

#include <cmath>

#include <LGDL/Types.h>



namespace LGDL
{
    //constants

    constexpr float PI = 3.1415926f;
    constexpr float tPI = 6.2831852;

    //angles

    float Degrees(float degrees); //degrees to radians
    float Radians(float radians); //radians to degrees

    // logical reason for this naming scheme:
    // the use of these functions are assumed to be in an expression that requires the other
    // ex. sin() takes in Radians, so sin(Degrees(90)) tells you that degrees are being input rather than radians

    // float

    float min(float a, float b);
    float max(float a, float b);
    float mod(float a, float b);

    // Mat3

    Mat3 Identity();

    Mat3 Translate(const Vec2& v);

    Mat3 Scale(const Vec2& s);

    Mat3 Rotate(float a);

    Mat3 Multiply(const Mat3& A, const Mat3& B);

    // Vec2

    Vec2 operator+(const Vec2& a, const Vec2& b);
    Vec2& operator+=(Vec2& a, const Vec2& b);

    Vec2 operator-(const Vec2& a, const Vec2& b);

    Vec2 operator*(const Vec2& v, float s);
    Vec2 operator*(const Vec2& a, const Vec2& b);
    Vec2 operator/(Vec2 v, float s);

    float VMag(const Vec2& v);

    float VDir(const Vec2& v);

    Vec2 Polar(float angle, float mag);

    float DistanceSquared(Vec2 a, Vec2 b);
}

