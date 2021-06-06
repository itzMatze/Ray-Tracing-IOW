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
#include "sdl_window/renderer.h"

#define NUM_THREADS 14
// choose to render image fast or in great quality
#if 1
constexpr int nx = 500;
constexpr int ny = 400;
constexpr int ns = 4;
constexpr int max_depth = 10;
#else
constexpr int nx = 3840;
constexpr int ny = 2160;
constexpr int ns = 256;
constexpr int max_depth = 20;
#endif
constexpr int channels = 4;

std::uniform_real_distribution<float> distribution;
std::default_random_engine generator;

inline float random_num()
{
    return distribution(generator);
}

void saveImage(uint32_t* pixels, const std::string& name)
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
        p = 2.0f * glm::vec3(random_num(), random_num(), random_num()) - glm::vec3(1.0f, 1.0f, 1.0f);
    }
    while (glm::length(p) >= 1.0f);
    return p;
}

glm::vec4 calculate_color(const ray& r, hitable* world, int depth)
{
    hit_record rec = {};
    if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
    {
        ray scattered = {};
        glm::vec4 attenuation;
        if (depth < max_depth && rec.mat->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * calculate_color(scattered, world, depth + 1);
        }
        else
        {
            return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        // visualization of normals
        //return 0.5f * glm::vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
    }
    else
    {
        glm::vec3 unit_direction = glm::normalize(r.direction);
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + t * glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
    }
}

void calculate_pixel_row(camera* cam, hitable* world, std::atomic<int>* row, renderer* render_window)
{
    for (int j = (*row)--; j >= 0; j = (*row)--)
    {
        for (int i = 0; i < nx; ++i)
        {
            color color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; ++s)
            {
                float u = (float(i) + random_num()) / float(nx);
                float v = (float(j) + random_num()) / float(ny);
                ray r = cam->get_ray(u, v);
                color.values += calculate_color(r, world, 0);
                // keep the system responsive
                std::this_thread::yield();
            }
            color.values /= float(ns);
            color.values = glm::vec4(sqrt(color.values.r), sqrt(color.values.g), sqrt(color.values.b), 1.0f);
            render_window->set_pixel(i, ny - j - 1, color);
        }
    }
}

void trace(camera* cam, hitable* world, renderer* render_window)
{
    std::thread threads[NUM_THREADS];
    bool threads_joined = false;
    std::atomic<int> row = ny - 1;
    for (auto& t : threads)
    {
        t = std::thread(calculate_pixel_row, cam, world, &row, render_window);
    }
    bool quit = false;
    while (!quit)
    {
        render_window->render_frame();
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        // if the window gets closed before rendering has ended there will be an error because threads never got joined
        if (!threads_joined && row < 0)
        {
            threads_joined = true;
            for (auto& t : threads)
            {
                t.join();
            }
        }
    }
}

hitable* random_scene()
{
    auto* list = new std::vector<hitable*>;
    lambertian* silver = new lambertian(glm::vec3(0.2f, 0.5f, 0.5f));
    sphere* s = new sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, *silver);
    list->push_back(s);
    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            float choose_mat = random_num();
            glm::vec3 center(a + 0.6f * random_num(), 0.2f, b + 0.6f * random_num());
            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f)
            {
                if (choose_mat < 0.7f)
                {
                    lambertian* color = new lambertian(
                            glm::vec3(glm::max(random_num(), 0.2f), glm::max(random_num(), 0.2f),
                                      glm::max(random_num(), 0.2f)));
                    s = new sphere(center, 0.2f, *color);
                    list->push_back(s);
                }
                else if (choose_mat < 0.95f)
                {
                    metal* color = new metal(glm::vec3(glm::max(random_num(), 0.2f), glm::max(random_num(), 0.2f),
                                                       glm::max(random_num(), 0.2f)), 0.3f * random_num());
                    s = new sphere(center, 0.2f, *color);
                    list->push_back(s);
                }
                else
                {
                    dielectric* color = new dielectric(1.5f);
                    s = new sphere(center, 0.2f, *color);
                    list->push_back(s);
                }
            }
        }
    }
    lambertian* color = new lambertian(glm::vec3(0.1f, 0.8f, 0.9f));
    metal* metal_color = new metal(glm::vec3(0.8f, 0.8f, 0.8f), 0.001f);
    s = new sphere(glm::vec3(-1.0f, 2.0f, -2.6f), 1.0f, *metal_color);
    list->push_back(s);
    s = new sphere(glm::vec3(2.0f, 1.8f, -3.0f), 1.0f, *color);
    list->push_back(s);
    hitable* ran_scene = new hitable_list(list);
    return ran_scene;
}

// TODO make naming scheme consistent (forExample vs for_example)
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
    hitable* world = new hitable_list(&objects);
    camera cam(glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               90.0f, float(nx) / float(ny), 0.01f, 2.0f);
    hitable* ran_scene = random_scene();
    renderer render_window(1000, nx, ny);
    // TODO trace image incrementally, first one sample per pixel and then accumulate more
    // TODO probably also render first in lower resolution and then add resolution (this is probably not so easy)
    trace(&cam, ran_scene, &render_window);
    saveImage((uint32_t*) render_window.get_pixels(), "");
    return 0;
}
