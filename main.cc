//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "cube.h"

#include <iostream>


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        // This is the ambient color
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        point3 lightpos = point3(0, 15, 15);
        point3 lightpos_softshadow = lightpos + + 3.0 * random_in_unit_sphere();
        vec3 lightdir = rec.p - lightpos;
        vec3 lightdir_softshadow = rec.p - lightpos_softshadow;
        ray shadowRay(rec.p, -lightdir_softshadow);
        double kdiffuse = 1.0;
        double kspecular = 1.0;
        double shininess = 25.0;
        color lightColor = color(255.0, 255.0, 255.0);
        hit_record temp_rec;
        if (world.hit(shadowRay, 0.001, infinity, temp_rec)) {
           kdiffuse = 0.1;
           kspecular = 0.1;
        }
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return 0.5 * kdiffuse * rec.mat_ptr->calculateDiffuse(rec.normal, lightdir) * attenuation
           + 0.5 * attenuation * ray_color(scattered, world, depth-1)
            + kspecular * rec.mat_ptr->calculateSpecular(r.direction(), lightdir, rec.normal, shininess) * lightColor;
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<cube>(point3(3, 0, 0), 1.0, material3));

    return world;
}


int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    
    auto world = random_scene();
//    hittable_list world;
//
//    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
//    auto material1 = make_shared<dielectric>(1.5);
//    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
//
//    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//    world.add(make_shared<cube>(point3(0,2,0), 1.5, material2));
//    world.add(make_shared<triangle>(point3(-1,0,0), point3(1,0,0),point3(0,1,0), vec3(0,0,1),material2));

//    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
//
//    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    // Camera


    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
