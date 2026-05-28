#include <LGDL/Math.h>

#include <iostream>

namespace LGDL
{

    // float

    float min(float a, float b)
    {
        if(a < b) return a;
        return b;
    }

    float max(float a, float b)
    {
        if(a > b) return a;
        return b;
    }

    float mod(float a, float b) // this is the best modulo that I could think of
    {
        float v = a / b;
        int f = floor(v);
        float r = v - f;

        //std::cout << r*b << "\n";

        return r * b;
    }


    // Mat3

    Mat3 Identity()
    {
        return Mat3{
            {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1}
            }
        };
    }

    Mat3 Translate(const Vec2& v)
    {
        Mat3 r = Identity();

        r.m[0][2] = v.x;
        r.m[1][2] = v.y;

        return r;
    }

    Mat3 Scale(const Vec2& s)
    {
        Mat3 r = Identity();

        r.m[0][0] = s.x;
        r.m[1][1] = s.y;

        return r;
    }

    Mat3 Rotate(float a)
    {
        Mat3 r = Identity();

        float c = cos(a);
        float s = sin(a);

        r.m[0][0] = c;
        r.m[0][1] = -s;

        r.m[1][0] = s;
        r.m[1][1] = c;

        return r;
    }

    Mat3 Multiply(const Mat3& A, const Mat3& B)
    {
        Mat3 R = {};

        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                R.m[row][col] = 0;

                for (int k = 0; k < 3; k++)
                {
                    R.m[row][col] += A.m[row][k] * B.m[k][col];
                }
            }
        }

        return R;
    }

    // Vec2

    Vec2 operator+(const Vec2& a, const Vec2& b)
    {
        Vec2 r;

        r.x = a.x + b.x;
        r.y = a.y + b.y;

        return r;
    }

    Vec2& operator+=(Vec2& a, const Vec2& b)
    {
        a.x += b.x;
        a.y += b.y;

        return a;
    }

    Vec2 operator-(const Vec2& a, const Vec2& b)
    {
        Vec2 r;

        r.x = a.x - b.x;
        r.y = a.y - b.y;

        return r;
    }

    Vec2 operator*(const Vec2& v, float s)
    {
        Vec2 r;

        r.x = v.x * s;
        r.y = v.y * s;

        return r;
    }

    Vec2 operator*(const Vec2& a, const Vec2& b)
    {
        Vec2 r;

        r.x = a.x * b.x;
        r.y = a.y * b.y;

        return r;
    }

    float VMag(const Vec2& v)
    {
        return hypot(v.x, v.y);
    }

    float VDir(const Vec2& v)
    {
        return atan2(v.y, v.x);
    }
}