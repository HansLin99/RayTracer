//
// Created by Hans on 2020-12-05.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include <vector>

class triangle: public hittable{
public:
    triangle() {};
    triangle(point3 a, point3 b, point3 c, vec3 n, shared_ptr<material> mat): points(std::vector<point3>{a, b, c}), normal(n),
    mat_ptr(mat){}
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override ;

    std::vector<point3> points;
    vec3 normal;
    shared_ptr<material> mat_ptr;


private:
    bool check_barycentric(point3 p) const;
};

bool triangle::hit(const ray &r, double t_min, double t_max, hit_record & rec) const {
    point3 p = this->points[0];
    point3 w = p - r.origin();
    double a = dot(w, this->normal);
    double b = dot(r.direction(), this->normal);
    double k = a/b;
    point3 temp_intersection = k * r.direction() + r.origin();
    if (this->check_barycentric(temp_intersection)) {
        if (k < t_max && k> t_min) {
            rec.t = k;
            rec.p = temp_intersection;
            rec.set_face_normal(r, this->normal);
            rec.mat_ptr = this->mat_ptr;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool triangle::check_barycentric(point3 p) const {
    point3 a = this->points[0];
    point3 b = this->points[1];
    point3 c = this->points[2];
    double areaTotal = 0.5 * cross(a-b, b-c).length();
    double areaA = 0.5 * cross(b-p, c-b).length();
    double areaB = 0.5 * cross(a-p, c-a).length();
    double areaC = 0.5 * cross(a-p, b-a).length();
    double barycentric = (areaA/areaTotal)+ (areaB/areaTotal)  + (areaC/areaTotal);
    double delta = 0.01;
    return (barycentric > 1-delta) && (barycentric < 1+delta);

}

#endif //RAYTRACER_TRIANGLE_H
