position_avion = (0, 0, 0)

p' = p + v 
<=>
p'.x = p.x. + v.x
p'.y = p.y + v.y
p'.z = p.z + v.z

position_personnage = (0, 0, 4000)

Somme Forces = m * a

->

a = Somme force / masse

a = (-mg + Fd) / masse

Forces en presence: 

- resistance de l'air,
    Fd = - sens_de_la_vitesse * 1/2 * Cd * A * norm(v)^2
- force_de_gravité, Fg = mg

sens_de_la_vitesse = v / norm(v)

norm(v) = sqrt(vx ^ 2 + vy ^ 2 + vz ^ 2)

norm((1, 0, 0)) == 1
norm((1, 1, 1)) == sqrt(3)

(1 / sqrt(3), 1/sqrt(3), 1/sqrt(3))

Force sur un vehicule volant:

- poussée (thrust)
- frottments (drag)
- portance (lift)
- poids (weight)

a = force_de_gravité / masse



Fg = m * g
g = 9.8

a = mg / m
=>
a = g
=>
a = Fg / m

v'(t) = a(t)
p'(t) = v(t)

p(0) = (0, 0, 4000)

(si vitesse constante)
p(t) = p(0) + v * t
v(t) = v(0) + a * t

v(t) = a * t + v(0)

p(t) = (a * (t^2)) / 2 + v0 * t + p(0)
v(t) = a * t + v0 + 0
a(t) = a


v'(t) = a
v(t) = a * t + v(0)

p'(t) = v(t) = a * t + v(0)
p(t)  =      = a * t^2 / 2 + v(0) * t + p(0)



si norm(v) == 0
  a = -mg * z
sinon
  a = -mg * z - v / norm(v) * 1/2 * Cd * A * norm(v)^2

vitesse nulle == (0, 0, 0)


a(t( = si norm(v) == 0
  a = (-mg * z) / masse
sinon
  a = (-mg * z - v / norm(v(t)) * 1/2 * Cd * A * norm(v(t))^2) / masse

v'(t) = a
v(t) = primitive(a(t)) + v(0)


a(t)
v(t + dt) ~= v(t) + dt * a(t)
p(t + dt) ~= p(t) + dt * v(t)



t = 0
position = (0, 0, 4000)
vitesse = (50, 0, 0)
dt = 1

while position.x > 0:
  a = f(m, vitesse, Cd, A)

  vitesse = vitisse + dt * a
  position = position + dt * vitesse

  t = t + dt
  


