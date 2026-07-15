# Phase 1
## Issue1
 Some major issues while developing simple rigidbody physics was - responbilites divison, deltatime and communication between renderer&world <br>
**Solution used:** 
- separation of concerns in LLD
- deltatime physics integrator <- lotta maths lol ->
- Body as data and nothing more.

```
Deltatime is time elapsed between current and last rendered frame (and it has 1frame delay), it is used to move same distance despite the fps.
Types of update:
{
    1. fixed update - idealyl the physics engine uses this until we use the render class.
    2. dynamic update(used dt) - used along with the render class. [used lerp function to integrate deltatime]
}
```
**LESSON: Data structures should be dumb; but systems should be smart.**
**FINAL DESIGN CHOICE: the physics-engine and graphics-rnederer stays deterministic and the demos handle the deltatime as they want to**
**FUTURE DESIGN CHOICE could be a multthreaded deterministic ohysics-world and dynamic(dt) graphics-renderer**

## Issue2:
The demo_file looks to complex due to deterministic nature of the physics engine and handeling of the delta time in the demo.
Solution -> Utilize a pub-sub model in the future.
Added '''Engine''' class for abstraction while creating Demos.
---
---

# Phase 2
Adding collision logic - single instance brute force resolution for now ... [OPTIMIZE THIS LATER]

these are the standard commit msgs being used
![Commit msg standard](/assets/commits-msgs.png)

**FORCE OWN BODY SYSTEM**
FORCES KNOW WHICH BODY THEY HAVE TO BE APPLIED ON [optimize using ecs later]
```EACH DISPLAY LOOP = body*forces(position update based on force) + body*body(collision check)``` [OPTIMIZE THIS]

* issue1: jittering due to constant collision and force updates ---> solution is velocity dampening(?)

***phase 2 complete ---> added forces(continuous check) and collisions(instantaneous check)***

## Phase 2.5 -- proper rigid bodies (SAT + rotation)
The brute-force circle-only collisions were upgraded into a real narrow phase:

- **Vec2 fixed**: `dot` was returning `int` (!), `cross` returned a bogus Vec2. Now `dot`->float, `cross`->scalar, all ops const-correct + helpers (`normalized`, `distance`, `lengthSq`).
- **Inverse mass + static bodies**: `mass <= 0` => `invMass = 0` (immovable). Floors/walls are now just static bodies. Solver is branch-free on invMass.
- **Linear damping**: frame-rate-independent `vel *= 1/(1 + damping*dt)` finally kills the jitter from Phase 2 (answered the `velocity dampening(?)` TODO).
- **Shape system**: `Shape` base -> `CircleShape` / `PolygonShape`. Bodies own a `shared_ptr<Shape>`; legacy circle constructor kept so old demos never broke.
- **Rotational dynamics**: bodies gained angle/angularVel/torque + moment of inertia (disc + convex-polygon formulas).
- **Narrow phase (SAT)**: circle-circle, circle-polygon (Voronoi regions), polygon-polygon (min-penetration axis + reference/incident face clipping -> contact manifold).
- **Impulse solver**: angular impulses at each contact point + Coulomb friction (clamped to the cone) + Baumgarte positional correction. Verified: a box dropped on a static floor settles flat, no tunneling, no phantom spin.
- **Broad phase**: uniform spatial hash turns O(n^2) into ~O(n) candidate pairs.

**LESSON: build the abstraction (Shape) BEFORE the feature (polygons). The cloth demo failed exactly because it bolted positional constraints onto Euler velocity integration with no framework -- removed it; will redo on a real constraint system in Phase 3.**

---
---

# Phase 3
