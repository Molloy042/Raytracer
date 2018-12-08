//
//  Objects.h
//  Project
//
//  Created by Lucas Le Faucheur on 2018-06-01.
//  Copyright © 2018 Lucas Le Faucheur. All rights reserved.
//

#ifndef Objects_h
#define Objects_h

#include "Vec3.h"

bool antialiasing;
bool softshadows;

class Camera {
public:
    Vec3 pos;   // position
    float fov;  // field of view
    float f;    // focal length
    float a;    // aspect ratio
    
    Camera(Vec3 pos, float fov, float f, float a)
    : pos(pos), fov(fov), f(f), a(a) {}
};
class Object {
public:
    Vec3 amb;   // ambient color of object
    Vec3 dif;   // diffuse color of object
    Vec3 spe;   // specular color of object
    float shi;  // specular shininess factor
    Object(Vec3 amb, Vec3 dif, Vec3 spe, float shi)
    : amb(amb), dif(dif), spe(spe), shi(shi){}
    
    virtual Vec3 getNormal(Vec3 phit) const = 0;
};
class Plane : public Object {
public:
    Vec3 pos;   // position
    Vec3 nor;   // normal
    
    Plane(Vec3 pos, Vec3 nor, Vec3 amb, Vec3 dif, Vec3 spe, float shi)
    : pos(pos), nor(nor), Object(amb, dif, spe, shi) {}
    
    bool intersect(const Vec3 &rayorig, const Vec3 &raydir, float &t) const { // calculate ray-plane intersection
        if (nor.dot(-raydir) > 0) {
            t = -((pos - rayorig).dot(nor)) / nor.dot(-raydir);
            return true;
        }
        return false;
    }
    
    Vec3 getNormal(Vec3 phit) const { // return normal of the plane
        return nor;
    }
};
class Sphere : public Object {
public:
    Vec3 pos;   // position
    float rad2; // sphere radius^2

    Sphere(Vec3 pos, float rad, Vec3 amb, Vec3 dif, Vec3 spe, float shi)
    : pos(pos), rad2(rad * rad), Object(amb, dif, spe, shi) {}
    
    bool intersect(const Vec3 &rayorig, const Vec3 &raydir, float &t) const { // calculate ray-sphere intersection
        float a = 1;
        float b = 2 * (raydir.x * (rayorig.x - pos.x) + raydir.y * (rayorig.y - pos.y) + raydir.z * (rayorig.z - pos.z));
        float c = (rayorig.x - pos.x) * (rayorig.x - pos.x) + (rayorig.y - pos.y) * (rayorig.y - pos.y) + (rayorig.z - pos.z) * (rayorig.z - pos.z) - rad2;
        float delta = b * b - 4 * a * c;
        
        if (delta < 0) return false;
        
        float t0 = (-b - sqrt(delta)) / (2 * a);
        float t1 = (-b + sqrt(delta)) / (2 * a);
        
        if (t0 < 0) t = t1;
        else t = t0;

        return true;
    }
    
    Vec3 getNormal(Vec3 phit) const { // calculate and return normal of the sphere
        return phit - pos;
    }
};
class Light {
public:
    Vec3 pos;  // position
    Vec3 col;  // color
    std::vector<Vec3> positions;
    
    Light(Vec3 pos, Vec3 col)
    : pos(pos), col(col) {
        positions = getLightPositions();
    }
    
    std::vector<Vec3> getLightPositions() { // replace point light by area light of 27 different lights
        std::vector<Vec3> lightPos;
        
        float i = 1;
        lightPos.push_back(Vec3(pos.x - i, pos.y - i, pos.z - i));
        lightPos.push_back(Vec3(pos.x - i, pos.y - i, pos.z - 0));
        lightPos.push_back(Vec3(pos.x - i, pos.y - i, pos.z + i));
        lightPos.push_back(Vec3(pos.x - 0, pos.y - i, pos.z - i));
        lightPos.push_back(Vec3(pos.x - 0, pos.y - i, pos.z - 0));
        lightPos.push_back(Vec3(pos.x - 0, pos.y - i, pos.z + i));
        lightPos.push_back(Vec3(pos.x + i, pos.y - i, pos.z - i));
        lightPos.push_back(Vec3(pos.x + i, pos.y - i, pos.z - 0));
        lightPos.push_back(Vec3(pos.x + i, pos.y - i, pos.z + i));
        
        lightPos.push_back(Vec3(pos.x - i, pos.y - 0, pos.z - i));
        lightPos.push_back(Vec3(pos.x - i, pos.y - 0, pos.z - 0));
        lightPos.push_back(Vec3(pos.x - i, pos.y - 0, pos.z + i));
        lightPos.push_back(Vec3(pos.x - 0, pos.y - 0, pos.z - i));
        lightPos.push_back(Vec3(pos.x - 0, pos.y - 0, pos.z - 0));
        lightPos.push_back(Vec3(pos.x - 0, pos.y - 0, pos.z + i));
        lightPos.push_back(Vec3(pos.x + i, pos.y - 0, pos.z - i));
        lightPos.push_back(Vec3(pos.x + i, pos.y - 0, pos.z - 0));
        lightPos.push_back(Vec3(pos.x + i, pos.y - 0, pos.z + i));
        
        lightPos.push_back(Vec3(pos.x - i, pos.y + i, pos.z - i));
        lightPos.push_back(Vec3(pos.x - i, pos.y + i, pos.z - 0));
        lightPos.push_back(Vec3(pos.x - i, pos.y + i, pos.z + i));
        lightPos.push_back(Vec3(pos.x - 0, pos.y + i, pos.z - i));
        lightPos.push_back(Vec3(pos.x - 0, pos.y + i, pos.z - 0));
        lightPos.push_back(Vec3(pos.x - 0, pos.y + i, pos.z + i));
        lightPos.push_back(Vec3(pos.x + i, pos.y + i, pos.z - i));
        lightPos.push_back(Vec3(pos.x + i, pos.y + i, pos.z - 0));
        lightPos.push_back(Vec3(pos.x + i, pos.y + i, pos.z + i));
        
        return lightPos;
    }
};

Camera *camera = NULL;
std::vector<Sphere> spheres;
std::vector<Plane> planes;
std::vector<Light> lights;

#endif /* Objects_h */
