# The Computer Language Benchmarks Game
# http://shootout.alioth.debian.org/
#
# contributed by Kevin Carson
# modified by Tupteq
# modified by Fredrik Johansson
# modified by Tupteq (again)
# Cythonized by Robert Bradshaw

import sys

cdef extern from "math.h":
    double sqrt(double)

cdef double pi = 3.14159265358979323
cdef double solar_mass = 4 * pi * pi
cdef double days_per_year = 365.24

cdef extern from *:
    int nbodies "5"
# cdef int nbodies = 5

cdef struct Planet:
    double x
    double y
    double z
    
    double vx
    double vy
    double vz
    
    double mass


cdef bint advance(Planet *bodies, double dt, int n):
    cdef int i, j, k
    cdef double dx, dy, dz, mag
    cdef double dist2
    cdef double acc_i, acc_j
    for k from 0 <= k < n:
        for i from 0 <= i < nbodies:
            for j from i < j < nbodies:
                dx = bodies[i].x - bodies[j].x
                dy = bodies[i].y - bodies[j].y
                dz = bodies[i].z - bodies[j].z
                
                dist2 = (dx*dx + dy*dy + dz*dz)
                mag = dt / (sqrt(dist2)*dist2)

                acc_j = mag * bodies[i].mass
                acc_i = mag * bodies[j].mass                

                bodies[i].vx -= dx * acc_i
                bodies[i].vy -= dy * acc_i
                bodies[i].vz -= dz * acc_i
                
                bodies[j].vx += dx * acc_j
                bodies[j].vy += dy * acc_j
                bodies[j].vz += dz * acc_j
                
        for i from 0 <= i < nbodies:
            bodies[i].x += dt * bodies[i].vx
            bodies[i].y += dt * bodies[i].vy
            bodies[i].z += dt * bodies[i].vz


cdef energy(Planet *bodies):
    cdef double e = 0.0
    cdef Planet b
    cdef double dx, dy, dz
    cdef double distance
    cdef int i, j
    for i from 0 <= i < nbodies:
        b = bodies[i]
        e += 0.5 * b.mass * (b.vx * b.vx + b.vy * b.vy + b.vz * b.vz)
        for j from i < j < nbodies:
            dx = b.x - bodies[j].x
            dy = b.y - bodies[j].y
            dz = b.z - bodies[j].z
            distance = sqrt(dx*dx + dy*dy + dz*dz)
            e -= b.mass * bodies[j].mass / distance
    return e


cdef offset_momentum(Planet *bodies):
    cdef Planet sun = bodies[0]
    cdef int i
    cdef double px, py, pz
    px = py = pz = 0.0

    for i from 0 <= i < nbodies:
        px -= bodies[i].vx * bodies[i].mass
        py -= bodies[i].vy * bodies[i].mass
        pz -= bodies[i].vz * bodies[i].mass

    sun.vx = px / solar_mass
    sun.vy = py / solar_mass
    sun.vz = pz / solar_mass
    
    bodies[0] = sun


cdef Planet sun = Planet(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, solar_mass)

cdef Planet jupiter = Planet(
    4.84143144246472090e+00,
    -1.16032004402742839e+00,
    -1.03622044471123109e-01,
    1.66007664274403694e-03 * days_per_year,
    7.69901118419740425e-03 * days_per_year,
    -6.90460016972063023e-05 * days_per_year,
    9.54791938424326609e-04 * solar_mass)

cdef Planet saturn = Planet(
    8.34336671824457987e+00,
    4.12479856412430479e+00,
    -4.03523417114321381e-01,
    -2.76742510726862411e-03 * days_per_year,
    4.99852801234917238e-03 * days_per_year,
    2.30417297573763929e-05 * days_per_year,
    2.85885980666130812e-04 * solar_mass)

cdef Planet uranus = Planet(
    1.28943695621391310e+01,
    -1.51111514016986312e+01,
    -2.23307578892655734e-01,
    2.96460137564761618e-03 * days_per_year,
    2.37847173959480950e-03 * days_per_year,
    -2.96589568540237556e-05 * days_per_year,
    4.36624404335156298e-05 * solar_mass)

cdef Planet neptune = Planet(
    1.53796971148509165e+01,
    -2.59193146099879641e+01,
    1.79258772950371181e-01,
    2.68067772490389322e-03 * days_per_year,
    1.62824170038242295e-03 * days_per_year,
    -9.51592254519715870e-05 * days_per_year,
    5.15138902046611451e-05 * solar_mass)


cdef main(n):
    cdef Planet* bodies = [sun, jupiter, saturn, uranus, neptune]
    offset_momentum(bodies)
    print "%.9f" % energy(bodies)
    advance(bodies, 0.01, n)
    print "%.9f" % energy(bodies)

if __name__ == '__main__':
    try:
        n = int(sys.argv[1])
    except IndexError:
        n = 50000000
    main(n)
