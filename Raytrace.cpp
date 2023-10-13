#include "vec3.h"
#include <fstream>
#include <optional>
#include <random>
#include <vector>

// Utils

template <typename T, typename T2, typename F>
std::optional<T2> and_then(std::optional<T> &o, F &&f) {
  if (o) {
    return f(*o);
  } else {
    return {};
  }
}

template <typename T> T value_or(std::optional<T> &&o, T &&v) {
  if (o) {
    return *o;
  } else {
    return v;
  }
}

// Image

struct RGBPixel {
  uint8_t r, g, b;
};

// Call getPixelValue for each pixel of the image and write the image to the
// disk at path
template <typename F>
void generateImage(const char *path, int width, int height, F getPixelValue) {
  std::fstream fs;
  fs.open(path, std::fstream::out);

  fs << "P3\n" << width << " " << height << "\n255\n";

  for (unsigned int y = 0; y < height; y++)
    for (unsigned int x = 0; x < width; x++) {
      const auto &pixel = getPixelValue(y, x);
      fs << +pixel.r << " " << +pixel.g << " " << +pixel.b << " \n";
    }
}

// Objects

struct Sphere {
  vec3 center;
  float radius;
};

struct Rayon {
  vec3 origin;
  vec3 direction;
};

enum class Material { Diffuse, Glass, Mirror };

struct Object {
  Sphere sphere;
  vec3 albedo;
  Material material;
};

const float pi = 3.14;

std::optional<float> intersection(const Rayon &ray, const Sphere &sphere,
                                  float tMin) {
  // Rayon => X = O + t * D
  // Sphere => dist(X, C) = R
  //
  // dist(O + t * d, C) = R
  // norm^2(O + t * d - C) = R^2
  //
  // let OC = O - C
  const vec3 oc(ray.origin - sphere.center);
  //
  // norm^2(OC + t * D) = R^2
  // (OC + tD)^2 = R^2
  // OC^2 + t^2D^2 + 2 * tD.OC = R^2
  // t^2 * D^2 + t * 2 dot(D, OC) + OC^2 - R^2 = 0
  //
  // let a = D^2
  // let b = 2 (D . OC)
  // let c = OC^2 - R^2
  const float a = ray.direction.squared_length();
  const float b = 2 * dot(ray.direction, oc);
  const float c = oc.squared_length() - sq(sphere.radius);

  // t^2 * a + t * b + c = 0
  const float delta = b * b - 4 * a * c;
  if (delta >= 0) {
    const float t = (-b - sqrt(delta)) / (2 * a);
    if (t > tMin)
      return {t};
    {
      const float t = (-b + sqrt(delta)) / (2 * a);
      if (t > tMin)
        return {t};
    }
  }

  return {};
}

struct Intersection {
  float t;
  vec3 albedo;
  Material material;
  vec3 point;
  vec3 normal;
};

std::optional<Intersection> intersection(const Rayon &ray,
                                         const std::vector<Object> &objects) {
  std::optional<Intersection> it{};

  for (auto &&object : objects) {
    auto it2 = intersection(ray, object.sphere, 0.0001);
    if (it2) {
      if (!it || *it2 < it->t) {
        float t = *it2;
        const vec3 p = ray.origin + t * ray.direction;
        const vec3 n = unit_vector(p - object.sphere.center);
        it = Intersection{t, object.albedo, object.material, p, n};
      }
    }
  }

  return it;
}

bool has_intersection(const Rayon &ray, const std::vector<Object> &objects,
                      float tMin, float tMax) {
  for (auto &&object : objects) {
    auto it = intersection(ray, object.sphere, tMin);
    if (it) {
      float itT = *it;
      if (itT > tMin && itT < tMax)
        return true;
    }
  }

  return false;
}

uint8_t toInt(float f) {
  if (f > 1)
    return 255;
  if (f < 0)
    return 0;
  return f * 255;
}

RGBPixel toRGB(vec3 v) {
  return RGBPixel{toInt(v.x()), toInt(v.y()), toInt(v.z())};
}

struct Lamp {
  vec3 position;
  vec3 emission;
};

struct Scene {
  std::vector<Object> objects;
  std::vector<Lamp> lamps;
  mutable std::default_random_engine generator;
};

float clampPositive(float v) {
  if (v < 0)
    return 0;
  return v;
}

// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
vec3 reflect(vec3 I, vec3 N) { return I - 2.0 * dot(N, I) * N; }

// https://registry.khronos.org/OpenGL-Refpages/gl4/html/refract.xhtml
std::optional<vec3> refract(float eta, vec3 I, vec3 N) {
  float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
  if (k < 0.0)
    return {};
  else
    return eta * I - (eta * dot(N, I) + sqrt(k)) * N;
}

float schlick(float n1, float n2, float cosTheta) {
  float r0 = sq((n1 - n2) / (n1 + n2));

  float a = 1 - cosTheta;
  return r0 + (1 - r0) * a * a * a * a * a;
}

int rayCount = 0;

