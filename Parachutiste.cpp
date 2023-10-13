#include "vec3.h"
#include <iostream>

int main() {
  const float g = 9.8;
  const float mass = 90;
  const float coefDrag = 0.3;
  const vec3 Fgravity(mass * g * vec3(0, 0, -1));

  float t = 0;
  const float dt = 0.1;

  vec3 position(0, 0, 4000);
  vec3 vitesse(50, 0, 0);

  while (t < 60) {
    const vec3 Fdrag(-coefDrag * unit_vector(vitesse) * vitesse.squared_length());

    const vec3 acceleration = (Fdrag + Fgravity) / mass;
    position += dt * vitesse;
    vitesse += dt * acceleration;

    t += dt;
    std::cout << t << " " << position << std::endl;
  }

  return 0;
}
