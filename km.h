#pragma once
#ifndef KM_H
#define KM_H
#include <math.h>
#include <string.h>

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
}matrix3_FLAT;

#ifndef KM_INLINE
  #ifdef __cplusplus
    #define KM_INLINE inline
  #else
    #define KM_INLINE static inline
  #endif
#endif

    KM_INLINE float floatLerp(float a, float b, float delta)
    {
        if (delta < 0)
            delta = 0;
        if (delta > 1)
            delta = 1.0f;

        float r = (1 - delta) * a + delta * b;
        return r;
    }
    
    KM_INLINE vector2 vector2Zero(void)
    {
        vector2 r = {0};
        return r;
    }

    KM_INLINE vector2 vector2Add(vector2 a, vector2 b)
    {
        vector2 r = {0};
        r.x = a.x + b.x;
        r.y = a.y + b.y;

        return r;
    }
    
    KM_INLINE vector2 vector2Sub(vector2 a, vector2 b)
    {
        vector2 r = {0};
        r.x = a.x - b.x;
        r.y = a.y - b.y;
        return r;
    }

    KM_INLINE vector2 vector2Scale(vector2 a , float scaler)
    {
        vector2 r = {0};
        r.x = a.x * scaler;
        r.y = a.y * scaler;
        return r;
    }
    

    KM_INLINE float vector2Dot(vector2 a, vector2 b)
    {
        float r = a.x * b.x + a.y * b.y;
        return r;
    }
    
    KM_INLINE float vector2Cross(vector2 a , vector2 b)
    {
        float r = a.x * b.y - a.y * b.x;
        return r;
    }
    KM_INLINE float vector2Length(vector2 a)
    {
        float r = sqrtf(a.x * a.x + a.y * a.y);
        return r;
    }

    KM_INLINE vector2 vector2Normalize(vector2 a)
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

    KM_INLINE vector2 vector2Lerp(vector2 a, vector2 b, float delta)
    {
        vector2 r;
        r.x = a.x + delta * (b.x - a.x);
        r.y = a.y + delta * (b.y - a.y);
        return r;
    }


    KM_INLINE float vector2Distance(vector2 a , vector2 b)
    {
        vector2 sub = vector2Sub(a, b);
        return vector2Length(sub);
    }

    KM_INLINE vector2 vector2Lookat(vector2 a, vector2 b)
    {
        vector2 r = {0};
        r = vector2Sub(b, a);
        return vector2Normalize(r);
    }

    KM_INLINE matrix3 m3identity()
    {
        matrix3 m = {{{1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f}}};
        return m;
    }  

    KM_INLINE matrix3_FLAT m3toPlain(matrix3 mat)
    {
        matrix3_FLAT r = {0};
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                r.value[i * 3 + j] = mat.value[i][j];
            }
        }
        return r;
    }

    KM_INLINE matrix3 mult(matrix3 a , matrix3 b)
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

    KM_INLINE matrix3 m3translate(matrix3 mat,vector2 position)
    {
        matrix3 m = {{{1, 0, position.x},
                   {0, 1, position.y},
                   {0, 0, 1}}};

        return mult(m , mat);
    }

    KM_INLINE matrix3 m3rotate(matrix3 mat, float deg)
    {
        float r = deg * 3.141596f  / 180.0f;
        float c = cosf(r);
        float s = sinf(r);

        matrix3 m = {{{c, -s, 0.0f},
                   {s, c, 0.0f},
                   {0.0f, 0.0f, 1.0f}}};
        
        return mult(m, mat);
    }

    KM_INLINE matrix3 m3scale(matrix3 mat, vector2 scale)
    {
        matrix3 m = {{{scale.x, 0.0f, 0.0f},
                   {0.0f, scale.y, 0.0f},
                   {0.0f, 0.0f, 1.0f}}};
        return mult(m , mat);
    }

    KM_INLINE matrix3 m3ortho(int width, int height)
    {
        matrix3 r = {{{ 2.0f / width,  0.0f,          -1.0f },
                { 0.0f,         -2.0f / height,  1.0f },
                { 0.0f,          0.0f,           1.0f }}};
        return r;
    }

    KM_INLINE matrix3 m3lookat(matrix3 mat, vector2 dir)
    {
        float angle = atan2f(dir.y, dir.x) * 180.0f / 3.141596f;
        return m3rotate(mat, angle);
    }
    
    KM_INLINE float *value_ptr(matrix3 *mat)
    {
        return &mat->value[0][0];
    }
}

#ifdef __cplusplus
struct vec2 : public vector2 {
    vec2(float x = 0.0f, float y = 0.0f)
    {
        this->x = x;
        this->y = y; 
    }
    vec2(const vector2 &v)
    {
        this->x = v.x;
        this->y = v.y; 
    }
    static vec2 zero()
    {
        return vec2(0.0f, 0.0f);
    }
    vec2 operator+(const vec2 &o) const
    {
        return vec2(x + o.x, y + o.y);
    }
    vec2 operator-(const vec2 &o) const
    {
        return vec2(x-o.x, y-o.y);
    }
    vec2 operator/(float s) const
    {
        return vec2(x / s, y / s);
    }
    vec2 operator*(float s) const
    {
        return vec2(x * s, y * s);
    }
    vec2 &operator=(const vec2 &o)
    {
        if (this != &o) { 
            x = o.x;
            y = o.y;
        }
        return *this;
    }
    vec2 &operator+=(const vec2 &o)
    {
        x+=o.x; y+=o.y; 
        return *this; 
    }
    vec2 &operator-=(const vec2 &o)
    {
        x-=o.x; y-=o.y; 
        return *this;
    }
    vec2 normalize() const
    {
        vec2 norm =  vector2Normalize(vector2{this->x , this->y});
        return norm;
    }
    float length() const
    {
        float len = vector2Length(vector2{this->x, this->y});
        return len;
    }
    float dot(const vec2& v) const
    {
        float dot = vector2Dot(vec2(x, y), v);
        return dot;
    }
    float cross(const vec2& v) const
    {
        float cross = vector2Cross(vec2(x, y), v);
        return cross;
    }
    float distance(const vec2& v) const
    {
        float dist = vector2Distance(vec2(x, y), v);
        return dist;
    }
    vec2 lerp(const vec2& v, float t) const
    {
        vec2 vl = vector2Lerp(vec2(x, y), v, t);
        return vl;
    }
};


struct mat3: public matrix3
{
    mat3() = default;
    mat3(const matrix3& m) : matrix3(m) {}
    static mat3 identity()
    {
        return m3identity();
    }
    mat3 traslate(const vec2& v) const
    {
        return m3translate(*this, v);
    }
    mat3 rotate(const float d) const
    {
        return m3rotate(*this, d);
    }
    mat3 scale(const vec2& v) const
    {
        return m3scale(*this, v);
    }
    matrix3_FLAT flat() const 
    {
        matrix3_FLAT m = {0};
        m.value[0] = this->value[0][0];
        m.value[1] = this->value[1][0];
        m.value[2] = this->value[2][0];

        m.value[3] = this->value[0][1];
        m.value[4] = this->value[1][1];
        m.value[5] = this->value[2][1];

        m.value[6] = this->value[0][2];
        m.value[7] = this->value[1][2];
        m.value[8] = this->value[2][2];

        return m;
    }
    float* value_ptr() 
    {
        return &this->value[0][0];
    }
};
#endif

#endif // KM_H