//
//  Vec3.h
//  Project
//
//  Created by Lucas Le Faucheur on 2018-06-05.
//  Copyright © 2018 Lucas Le Faucheur. All rights reserved.
//

#ifndef Vec3_h
#define Vec3_h

class Vec3 {
public:
    float x, y, z;
    Vec3() : x(float(0)), y(float(0)), z(float(0)) {}
    Vec3(float xx) : x(xx), y(xx), z(xx) {}
    Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vec3& normalize() {
        float nor2 = length2();
        if (nor2 > 0) {
            float invNor = 1 / sqrt(nor2);
            x *= invNor;
            y *= invNor;
            z *= invNor;
        }
        return *this;
    }
    
    // Operators overloading
    Vec3 operator * (const float &f) const {
        return Vec3(x * f, y * f, z * f);
    }
    Vec3 operator * (const Vec3 &v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    float dot(const Vec3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    Vec3 operator - (const Vec3 &v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator + (const Vec3 &v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3& operator += (const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3& operator *= (const Vec3 &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vec3& operator /= (const Vec3 &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    Vec3 operator - () const {
        return Vec3(-x, -y, -z);
    }
    float length2() const {
        return x * x + y * y + z * z;
    }
    float length() const {
        return sqrt(length2());
    }
};

Vec3 power(Vec3 v, double p) {
    return Vec3((float)pow(v.x, p), (float)pow(v.y, p), (float)pow(v.z, p));
}

float max(float x, float y) {
    if (x > y) return x;
    else return y;
}

#endif /* Vec3_h */