vec3 raytrace(const Rayon &ray, const Scene &scene, int currentDepth) {
  rayCount++;
  if (currentDepth > 40) {
    return vec3{0, 0, 0};
  }

  auto tM = intersection(ray, scene.objects);

  return value_or(
      and_then<Intersection, vec3>(
          tM,
          [&scene, &ray, &currentDepth](Intersection &it) {
            vec3 contrib{0, 0, 0};

            // Debug for intersections
            // return it.albedo;

            switch (it.material) {
            case Material::Diffuse:
            {
              //std::uniform_int_distribution<int> distribution(1, scene.lamps.size());
              //int u = distribution(scene.generator);

              for (auto &&lamp : scene.lamps)
              {
                //auto &lamp = scene.lamps[u];
                vec3 direction_to_lamp = lamp.position - it.point;
                float dist2 = direction_to_lamp.squared_length();
                float cos = clampPositive(
                    dot(it.normal, unit_vector(direction_to_lamp)));
                                         //
                if (!has_intersection(
                        {it.point, unit_vector(direction_to_lamp)},
                        scene.objects, 00.1, direction_to_lamp.length())) {
                  contrib += cos * lamp.emission * it.albedo / dist2 / pi;
                }
              }

              // Calcul de la contribution indirect
              std::uniform_real_distribution<float> distribution(-1, 1);
              float u = distribution(scene.generator);
              float v = distribution(scene.generator);
              float w = distribution(scene.generator);
              vec3 newDirection = unit_vector({u, v, w});

              auto reflectContrib = raytrace({it.point + newDirection * 0.1, newDirection}, scene, currentDepth + 1) * (2 * pi);
              float cos = clampPositive(dot(newDirection, it.normal));
              contrib += it.albedo * reflectContrib * cos / pi;
              break;
            }
            case Material::Mirror: {
              auto refDir = reflect(ray.direction, it.normal);
              Rayon newRay{it.point + 0.1 * refDir, refDir};

              auto reflectContrib = raytrace(newRay, scene, currentDepth + 1);

              contrib += reflectContrib * it.albedo;
            }

            break;
            case Material::Glass: {
              float ior = 1.5;
              float cosTheta = abs(dot(it.normal, ray.direction));
              float reflexionCoef = schlick(1, ior, cosTheta);

              auto getContribGlass = [&]() {
                // glass part
                auto refDirM =
                    dot(ray.direction, it.normal) < 0
                        ? (refract(1 / ior, ray.direction, it.normal))
                        : (refract(ior, ray.direction, -it.normal));
                if (refDirM) {
                  auto refDir = *refDirM;
                  Rayon newRay{it.point + 0.1 * refDir, refDir};

                  auto refractContrib =
                      raytrace(newRay, scene, currentDepth + 1);

                  return (1 - reflexionCoef) * refractContrib * it.albedo;
                }
                return vec3{0, 0, 0};
              };

              auto getContribMirror = [&]() {
                // mirror part
                auto refDir = reflect(ray.direction, it.normal);
                Rayon newRay{it.point + 0.1 * refDir, refDir};

                auto reflectContrib = raytrace(newRay, scene, currentDepth + 1);

                return reflexionCoef * reflectContrib * it.albedo;
              };

              std::uniform_real_distribution<float> distribution(0, 1);
              float u = distribution(scene.generator);

              float threshold = reflexionCoef;
              if (u < threshold) {
                contrib += getContribMirror() / threshold;
              } else {
                contrib += getContribGlass() / (1 - threshold);
              }

            } break;
            }

            return contrib;
          }),
      {0.9, 0.3, 0.7});
}

int main() {
  const std::vector<Object> objects{
      Object{Sphere{{400, 100000, 500}, 99500}, {1, 1, 1}, Material::Diffuse},
      Object{Sphere{{200, 400, 500}, 100}, {1, 0, 0}, Material::Diffuse},
      Object{Sphere{{400, 400, 700}, 100}, {0.9, 0.9, 0.9}, Material::Mirror},
      Object{Sphere{{600, 400, 500}, 100}, {0.9, 0.9, 0.9}, Material::Glass},
      Object{Sphere{{400, 300, 410}, 20}, {0, 0.5, 1}, Material::Diffuse},
      Object{Sphere{{200, 100, 500}, 100}, {0, 1, 1}, Material::Diffuse},
  };

  const Scene scene{objects,
                    {
                        //Lamp{{400, 600, 500}, {1000000, 1000000, 1000000}},
                        //Lamp{{0, 200, 300}, {20000, 20000, 2000}},
                        Lamp{{400, 200, 500}, {200000, 200000, 200000}},
                    }};

  generateImage(
      "im.ppm", 800, 600, [&scene](const int y, const int x) -> RGBPixel {
        // Creation du rayon camera
        // far
        vec3 p{400, 300, -2000};

        std::uniform_real_distribution<float> distribution(-.5, .5);

        vec3 contrib{0, 0, 0};
        const int N = 100;

        for (int i = 0; i < N; i++) {
          float u = distribution(scene.generator);
          float v = distribution(scene.generator);
          vec3 direction = unit_vector(vec3{float(x + u), float(y + v), 0} - p);

          const Rayon ray{p, direction};

          // Convertie en couleur image
          contrib += raytrace(ray, scene, 0);
        }

        return toRGB(contrib / N);
      });

  std::cout << rayCount << std::endl;
}

// Benchmark:
// 249ms. with 10spp: 2.8s
// With random numbers: 110ms / with 10spp: 1.63s
