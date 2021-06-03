#include <iostream>
#include <filesystem>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Ray.h" // contains using namespace glm

void saveImage(int width, int height, uint8_t* pixels, int channels, std::string name)
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
    filename.append( ".png");
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
    } while (std::filesystem::exists(path_to_image));
    stbi_write_png(path_to_image.string().c_str(), width, height, channels, pixels, width * channels);
}

bool hit_sphere(const glm::vec3& center, float radius, const Ray& ray)
{
    glm::vec3 oc = ray.origin - center;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

glm::vec3 calculate_color(const Ray& ray)
{
    if (hit_sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, ray))
    {
        return glm::vec3(1.0f, 0.0f, 1.0f);
    }
    glm::vec3 unit_direction = glm::normalize(ray.direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
    int nx = 1000;
    int ny = 800;
    int channels = 3;
    glm::vec3 lower_left_corner(-1.0f, -1.0f, -1.0f);
    glm::vec3 horizontal(2.0f, 0.0f, 0.0f);
    glm::vec3 vertical(0.0f, 2.0f, 0.0f);
    glm::vec3 origin(0.0f, 0.0f, 0.0f);
    uint8_t* pixels = new uint8_t[nx * ny * channels]; // 3 because we don't use alpha
    int index = 0;
    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
            glm::vec3 color = calculate_color(ray);
            int ir = int(255.99 * color.r);
            int ig = int(255.99 * color.g);
            int ib = int(255.99 * color.b);

            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }
    // TODO built in my renderer to have the output show up in realtime in a window
    saveImage(nx, ny, pixels, channels, "");
    return 0;
}
