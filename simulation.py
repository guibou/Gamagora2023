mass = 80
g = 9.8

t = 0
dt = 1

ax, ay, az = 0, 0, - mass * g / mass
v0x, v0y, v0z = 50, 0, 0
p0x, p0y, p0z = 0, 0, 4000

px, py, pz = p0x, p0y, p0z

while pz > 0:
    px = (ax * t ** 2) / 2 + v0x * t + p0x
    py = (ay * t ** 2) / 2 + v0y * t + p0y
    pz = (az * t ** 2) / 2 + v0z * t + p0z

    print(t, px, py, pz)
    t += dt

