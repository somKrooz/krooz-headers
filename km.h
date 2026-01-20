/* Copyright (c) 2026 Som Krooz
 * This is a header-only math library designed specifically for 2D calculations.
 * 
 * vec2: 2D vectors for positions, directions etc
 * mat3: 3x3 matrices for 2D transformations mainly
 *
 * All functions are inline and compatible with C and C++.
 * Intended for small 2D engines, games etc
 *
 * Usage:
 *   #define KM_IMPLEMENTATION
 *   #include "km.h"
 *   in *one* source file to generate the implementations.
 */

#ifndef _KM_H
#define _KM_H

#ifndef KMAPI
#define KMAPI static inline
#endif

#define PI 3.14159265359

typedef struct {
    float value[3][3];
} mat3;

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float r;
    float g;
    float b;
} vec3;

KMAPI float floatLerp(float a, float b, float delta);
KMAPI vec2 vec2Zero(void);
KMAPI vec2 vec2Add(vec2 a, vec2 b);
KMAPI vec2 vec2Normalize(vec2 a);
KMAPI vec2 vec2Lookat(vec2 a, vec2 b);
KMAPI vec2 vec2Sub(vec2 a, vec2 b);
KMAPI vec2 vec2Scale(vec2 a, float scaler);
KMAPI vec2 vec2Learp(vec2 a, vec2 b, float delta);
KMAPI float vec2Dot(vec2 a, vec2 b);
KMAPI float vec2Cross(vec2 a, vec2 b);
KMAPI float vec2Length(vec2 a);
KMAPI float vec2Distance(vec2 a, vec2 b);

KMAPI mat3 m3identity();
KMAPI mat3 m3translate(mat3 mat, vec2 position);
KMAPI mat3 m3rotate(mat3 mat, float deg);
KMAPI mat3 m3lookat(mat3 mat, vec2 dir);
KMAPI mat3 m3scale(mat3 mat, vec2 scale);
KMAPI mat3 m3ortho(int width, int height);
KMAPI mat3 m3transpose(mat3 mat);
KMAPI float *value_ptr(mat3 *mat);

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
    
    KMAPI vec2 vec2Zero(void)
    {
        vec2 r = {0};
        return r;
    }

    KMAPI vec2 vec2Add(vec2 a, vec2 b)
    {
        vec2 r = {0};
        r.x = a.x + b.x;
        r.y = a.y + b.y;

        return r;
    }
    
    KMAPI vec2 vec2Sub(vec2 a, vec2 b)
    {
        vec2 r = {0};
        r.x = a.x - b.x;
        r.y = a.y - b.y;
        return r;
    }

    KMAPI vec2 vec2Scale(vec2 a , float scaler)
    {
        vec2 r = {0};
        r.x = a.x * scaler;
        r.y = a.y * scaler;
        return r;
    }
    
    KMAPI vec2 vec2Normalize(vec2 a)
    {
        vec2 r = {0};
        float len = vec2Length(a);
        if(len > 0)
        {
            float ilen = 1.0f / len;
            r.x = a.x * ilen;
            r.y = a.y * ilen;
        }
        return r;
    }

    KMAPI float vec2Dot(vec2 a, vec2 b)
    {
        float r = a.x * b.x + a.y * b.y;
        return r;
    }
    
    KMAPI float vec2Cross(vec2 a , vec2 b)
    {
        float r = a.x * b.y - a.y * b.x;
        return r;
    }
    KMAPI float vec2Length(vec2 a)
    {
        float r = sqrtf(a.x * a.x + a.y * a.y);
        return r;
    }
    
    KMAPI vec2 vec2Learp(vec2 a, vec2 b , float delta)
    {
        vec2 r = {0};
        r.x = a.x + delta * (b.x - a.x);
        r.x = a.y + delta * (b.y - a.y);
    }

    KMAPI float vec2Distance(vec2 a , vec2 b)
    {
        vec2 sub = vec2Sub(a, b);
        return vec2Length(sub);
    }

    KMAPI vec2 vec2Lookat(vec2 a, vec2 b)
    {
        vec2 r = {0};
        r = vec2Sub(b, a);
        return vec2Normalize(r);
    }

    KMAPI void vec2Print(vec2 a)
    {
        printf("(%.1f , %.1f)\n", a.x, a.y);
    }


    KMAPI mat3 m3identity()
    {
        mat3 m = {{{1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f}}};
        return m;
    }  
        
    KMAPI mat3 mult(mat3 a , mat3 b)
    {
        mat3 result = {0};
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

    KMAPI mat3 m3translate(mat3 mat,vec2 position)
    {
        mat3 m = {{{1, 0, position.x},
                   {0, 1, position.y},
                   {0, 0, 1}}};

        return mult(m , mat);
    }

    KMAPI mat3 m3rotate(mat3 mat, float deg)
    {
        float r = deg * PI / 180.0f;
        float c = cosf(r);
        float s = sinf(r);

        mat3 m = {{{c, -s, 0.0f},
                   {s, c, 0.0f},
                   {0.0f, 0.0f, 1.0f}}};
        
        return mult(m, mat);
    }

    KMAPI mat3 m3scale(mat3 mat, vec2 scale)
    {
        mat3 m = {{{scale.x, 0.0f, 0.0f},
                   {0.0f, scale.y, 0.0f},
                   {0.0f, 0.0f, 1.0f}}};
        return mult(m , mat);
    }

    KMAPI mat3 m3ortho(int width, int height)
    {
        mat3 r = {{{ 2.0f / width,  0.0f,          -1.0f },
                { 0.0f,         -2.0f / height,  1.0f },
                { 0.0f,          0.0f,           1.0f }}};
        return r;
    }

    KMAPI mat3 m3lookat(mat3 mat, vec2 dir)
    {
        float angle = atan2f(dir.y, dir.x) * 180.0f / PI;
        return m3rotate(mat, angle);
    }

    KMAPI mat3 m3transpose(mat3 mat)
    {
        return mat3{{
            {mat.value[0][0],mat.value[1][0],mat.value[2][0]},
            {mat.value[0][1],mat.value[1][1],mat.value[2][1]},
            {mat.value[0][2],mat.value[1][2],mat.value[2][2]},
        }};
    }
    
    KMAPI float *value_ptr(mat3 *mat)
    {
        return &mat->value[0][0];
    }

#endif // KM_IMPLEMENTATION
#endif // KM_H