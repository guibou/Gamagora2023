Rayon:
  - origine O
  - direction D

Point sur le rayon: X = O + tD
       (avec t >= 0)

Sphere:
  - Centre C
  - Rayon R

Point sur la sphere: dist(X, C) = R


==>
dist(O + tD, C) = R
<==>
norm(C - O - tD) = R

(norm(v) = sqrt(vx ^ 2 + vy ^2 + vz ^2))

<==>
sqrt((Cx - Ox - tDx)^2
    +(Cy - Oy - tDy)^2)
    +(Cz - Oz - tDz)^2) = R
<==>
    ((Cx - Ox - tDx)^2
    +(Cy - Oy - tDy)^2)
    +(Cz - Oz - tDz)^2) = R^2
<==>
at^2 + bt + c = 0
<==>

Equation du second degré:

solutions pour t:

- 0 resultat: pas d'intersection
- 1 resultat: intersection si t >= 0
- 2 resultat: 2 intersections, on prend le min >= 0


intersection_ray_sphere(rayon, sphere) -> intersection


int main()
{
   ImageProfondeur im(800, 600, 0);

   const Sphere s(vec3(400, 300, 400), 100);

   for(int x = 0; x < 800; x++)
     for(int y = 0; y < 600; y++)
     {
        const Rayon r(vec3(x, y, 0), vec3(0, 0, 1));

        calcul_distance_avec_la_sphere
        --> dans l'image
     }
  sauvegarder_image();
}




