//
//  main.cpp
//  Project
//
//  Created by Lucas Le Faucheur on 2018-06-05.
//  Copyright © 2018 Lucas Le Faucheur. All rights reserved.
//

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Objects.h"

#define cimg_display 0
#include "CImg.h"

void loadScene(const char* path) {
    FILE * file = fopen(path, "r");
    
    if(file == NULL) {
        printf("Impossible to open the file");
        exit(0);
    }
    
    while(1){ // loops through the file
        char lineHeader[2048];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) { // break if end of file
            break;
        } else if (strcmp(lineHeader, "camera") == 0) { // if file describes a camera
            fscanf(file, "\n");
            
            Vec3 pos;
            float fov;
            float f;
            float a;
            
            fscanf(file, "pos: %f %f %f\n", &pos.x, &pos.y, &pos.z);
            fscanf(file, "fov: %f\n", &fov);
            fscanf(file, "f: %f\n", &f);
            fscanf(file, "a: %f\n", &a);
            
            camera = new Camera(pos, fov, f, a);
        } else if (strcmp(lineHeader, "plane") == 0) { // if file describes a plane
            fscanf(file, "\n");
            
            Vec3 nor;
            Vec3 pos;
            Vec3 amb;
            Vec3 dif;
            Vec3 spe;
            float shi;
            
            fscanf(file, "nor: %f %f %f\n", &nor.x, &nor.y, &nor.z);
            fscanf(file, "pos: %f %f %f\n", &pos.x, &pos.y, &pos.z);
            fscanf(file, "amb: %f %f %f\n", &amb.x, &amb.y, &amb.z);
            fscanf(file, "dif: %f %f %f\n", &dif.x, &dif.y, &dif.z);
            fscanf(file, "spe: %f %f %f\n", &spe.x, &spe.y, &spe.z);
            fscanf(file, "shi: %f\n", &shi);
            
            Plane plane(pos, nor, amb, dif, spe, shi);
            planes.push_back(plane);
        } else if (strcmp(lineHeader, "sphere") == 0) { // if file describes a sphere
            fscanf(file, "\n");
            
            Vec3 pos; 
            float rad;
            Vec3 amb;
            Vec3 dif;
            Vec3 spe;
            float shi;
            
            fscanf(file, "pos: %f %f %f\n", &pos.x, &pos.y, &pos.z);
            fscanf(file, "rad: %f\n", &rad);
            fscanf(file, "amb: %f %f %f\n", &amb.x, &amb.y, &amb.z);
            fscanf(file, "dif: %f %f %f\n", &dif.x, &dif.y, &dif.z);
            fscanf(file, "spe: %f %f %f\n", &spe.x, &spe.y, &spe.z);
            fscanf(file, "shi: %f\n", &shi);
            
            Sphere sphere(pos, rad, amb, dif, spe, shi);
            spheres.push_back(sphere);
        } else if (strcmp(lineHeader, "light") == 0) { // if file describes a light
            fscanf(file, "\n");
            
            Vec3 pos;
            Vec3 col;
            
            fscanf(file, "pos: %f %f %f\n", &pos.x, &pos.y, &pos.z);
            fscanf(file, "col: %f %f %f\n", &col.x, &col.y, &col.z);
            
            Light light(pos, col);
            lights.push_back(light);
        } else {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
}

bool intersection(const Vec3 &rayorig, const Vec3 &raydir, const Object* &object, float &tnear) {
    // Find closest intersection in Spheres
    for (int i = 0; i < spheres.size(); i++) {
        float t = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t)) {
            if (t < tnear) {
                tnear = t;
                object = &spheres[i];
            }
        }
    }
    
    // Find closest intersection in Planes
    for (int i = 0; i < planes.size(); i++) {
        float t = INFINITY;
        if (planes[i].intersect(rayorig, raydir, t)) {
            if (t < tnear) {
                tnear = t;
                object = &planes[i];
            }
        }
    }
    
    if (!object)
        return false; // return false if no intersection
    return true; // return true if a ray-object intersection exists
}

