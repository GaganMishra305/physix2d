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
## Joints & Constraints
Built a real constraint framework -- the thing the original cloth never had.
- `Joint` base -> `DistanceJoint` (velocity-impulse + Baumgarte), `RevoluteJoint` (2x2 effective-mass pin), `RopeJoint` (tension-only).
- `World` solves joints each step for N iterations.
- `IslandBuilder` (union-find; static bodies don't bridge islands) + body **sleeping/rest islands**.
- Demos: pendulum, rope bridge, ragdoll, Newton's cradle.
**LESSON:** velocity-impulse constraints coexist with the Euler integrator because they correct *velocity*, not just position -- no desync.

# Phase 4
## Advanced Collisions
- **Sweep-and-prune** broad phase (verified identical pairs to brute force).
- **CCD / TOI**: analytic swept circle-vs-circle and circle-vs-polygon (expanded faces + vertex caps). Verified a 4000px/s bullet is caught at the surface instead of tunneling.
- Demos: bullet-through-stack, loop track (centripetal), stress test (grid pruned to ~0.15% of brute force at N=3000).

# Phase 5
## Particles & Fluids
- `Particle` + `ParticleSystem` (emit/integrate/lifetime/bounds), spatial-hash particle contacts + `clampToBounds` (PBD ordering).
- Demos: water fountain, sand pile, smoke (alpha fade), liquid-in-box (sloshing).

# Phase 6
## Soft Bodies -- THE CLOTH REDEMPTION
- `SoftBody`: **Verlet integration + position-based distance constraints**. Because velocity == `pos - prev`, constraint position-edits feed straight back into motion. This is *exactly* what the original cloth got wrong (Euler velocity + positional sticks -> desync -> explosion).
- rope/cloth factories (structural + shear constraints). `RopeJoint` for rigid ropes.
- Demos: rope swing, flag in wind, jelly cube, cloth+fluid (two-way coupling, verified stable).

# Phase 7
## Polishing + Showcase
- `World` begin/end **contact callbacks** (SAT-confirmed, diffed vs last step).
- `Profiler` (scoped timers) + `ObjectPool<T>` (free-list allocator).
- Showcase demos: mixed scene (wrecking ball + sparks via callbacks), destructible bridge (breakable links), tornado (vortex force field).

---
**ROADMAP COMPLETE (Phases 1-7 + bonus).** 24 demos. The cloth that was deleted in Phase 3's first commit was rebuilt in `clothe_simulation.cpp` on the SoftBody foundation -- full circle.
