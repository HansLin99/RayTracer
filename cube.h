//
// Created by Hans on 2020-12-04.
//

#ifndef RAYTRACER_CUBE_H
#define RAYTRACER_CUBE_H

#include <vector>
#include "rtweekend.h"

#include "hittable.h"
#include "triangle.h"

class cube : public hittable {
public:
    cube(){};

    cube(point3 c, double s, shared_ptr<material> mat): c(c), s(s), mat_ptr(mat){
        std::vector<point3> tmp_points;
        // Top left corner of the near plane
        tmp_points.push_back(c);
        // Top right corner of the near plane
        tmp_points.push_back(c + point3(s, 0, 0));
        // Bottom right corner of the near plane
        tmp_points.push_back(c + point3(s, -s, 0));
        // Bottom left corner of the near plane
        tmp_points.push_back(c + point3(0, -s, 0));
        // Top left corner of the rear plane
        tmp_points.push_back(c + point3(0, 0, -s));
        // Top right corner of the rear plane
        tmp_points.push_back(c + point3(s, 0, -s));
        // Bottom right corner of the rear plane
        tmp_points.push_back(c + point3(s, -s, -s));
        // Bottom left corner of the rear plane
        tmp_points.push_back(c + point3(0, -s, -s));
        std::vector<point3> points;
        for (int i=0;i<tmp_points.size();i++) {
            points.push_back(rotate(tmp_points[i], vec3(0, 0, 1), 45));
        }

        // font plane
        triangle triangle1(points[0], points[1], points[2], unit_vector(cross(points[2]-points[0],points[1]-points[0])), mat);
        triangle triangle2(points[0], points[3], points[2], unit_vector(cross(points[3]-points[0],points[2]-points[0])), mat);
        // top plane
        triangle triangle3(points[0], points[1], points[4], unit_vector(cross(points[1]-points[0],points[4]-points[0])), mat);
        triangle triangle4(points[4], points[1], points[5], unit_vector(cross(points[1]-points[4], points[5]-points[4])), mat);
        // right plane
        triangle triangle5(points[6], points[1], points[2], unit_vector(cross(points[2]-points[1],points[6]-points[1])), mat);
        triangle triangle6(points[5], points[1], points[6], unit_vector(cross(points[6]-points[1],points[5]-points[1])), mat);
        // rear plane
        triangle triangle7(points[4], points[6], points[7], unit_vector(cross(points[6]-points[4], points[7]-points[4])), mat);
        triangle triangle8(points[4], points[5], points[6], unit_vector(cross(points[5]-points[4], points[6]-points[4])), mat);
        // left plane
        triangle triangle9(points[0], points[7], points[3], unit_vector(cross(points[4]-points[0], points[7]-points[0])), mat);
        triangle triangle10(points[0], points[4], points[7], triangle9.normal, mat);
        // bottom plane
        triangle triangle11(points[3], points[7], points[2], unit_vector(cross(points[7]-points[3], points[2]-points[3])), mat);
        triangle triangle12(points[6], points[7], points[2], triangle11.normal, mat);
        std::vector<triangle> temp_triangles{triangle1, triangle2, triangle3, triangle4, triangle5, triangle6
                ,triangle7,triangle8, triangle9, triangle10, triangle11, triangle12};
        this->triangles.insert(this->triangles.end(), temp_triangles.begin(), temp_triangles.end());
    };

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;



public:
    point3 c;
    double s;
    std::vector<triangle> triangles;
    shared_ptr<material> mat_ptr;
};

bool cube::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double t_min_so_far = t_max;
    bool ifhit = false;
    for (auto a=this->triangles.begin();a != this->triangles.end();a++){
        hit_record temp_hit;
        if (a->hit(r, t_min, t_max, temp_hit)) {
            if (temp_hit.t < t_min_so_far) {
                t_min_so_far = temp_hit.t;
                rec.t = temp_hit.t;
                rec.p = temp_hit.p;
                rec.normal = temp_hit.normal;
                rec.mat_ptr = this->mat_ptr;
                ifhit = true;
            }
        }
    }
    return ifhit;
}

#endif //RAYTRACER_CUBE_H
