//
// Created by Hans on 2020-12-04.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H


#include "ray.h"
#include "hittable_list.h"

class light {
public:
    virtual bool check_if_blocked(ray & r, hittable_list world);
};

class direction_light : light {
public:
    direction_light(point3 dir): direction(dir) {};

public:
    point3 direction;

    bool check_if_blocked(ray &r, hittable_list world);
};

class point_light : light {
public:
    point_light(point3 pos): position(pos) {};

    bool check_if_blocked(ray &r, hittable_list world);

public:
    point3 position;
};


bool direction_light::check_if_blocked(ray &r, hittable_list world) {

    return false;
}



#endif //RAYTRACER_LIGHT_H
