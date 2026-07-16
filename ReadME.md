# Physix2D

A lightweight 2D physics engine in C++ with an SFML renderer — built from scratch as a
learning journey and passion project. Rigid bodies, joints, continuous collision,
particles/fluids, and soft bodies. All 7 roadmap phases complete.

> Demo videos are hosted as GitHub Release assets and embedded below.

---

## Showcase (ranked by wow-factor)

### 1. Tornado
A wandering vortex force field — swirl + inward pull + lift — sucks up and flings debris.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/tornado_demo.mp4" controls muted loop width="100%"></video>

### 2. Mixed Scene
A wrecking ball on a joint chain smashes a box pyramid; spark particles burst on impact via
World begin-contact callbacks. Rigid bodies + joints + particles + events, all at once.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/mixed_scene_demo.mp4" controls muted loop width="100%"></video>

### 3. Cloth + Fluid
Water is poured onto a hanging cloth net — two-way coupling: the water sags the cloth, the
cloth catches and deflects the water.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/cloth_fluid_demo.mp4" controls muted loop width="100%"></video>

---

### More demos

<table>
<tr>
<td width="50%">

**4. Stress Test** — 1500 balls via a spatial-hash broad phase (~0.15% of brute-force checks).

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/stress_test_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**5. Destructible Bridge** — distance-joint links snap when stretched past a threshold.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/destructible_bridge_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**6. Bullet Through Stack** — CCD catches a 4000 px/s bullet at the surface instead of tunneling.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bullet_through_stack_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**7. Ragdoll** — revolute-jointed torso/head/limbs flop and settle.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ragdoll_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**8. Newton's Cradle** — near-elastic momentum transfer; only the far ball ejects.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/newtons_cradle_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**9. Flag in Wind** — a left-edge-pinned cloth ripples in a gusting wind field.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/flag_in_wind_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**10. Cloth Simulation** — a PBD curtain (the demo deleted early on, rebuilt correctly).

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/clothe_simulation.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**11. Water Fountain** — a particle jet arcs under gravity and pools.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/water_fountain_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**12. Liquid in a Box** — a body of water sloshes as gravity rocks side to side.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/liquid_in_box_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**13. Loop Track** — a ball races the inside of a loop, held up by centripetal contact.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/loop_track_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**14. Sand Pile** — grains trickle down and heap up (stiff, low-restitution contacts).

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/sand_pile_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**15. Smoke** — buoyant particles rise, expand, and fade out.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/smoke_effect_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**16. Jelly Cube** — an unpinned soft-body grid squishes and wobbles.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/jelly_cube_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**17. Domino Chain** — a nudge topples a full polygon-collision cascade.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/domino_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**18. Box Stack** — a pyramid settles on resting contacts.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/box_stack_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**19. Rope Bridge** — a plank walkway of revolute-jointed segments.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_bridge_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**20. Rope Swing** — a weighted PBD rope swings and settles.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_swing_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**21. Pendulum** — a distance-joint chain hangs and swings.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/pendulum_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**22. Ball in Box** — bouncing balls with damping and friction.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ball_in_box_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%">

**23. Colliding Balls** — circle-circle impulse collisions.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/colliding_balls_demo.mp4" controls muted loop width="100%"></video>

</td>
</tr>
<tr>
<td width="50%">

**24. Bouncing Balls** — where it all started.

<video src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bouncing_balls_demo.mp4" controls muted loop width="100%"></video>

</td>
<td width="50%"></td>
</tr>
</table>

> If a player doesn't load inline, click it to open the video — all clips live on the
> [demo-reel-v1 release](https://github.com/GaganMishra305/physix2d/releases/tag/demo-reel-v1).

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

Record a demo to PNG frames (how the videos above were made):

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
- [x] `Vec2`, `Body`, `World` (gravity + integration), SFML `DebugDraw`
- [x] Falling / rain / bouncing ball demos

### Phase 2: Collision Detection + Resolution
- [x] `Shape` / `CircleShape` / `PolygonShape`
- [x] SAT detection (circle-circle, circle-polygon, polygon-polygon) + contact manifold
- [x] Impulse solver (rotation + Coulomb friction), static bodies, damping
- [x] Uniform spatial-hash broad phase; ball pit / box stack / domino / ball-in-box demos

### Phase 3: Joints & Constraints
- [x] `Joint`, `DistanceJoint`, `RevoluteJoint`; island solver + body sleeping
- [x] Pendulum / rope bridge / ragdoll / Newton's cradle demos

### Phase 4: Advanced Collisions (Broad Phase + CCD)
- [x] Sweep-and-prune broad phase; continuous collision detection (TOI)
- [x] Bullet-through-stack / loop track / stress test demos

### Phase 5: Particles & Fluids
- [x] `Particle` + `ParticleSystem`, particle contacts
- [x] Water fountain / sand pile / smoke / liquid-in-box demos

### Phase 6: Soft Bodies
- [x] `RopeJoint`, `SoftBody` / `Cloth` (Verlet + position-based dynamics)
- [x] Rope swing / flag in wind / jelly cube / cloth+fluid demos

### Phase 7: Polishing + Showcase
- [x] World begin/end contact callbacks; Profiler + ObjectPool
- [x] Mixed scene / destructible bridge / tornado demos

### Bonus: Advanced Simulations
- [x] Cloth (PBD) / Fluid (particle) / Smoke simulations

---

Built with C++17 + SFML 2. See `Devlogs.md` for the engineering notes behind each phase.
