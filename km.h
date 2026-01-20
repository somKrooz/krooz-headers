/* Copyright (c) 2026 Som Krooz
 * This is a header-only math library designed specifically for 2D calculations.
 * 
 * vector2: 2D vectors for positions, directions etc
 * matrix3: 3x3 matrices for 2D transformations mainly
 *
 * All functions are inline and compatible with C and C++.
 * Intended for small 2D engines, games etc
 *
 * Usage:
 *   #define KM_IMPLEMENTATION
 *   #include "km.h"
 *   in *one* source file to generate the implementations.
 */

#ifndef KM_H
#define KM_H
#ifdef __cplusplus
  #define KMAPI inline
#else
  #define KMAPI static inline
#endif

#ifndef PI
    #define PI 3.14159265358979323846f
#endif

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct {
        float value[3][3];
    } matrix3;

    typedef struct {
        float x;
        float y;
    } vector2;

    typedef struct {
        float r;
        float g;
        float b;
    } vec3;

    typedef struct {
        float value[9];
    }FLATmatrix3;

    KMAPI float floatLerp(float a, float b, float delta);
    KMAPI vector2 vector2Zero(void);
    KMAPI vector2 vector2Add(vector2 a, vector2 b);
    KMAPI vector2 vector2Normalize(vector2 a);
    KMAPI vector2 vector2Lookat(vector2 a, vector2 b);
    KMAPI vector2 vector2Sub(vector2 a, vector2 b);
    KMAPI vector2 vector2Scale(vector2 a, float scaler);
    KMAPI vector2 vector2Lerp(vector2 a, vector2 b, float delta);
    KMAPI float vector2Dot(vector2 a, vector2 b);
    KMAPI float vector2Cross(vector2 a, vector2 b);
    KMAPI float vector2Length(vector2 a);
    KMAPI float vector2Distance(vector2 a, vector2 b);

    KMAPI matrix3 m3identity();
    KMAPI matrix3 m3translate(matrix3 mat, vector2 position);
    KMAPI matrix3 m3rotate(matrix3 mat, float deg);
    KMAPI matrix3 m3lookat(matrix3 mat, vector2 dir);
    KMAPI matrix3 m3scale(matrix3 mat, vector2 scale);
    KMAPI matrix3 m3ortho(int width, int height);
    KMAPI FLATmatrix3 m3toPlain(matrix3 mat);
    KMAPI float *value_ptr(matrix3 *mat);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
struct vec2 : public vector2 {
    vec2(float x = 0.0f, float y = 0.0f);
    vec2(const vector2 &v);
    vec2 operator+(const vec2 &o) const;
    vec2 operator-(const vec2 &o) const;
    vec2 operator*(float s) const;
    vec2 &operator+=(const vec2 &o);
    vec2 &operator-=(const vec2 &o);
    vec2 normalize() const;
    float length() const;
    float dot(const vec2& v) const;
    float cross(const vec2& v) const;
    float distance(const vec2& v) const;
};
struct mat3 : public matrix3 {
    //TODO
};
#endif

