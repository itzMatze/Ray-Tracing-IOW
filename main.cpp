#include <iostream>
#include <filesystem>
#include <random>
#include <thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "materials/dielectric.h"
#include "materials/metal.h"
#include "materials/lambertian.h"
#include "objects/sphere.h"
#include "objects/hitable_list.h"
#include "camera.h"

// choose to render image fast or in great quality
#if 1
constexpr int nx = 1000;
constexpr int ny = 800;
constexpr int ns = 32;
constexpr int max_depth = 15;
#else
constexpr int nx = 3840;
constexpr int ny = 2160;
constexpr int ns = 256;
constexpr int max_depth = 20;
#endif
constexpr int channels = 3;

std::uniform_real_distribution<float> distribution;
std::default_random_engine generator;

inline float random_num()
{
    return distribution(generator);
}

void saveImage(uint8_t* pixels, const std::string &name)
{
    std::string filename;
    if (!name.empty())
    {
        filename = "_";
    }
    // getting time to add it to filename
#if 0
    time_t     now = time(nullptr);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), " %Y-%m-%d_%H-%M-%S", &tstruct);
    std::string time(buf);
    filename.append(time);
#endif
    filename.append(name);
    filename.append(".png");
    // create target directory if needed
    std::filesystem::path images_path("../images/");
    if (!std::filesystem::exists(images_path))
    {
        std::filesystem::create_directory(images_path);
    }
    std::filesystem::path path_to_image;
    // if filename contains the time this index is useless, every file is unique by time (except you generate two files in one second)
    // be aware that this indexing uses empty indices in between e.g. if a file got deleted
    int index = 1;
    do
    {
        std::string index_string = std::to_string(index);
        path_to_image.clear();
        path_to_image = std::filesystem::path(images_path.string().append(index_string.append(filename)));
        index++;
    }
    while (std::filesystem::exists(path_to_image));
    stbi_write_png(path_to_image.string().c_str(), nx, ny, channels, pixels, nx * channels);
}

glm::vec3 random_in_unit_sphere()
{
    glm::vec3 p;
    do
    {
        p = 2.0f * glm::vec3(random_num(), random_num(), random_num()) -
            glm::vec3(1.0f, 1.0f, 1.0f);
    }
    while (glm::length(p) >= 1.0f);
    return p;
}

glm::vec3 calculate_color(const ray &r, hitable* world, int depth)
{
    hit_record rec = {};
    if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
    {
        ray scattered = {};
        glm::vec3 attenuation;
        if (depth < max_depth && rec.mat->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * calculate_color(scattered, world, depth + 1);
        }
        else
        {
            return glm::vec3(0.0f, 0.0f, 0.0f);
        }
        // visualization of normals
        //return 0.5f * glm::vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
    }
    else
    {
        glm::vec3 unit_direction = glm::normalize(r.direction);
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}

void calculate_pixel_row(camera* cam, hitable* world, uint8_t* pixels, int j)
{
    int index = (ny - j - 1) * 3 * nx;
    for (int i = 0; i < nx; ++i)
    {
        glm::vec3 color(0.0f, 0.0f, 0.0f);
        for (int s = 0; s < ns; ++s)
        {
            float u = (float(i) + random_num()) / float(nx);
            float v = (float(j) + random_num()) / float(ny);
            ray r = cam->get_ray(u, v);
            color += calculate_color(r, world, 0);
            // keep the system responsive
            std::this_thread::yield();
        }
        color /= float(ns);
        color = glm::vec3(sqrt(color.r), sqrt(color.g), sqrt(color.b));
        int ir = int(255.99 * color.r);
        int ig = int(255.99 * color.g);
        int ib = int(255.99 * color.b);
        pixels[index++] = ir;
        pixels[index++] = ig;
        pixels[index++] = ib;
    }
}

void trace(camera* cam, hitable* world, uint8_t* pixels)
{
    std::thread threads[ny];
    for (int j = ny - 1; j >= 0; --j)
    {
        threads[j] = std::thread(calculate_pixel_row, cam, world, pixels, j);
    }
    for (int j = ny - 1; j >= 0; --j)
    {
        threads[j].join();
    }
}

int main()
{
    {
        // random number generator setup
        std::random_device rd;
        std::uniform_real_distribution<float> dis(0, 20000);
        generator = std::default_random_engine(dis(rd));
        distribution = std::uniform_real_distribution<float>(0, 1);
    }
    std::vector<hitable*> objects;
    lambertian lambertian1(glm::vec3(0.5f, 0.1f, 0.7f));
    lambertian lambertian2(glm::vec3(0.1f, 0.8f, 0.8f));
    metal silver(glm::vec3(0.8f, 0.8f, 0.8f), 0.01f);
    metal gold(glm::vec3(0.8f, 0.6f, 0.2f), 0.1f);
    dielectric glass(1.5f);
    objects.push_back(new sphere(glm::vec3(0.0f, 0.0f, -2.0f), 0.5, lambertian1));
    objects.push_back(new sphere(glm::vec3(0.0f, -100.5f, -2.0f), 100, lambertian2));
    objects.push_back(new sphere(glm::vec3(1.1f, 0.0f, -2.0f), 0.5f, glass));
    objects.push_back(new sphere(glm::vec3(-1.1f, 0.0f, -2.0f), 0.5f, gold));
    objects.push_back(new sphere(glm::vec3(0.3f, -0.3f, -1.1f), 0.2f, silver));
    hitable* world = new hitable_list(objects);
    camera cam(glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, float(nx) / float(ny));
    uint8_t* pixels = new uint8_t[nx * ny * channels]; // 3 because we don't use alpha
    trace(&cam, world, pixels);
    // TODO built in my renderer to have the output show up in realtime in a window, at this point also show initial output and trace more samples after that
    saveImage(pixels, "");
    return 0;
}