Vec3 trace(const Vec3 &rayorig, const Vec3 &raydir) {
    float tnear = INFINITY;
    const Object* object = NULL;
    
    Vec3 localillumination;
    
    if (intersection(rayorig, raydir, object, tnear)) { // calculate color only if ray hits an object
        localillumination += object->amb;
    
        Vec3 point = rayorig + raydir * tnear; // position of point of intersection
        
        Vec3 pnormal = object->getNormal(point); // normal direction at point of intersection
        pnormal.normalize();
        
        Vec3 pcamera(camera->pos - point); // point to camera direction at point of intersection
        pcamera.normalize();
        
        for (int i = 0; i < lights.size(); i++) { // calculate pixel color for each lights
            if (softshadows) {
                for (int j = 0; j < lights[i].positions.size(); j++) { // for each light calculate an area light
                    Vec3 plight(lights[i].positions[j] - point); // point to light direction at point of intersection
                    plight.normalize();
                
                    Vec3 preflection = pnormal * 2 * pnormal.dot(plight) - plight; // reflection direction at point of intersection
                    preflection.normalize();
                
                    float temptnear = INFINITY;
                    const Object* tempobject = NULL;
                
                    if (!intersection(point, plight, tempobject, temptnear) || object == tempobject) { // calculate dif and spe only if the light can reach the object
                        localillumination += lights[i].col * object->dif * max(plight.dot(pnormal), 0.0) * (1.0/37.0);
                        localillumination += lights[i].col * object->spe * power(max(preflection.dot(pcamera), 0.0), object->shi) * (1.0/37.0);
                    }
                }
            }
            else {
                Vec3 plight(lights[i].pos - point); // point to light direction at point of intersection
                plight.normalize();
                
                Vec3 preflection = pnormal * 2 * pnormal.dot(plight) - plight; // reflection direction at point of intersection
                preflection.normalize();
                
                float temptnear = INFINITY;
                const Object* tempobject = NULL;
                
                if (!intersection(point, plight, tempobject, temptnear) || object == tempobject) { // calculate dif and spe only if the light can reach the object
                    localillumination += lights[i].col * object->dif * max(plight.dot(pnormal), 0.0);
                    localillumination += lights[i].col * object->spe * power(max(preflection.dot(pcamera), 0.0), object->shi);
                }
            }
        }
    }
    return localillumination; // return pixel color
}

int width, height;

int main() {
    antialiasing = false; // activate or no the good antialiasing effect
    softshadows = false; // activate or no the soft shadow effect
    
    loadScene("/Users/lucaslefaucheur/Downloads/scenes/scene5.txt");
    
    height = 1000; // height of the final image
    width = height * camera->a; // width of the final image

    cimg_library::CImg<float> image(width, height, 1, 3, 0);
    
    Vec3 pixel = *new Vec3[width * height];
    float angle = tan(M_PI * 0.5 * camera->fov / 180.);
    
    for (int y = 0; y < height; ++y) { // for each pixel on the y axis
        for (int x = 0; x < width; ++x) { // for each pixel on the x axis
            if (antialiasing) { // calculate pixel color through 4 different rays and average them
                float xx1 = (2 * ((x - 0.25) / float(width)) - 1) * angle * camera->a;
                float xx2 = (2 * ((x - 0.25) / float(width)) - 1) * angle * camera->a;
                float xx3 = (2 * ((x + 0.25) / float(width)) - 1) * angle * camera->a;
                float xx4 = (2 * ((x + 0.25) / float(width)) - 1) * angle * camera->a;
                float yy1 = (1 - 2 * ((y - 0.25) / float(height))) * angle;
                float yy2 = (1 - 2 * ((y + 0.25) / float(height))) * angle;
                float yy3 = (1 - 2 * ((y - 0.25) / float(height))) * angle;
                float yy4 = (1 - 2 * ((y + 0.25) / float(height))) * angle;
                Vec3 raydir1(xx1, yy1, -1);
                Vec3 raydir2(xx2, yy2, -1);
                Vec3 raydir3(xx3, yy3, -1);
                Vec3 raydir4(xx4, yy4, -1);
                raydir1.normalize();
                raydir2.normalize();
                raydir3.normalize();
                raydir4.normalize();
                pixel = (trace(camera->pos, raydir1) + trace(camera->pos, raydir2) + trace(camera->pos, raydir3) + trace(camera->pos, raydir4)) * 0.25;
            }
            else { // calculate pixel color through 1 ray
                float xx = (2 * ((x - 0.5) / float(width)) - 1) * angle * camera->a;
                float yy = (1 - 2 * ((y - 0.5) / float(height))) * angle;
                Vec3 raydir(xx, yy, -1);
                raydir.normalize();
                pixel = trace(camera->pos, raydir);
            }
            
            image(x, y, 0, 0) = std::min(float(1), pixel.x) * 255; // scale and copy Red color intensity
            image(x, y, 0, 1) = std::min(float(1), pixel.y) * 255; // scale and copy Green color intensity
            image(x, y, 0, 2) = std::min(float(1), pixel.z) * 255; // scale and copy Blue color intensity
        }
    }
    
    image.save("/Users/lucaslefaucheur/Downloads/scenes/test.bmp"); // save image
    std::string command = "open -a Preview /Users/lucaslefaucheur/Downloads/scenes/test.bmp";
    system(command.c_str()); // open Preview to visiualize image
    
    return 0;
}


