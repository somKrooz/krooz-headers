#pragma once
#ifndef KM_H // KM_H
#define KM_H
#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float m[16];
} matrix4;

typedef struct {
    float x;
    float y;
} vector2;

typedef struct {
    float r;
    float g;
    float b;
} vector3;


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

    KM_INLINE matrix4 m4identity()
    {
        matrix4 iden = {0};
        iden.m[0] = 1;
        iden.m[5] = 1;
        iden.m[10] = 1;
        iden.m[15] = 1;

        return iden;
    }
    KM_INLINE matrix4 m4multiplication(matrix4 a , matrix4 b)
    {
        matrix4 result = {0};
        for (int row = 0; row < 4; row++){
            for (int col = 0; col < 4; col++)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; k++) 
                {
                    sum += a.m[k*4 + row] * b.m[col*4 + k];
                }
                result.m[col * 4 + row] = sum;
            }
        }
        return result;
    };

    KM_INLINE matrix4 m4Translate(matrix4 mat, vector2 v)  
    {
        matrix4 trsMat = m4identity();
        trsMat.m[12] = v.x;
        trsMat.m[13] = v.y;
        return m4multiplication(mat, trsMat);
    }

    matrix4 m4Rotate(matrix4 mat, float rot)
    {
        float c = cosf(rot);
        float s = sinf(rot);

        matrix4 rotMat = m4identity();
        rotMat.m[0] = c; 
        rotMat.m[1] = s;
        rotMat.m[4] = -s; 
        rotMat.m[5] = c;
        return m4multiplication(mat , rotMat);
    }


    KM_INLINE matrix4 m4Scale(matrix4 mat,  vector2 v) 
    {
        matrix4 scaleMat = m4identity();
        scaleMat.m[0] = v.x;
        scaleMat.m[5] = v.y;

        return m4multiplication(mat, scaleMat);
    };
    
    KM_INLINE matrix4 m4ortho(float l, float r, float b, float t)
    {
        float f = 1;
        float n = -1;
        matrix4 ortho = m4identity();
        ortho.m[0] = 2 / (r - l);
        ortho.m[5] = 2 / (t - b);
        ortho.m[10] = -2 / (f - n);
		ortho.m[12] = -(r + l) / (r - l);
		ortho.m[13] = -(t + b) / (t - b);
		ortho.m[14] = -(f + n) / (f - n);

        return ortho;
    }

#ifdef __cplusplus
}

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
    vec2 operator-() const 
    {
        return vec2(-x, -y);
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

struct mat4 : public matrix4
{
    mat4() = default;
    mat4(const matrix4& m) : matrix4(m) {}  
    mat4 operator*(const matrix4& m) const
    {
        return m4multiplication(m, *this);
    }
    static mat4 identity()
    {
        return m4identity();
    }
    mat4 translate(const vec2& v) const
    {
        return m4Translate(*this, v);
    }
    mat4 scale(const vec2& v) const
    {
        return m4Scale(*this, v);
    }
    mat4 rotate(float d) const
    {
        return m4Rotate(*this, d);
    }
    static mat4 ortho(float l, float r, float b, float t)
    {
        return m4ortho(l, r, b, t);
    }
    
    float* value_ptr(){
        return &this->m[0];
    }
};

#endif

#endif // KM_H