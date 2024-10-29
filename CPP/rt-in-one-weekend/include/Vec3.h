#ifndef RT_VEC3_H
#define RT_VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3 {
public:
    Vec3 () = default;
    Vec3(const float e0, const float e1, const float e2): e{e0, e1, e2} {}
    inline float x() const {return e[0];}
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}
    inline float r() const {return e[0];}
    inline float g() const {return e[1];}
    inline float b() const {return e[2];}

    inline const Vec3& operator+() const {return *this;}
    inline Vec3 operator-() const {return Vec3(-e[0], -e[1], -e[2]);}

    inline float operator[](const int i) const {
        if (i > 2)
        {
            std::cout<<"operator[] accepts only integers less than 3."<<std::endl;
            exit(-1);
        }
        return e[i];
    }

    // why would you want a reference to a float?
    inline float& operator[] (const int i) {
        std::cout<<"calling a reference operator[]\n";
        if (i > 2)
        {
            std::cout<<"operator[] accepts only integers less than 3."<<std::endl;
            exit (-1);
        }
        return e[i];
    }

    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);

    inline float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline float squared_length() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline void make_unit_vector();

private:
    float e[3];
};

inline std::istream& operator>>(std::istream &is, Vec3 &t) {
    is >> t[0] >> t[1] >> t[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t) {
    os << t[0] << " " << t[1] << " " << t[2];
    return os;
}

inline void Vec3::make_unit_vector() {
    const float k = 1.0 / length();
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}


inline Vec3& Vec3::operator+=(const Vec3 &v2) {
    e[0] += v2[0];
    e[1] += v2[1];
    e[2] += v2[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v2) {
    e[0] -= v2[0];
    e[1] -= v2[1];
    e[2] -= v2[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v2) {
    e[0] *= v2[0];
    e[1] *= v2[1];
    e[2] *= v2[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v2) {
    e[0] /= v2[0];
    e[1] /= v2[1];
    e[2] /= v2[2];
    return *this;
}
inline Vec3& Vec3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}


inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]);
}

inline Vec3 operator*(const Vec3 &v, const float t) {
    return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator/(const Vec3&v, const float t) {
    return Vec3(v[0] / t, v[1] / t, v[2] / t);
}

// what for? multiply was already defined
// inline operator*

inline float dot(const Vec3 &v1, const Vec3 &v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
    return Vec3((v1[1] * v2[2] - v1[2]*v2[1]),
                -(v1[0] * v2[2] - v1[2] * v2[0]),
                (v1[0] * v2[1] - v1[1] * v2[0]));
}

inline Vec3 unit_vector(const Vec3 &v) {
    return v / v.length();
}

#endif  // RT_VEC3_H