// #define KM_IMPLEMENTATION 
#ifdef KM_IMPLEMENTATION
    #include "math.h"
    #include "stdio.h"

    KMAPI float floatLerp(float a, float b, float delta)
    {
        if (delta < 0)
            delta = 0;
        if (delta > 1)
            delta = 1.0f;

        float r = (1 - delta) * a + delta * b;
        return r;
    }
    
    KMAPI vector2 vector2Zero(void)
    {
        vector2 r = {0};
        return r;
    }

    KMAPI vector2 vector2Add(vector2 a, vector2 b)
    {
        vector2 r = {0};
        r.x = a.x + b.x;
        r.y = a.y + b.y;

        return r;
    }
    
    KMAPI vector2 vector2Sub(vector2 a, vector2 b)
    {
        vector2 r = {0};
        r.x = a.x - b.x;
        r.y = a.y - b.y;
        return r;
    }

    KMAPI vector2 vector2Scale(vector2 a , float scaler)
    {
        vector2 r = {0};
        r.x = a.x * scaler;
        r.y = a.y * scaler;
        return r;
    }
    
    KMAPI vector2 vector2Normalize(vector2 a)
    {
        vector2 r = {0};
        float len = vector2Length(a);
        if(len > 0)
        {
            float ilen = 1.0f / len;
            r.x = a.x * ilen;
            r.y = a.y * ilen;
        }
        return r;
    }

    KMAPI float vector2Dot(vector2 a, vector2 b)
    {
        float r = a.x * b.x + a.y * b.y;
        return r;
    }
    
    KMAPI float vector2Cross(vector2 a , vector2 b)
    {
        float r = a.x * b.y - a.y * b.x;
        return r;
    }
    KMAPI float vector2Length(vector2 a)
    {
        float r = sqrtf(a.x * a.x + a.y * a.y);
        return r;
    }
    
    KMAPI vector2 vector2Lerp(vector2 a, vector2 b, float delta)
    {
        vector2 r;
        r.x = a.x + delta * (b.x - a.x);
        r.y = a.y + delta * (b.y - a.y);
        return r;
    }


    KMAPI float vector2Distance(vector2 a , vector2 b)
    {
        vector2 sub = vector2Sub(a, b);
        return vector2Length(sub);
    }

    KMAPI vector2 vector2Lookat(vector2 a, vector2 b)
    {
        vector2 r = {0};
        r = vector2Sub(b, a);
        return vector2Normalize(r);
    }

    KMAPI void vector2Print(vector2 a)
    {
        printf("(%.1f , %.1f)\n", a.x, a.y);
    }

    #ifdef __cplusplus /// vec2 cpp implementation (start)
    vec2::vec2(float x, float y) 
    {
        this->x = x;
        this->y = y; 
    }

    vec2::vec2(const vector2 &v)
    {
        this->x = v.x;
        this->y = v.y; 
    }
    
    KMAPI vec2 vec2::operator+(const vec2 &o) const 
    { 
        return  vec2(x+o.x, y+o.y);
    }

    KMAPI vec2 vec2::operator-(const vec2 &o) const 
    { 
        return vec2(x-o.x, y-o.y);
    }
    
    KMAPI vec2 vec2::operator*(float s) const { 
        return  vec2(x*s, y*s); 
    }

    KMAPI vec2& vec2::operator+=(const vec2 &o) 
    { 
        x+=o.x; y+=o.y; return *this; 
    }
    
    KMAPI vec2& vec2::operator-=(const vec2 &o) 
    { 
        x-=o.x; y-=o.y; return *this; 
    }

    KMAPI float vec2::length() const
    { 
        return vector2Length(vector2{ this->x, this->y });
    }

    KMAPI float vec2::distance(const vec2 &v) const
    {
        return vector2Distance(vec2(x, y), v);
    }

    KMAPI vec2 vec2::normalize() const
    {
        return vector2Normalize({this->x , this->y});
    }

    KMAPI float vec2::dot(const vec2& v) const
    {
        return vector2Dot(vec2(x, y), v);
    }

    KMAPI float vec2::cross(const vec2& v) const
    {
        return vector2Cross(vec2(x, y), v);
    }
    #endif

    KMAPI matrix3 m3identity()
    {
        matrix3 m = {{{1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f}}};
        return m;
    }  

    KMAPI FLATmatrix3 m3toPlain(matrix3 mat)
    {
        FLATmatrix3 r = {0};
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                r.value[i * 3 + j] = mat.value[i][j];
            }
        }
        return r;
    }

    KMAPI matrix3 mult(matrix3 a , matrix3 b)
    {
        matrix3 result = {0};
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                result.value[row][col] = 
                    a.value[row][0] * b.value[0][col] +
                    a.value[row][1] * b.value[1][col] +
                    a.value[row][2] * b.value[2][col];

            }
        }
        return result;
    }

    KMAPI matrix3 m3translate(matrix3 mat,vector2 position)
    {
        matrix3 m = {{{1, 0, position.x},
                   {0, 1, position.y},
                   {0, 0, 1}}};

        return mult(m , mat);
    }

    KMAPI matrix3 m3rotate(matrix3 mat, float deg)
    {
        float r = deg * PI / 180.0f;
        float c = cosf(r);
        float s = sinf(r);

        matrix3 m = {{{c, -s, 0.0f},
                   {s, c, 0.0f},
                   {0.0f, 0.0f, 1.0f}}};
        
        return mult(m, mat);
    }

    KMAPI matrix3 m3scale(matrix3 mat, vector2 scale)
    {
        matrix3 m = {{{scale.x, 0.0f, 0.0f},
                   {0.0f, scale.y, 0.0f},
                   {0.0f, 0.0f, 1.0f}}};
        return mult(m , mat);
    }

    KMAPI matrix3 m3ortho(int width, int height)
    {
        matrix3 r = {{{ 2.0f / width,  0.0f,          -1.0f },
                { 0.0f,         -2.0f / height,  1.0f },
                { 0.0f,          0.0f,           1.0f }}};
        return r;
    }

    KMAPI matrix3 m3lookat(matrix3 mat, vector2 dir)
    {
        float angle = atan2f(dir.y, dir.x) * 180.0f / PI;
        return m3rotate(mat, angle);
    }
    
    KMAPI float *value_ptr(matrix3 *mat)
    {
        return &mat->value[0][0];
    }
#endif // KM_IMPLEMENTATION
#endif // KM_H