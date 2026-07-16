# Physix2D

A lightweight 2D physics engine in C++ with an SFML renderer — built from scratch as a
learning journey and passion project. Rigid bodies, joints, continuous collision,
particles/fluids, and soft bodies. All 7 roadmap phases complete.

> Videos below are hosted as GitHub Release assets. Click any thumbnail to play.

---

## Showcase (ranked by wow-factor)

### 1. Tornado
A wandering vortex force field — swirl + inward pull + lift — sucks up and flings debris.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/tornado_demo.mp4

### 2. Mixed Scene
A wrecking ball on a joint chain smashes a box pyramid; spark particles burst on impact via
World begin-contact callbacks. Rigid bodies + joints + particles + events, all at once.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/mixed_scene_demo.mp4

### 3. Cloth + Fluid
Water is poured onto a hanging cloth net — two-way coupling: the water sags the cloth, the
cloth catches and deflects the water.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/cloth_fluid_demo.mp4

---

### More demos

<table>
<tr>
<td width="50%">

**4. Stress Test** — 1500 balls via a spatial-hash broad phase (~0.15% of brute-force checks).

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/stress_test_demo.mp4

</td>
<td width="50%">

**5. Destructible Bridge** — distance-joint links snap when stretched past a threshold.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/destructible_bridge_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**6. Bullet Through Stack** — CCD catches a 4000 px/s bullet at the surface instead of tunneling.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bullet_through_stack_demo.mp4

</td>
<td width="50%">

**7. Ragdoll** — revolute-jointed torso/head/limbs flop and settle.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ragdoll_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**8. Newton's Cradle** — near-elastic momentum transfer; only the far ball ejects.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/newtons_cradle_demo.mp4

</td>
<td width="50%">

**9. Flag in Wind** — a left-edge-pinned cloth ripples in a gusting wind field.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/flag_in_wind_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**10. Cloth Simulation** — a PBD curtain (the demo that was deleted early on, rebuilt correctly).

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/clothe_simulation.mp4

</td>
<td width="50%">

**11. Water Fountain** — a particle jet arcs under gravity and pools.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/water_fountain_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**12. Liquid in a Box** — a body of water sloshes as gravity rocks side to side.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/liquid_in_box_demo.mp4

</td>
<td width="50%">

**13. Loop Track** — a ball races the inside of a loop, held up by centripetal contact.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/loop_track_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**14. Sand Pile** — grains trickle down and heap up (stiff, low-restitution contacts).

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/sand_pile_demo.mp4

</td>
<td width="50%">

**15. Smoke** — buoyant particles rise, expand, and fade out.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/smoke_effect_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**16. Jelly Cube** — an unpinned soft-body grid squishes and wobbles.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/jelly_cube_demo.mp4

</td>
<td width="50%">

**17. Domino Chain** — a nudge topples a full polygon-collision cascade.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/domino_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**18. Box Stack** — a pyramid settles on resting contacts.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/box_stack_demo.mp4

</td>
<td width="50%">

**19. Rope Bridge** — a plank walkway of revolute-jointed segments.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_bridge_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**20. Rope Swing** — a weighted PBD rope swings and settles.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_swing_demo.mp4

</td>
<td width="50%">

**21. Pendulum** — a distance-joint chain hangs and swings.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/pendulum_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**22. Ball in Box** — bouncing balls with damping and friction.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ball_in_box_demo.mp4

</td>
<td width="50%">

**23. Colliding Balls** — circle-circle impulse collisions.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/colliding_balls_demo.mp4

</td>
</tr>
<tr>
<td width="50%">

**24. Bouncing Balls** — where it all started.

https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bouncing_balls_demo.mp4

</td>
<td width="50%"></td>
</tr>
</table>

---

## Getting Started

```bash
# 1. Install deps (macOS): cmake, pkg-config, sfml@2
brew install cmake pkg-config sfml@2

# 2. Build the library
./build_lib.sh

# 3. Build all demos
./examples/build_examples.sh

# 4. Run a demo
./examples/build/tornado_demo
```

Record a demo to PNG frames (used to make the videos above):

```bash
PHYSIX_RECORD=1 PHYSIX_OUTDIR=/tmp/frames PHYSIX_FRAMES=300 PHYSIX_SUBSTEPS=2 \
  ./examples/build/tornado_demo
# then: ffmpeg -framerate 30 -i /tmp/frames/frame_%05d.png out.mp4
```

---

## Features

- **Rigid bodies** — circles and convex polygons, SAT narrow phase with face clipping,
  rotational dynamics (angle / angular velocity / torque / moment of inertia).
- **Solver** — impulse resolution with angular terms, Coulomb friction, Baumgarte
  positional correction; inverse-mass static bodies; body sleeping / rest islands.
- **Joints** — DistanceJoint, RevoluteJoint (2x2 effective mass), RopeJoint (tension-only);
  union-find island builder.
- **Broad phase** — uniform spatial hash and sweep-and-prune.
- **Continuous collision (CCD)** — analytic swept circle-vs-circle and circle-vs-polygon TOI.
- **Particles & fluids** — ParticleSystem with spatial-hash contacts.
- **Soft bodies** — Verlet integration + position-based distance constraints (rope, cloth, jelly).
- **Tooling** — begin/end contact callbacks, scoped Profiler, ObjectPool allocator.

---

## Development Roadmap

### Phase 1: Core Physics + Visual Debug
- [x] `Vec2` (basic vector math)
- [x] `Body` (pos, vel, mass, radius)
- [x] `World` (gravity + integration loop)
- [x] `DebugDraw` (SFML)
- [x] Falling / rain / bouncing ball demos

### Phase 2: Collision Detection + Resolution
- [x] `Shape` base class, `CircleShape`, `PolygonShape`
- [x] Collision detection (circle-circle, circle-polygon, polygon-polygon via SAT)
- [x] Contact manifold + impulse solver (rotation + Coulomb friction)
- [x] Inverse-mass / static bodies, rotational dynamics, linear damping
- [x] Uniform spatial-hash broad phase
- [x] Ball pit / box stack / domino / ball-in-box demos

### Phase 3: Joints & Constraints
- [x] `Joint` base, `DistanceJoint`, `RevoluteJoint`
- [x] Island solver (union-find + body sleeping)
- [x] Pendulum / rope bridge / ragdoll / Newton's cradle demos

### Phase 4: Advanced Collisions (Broad Phase + CCD)
- [x] Broad phase (sweep-and-prune + uniform hash)
- [x] Continuous collision detection (TOI)
- [x] Bullet-through-stack / loop track / stress test demos

### Phase 5: Particles & Fluids
- [x] `Particle` + `ParticleSystem`, particle contacts
- [x] Water fountain / sand pile / smoke / liquid-in-box demos

### Phase 6: Soft Bodies (Rope, Cloth, Deformables)
- [x] `RopeJoint`, `SoftBody` / `Cloth` (Verlet + position-based dynamics)
- [x] Rope swing / flag in wind / jelly cube / cloth+fluid demos

### Phase 7: Polishing + Showcase
- [x] World begin/end contact callbacks
- [x] Profiler + ObjectPool memory manager
- [x] Mixed scene / destructible bridge / tornado demos

### Bonus: Advanced Simulations
- [x] Cloth simulation (PBD)
- [x] Fluid simulation (particle fluids)
- [x] Smoke simulation

---

Built with C++17 + SFML 2. See `Devlogs.md` for the engineering notes behind each phase.
