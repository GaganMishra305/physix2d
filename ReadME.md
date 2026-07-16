# Physix2D

A lightweight 2D physics engine in C++ with an SFML renderer — built from scratch as a
learning journey and passion project. Rigid bodies, joints, continuous collision,
particles/fluids, and soft bodies. All 7 roadmap phases complete.

> Each clip below is an animated preview — click any one to open the full-quality video.

---

## Showcase

### 1. Domino Chain
A single nudge topples a full polygon-collision cascade.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/domino_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/domino_demo.gif" width="640"></a>

### 2. Cloth + Fluid
Water poured onto a hanging cloth net — two-way coupling: the water sags the cloth, the
cloth catches and deflects the water.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/cloth_fluid_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/cloth_fluid_demo.gif" width="640"></a>

### 3. Tornado
A wandering vortex force field — swirl + inward pull + lift — sucks up and flings debris.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/tornado_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/tornado_demo.gif" width="640"></a>

---

### More demos

<table>
<tr>
<td width="50%" valign="top">

**4. Mixed Scene** — a wrecking ball on a joint chain smashes a box pyramid; spark particles burst on impact via contact callbacks.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/mixed_scene_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/mixed_scene_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**5. Stress Test** — 1500 balls resolved through a spatial-hash broad phase.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/stress_test_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/stress_test_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**6. Destructible Bridge** — distance-joint links snap past a stretch threshold.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/destructible_bridge_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/destructible_bridge_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**7. Bullet Through Stack** — CCD catches a 4000 px/s bullet instead of tunneling.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bullet_through_stack_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bullet_through_stack_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**8. Ragdoll** — revolute-jointed torso/head/limbs flop and settle.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ragdoll_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ragdoll_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**9. Newton's Cradle** — near-elastic momentum transfer; only the far ball ejects.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/newtons_cradle_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/newtons_cradle_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**10. Flag in Wind** — a left-edge-pinned cloth ripples in a gusting wind field.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/flag_in_wind_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/flag_in_wind_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**11. Cloth Simulation** — a position-based-dynamics curtain.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/clothe_simulation.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/clothe_simulation.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**12. Water Fountain** — a particle jet arcs under gravity and pools.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/water_fountain_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/water_fountain_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**13. Liquid in a Box** — a body of water sloshes as gravity rocks side to side.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/liquid_in_box_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/liquid_in_box_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**14. Loop Track** — a ball races the inside of a loop, held by centripetal contact.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/loop_track_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/loop_track_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**15. Sand Pile** — grains trickle down and heap up.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/sand_pile_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/sand_pile_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**16. Smoke** — buoyant particles rise, expand, and fade out.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/smoke_effect_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/smoke_effect_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**17. Jelly Cube** — an unpinned soft-body grid squishes and wobbles.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/jelly_cube_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/jelly_cube_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**18. Box Stack** — a pyramid settles on resting contacts.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/box_stack_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/box_stack_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**19. Rope Bridge** — a plank walkway of revolute-jointed segments.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_bridge_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_bridge_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**20. Rope Swing** — a weighted rope swings and settles.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_swing_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/rope_swing_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**21. Pendulum** — a distance-joint chain hangs and swings.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/pendulum_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/pendulum_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**22. Ball in Box** — bouncing balls with damping and friction.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ball_in_box_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/ball_in_box_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top">

**23. Colliding Balls** — circle-circle impulse collisions.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/colliding_balls_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/colliding_balls_demo.gif" width="100%"></a>

</td>
</tr>
<tr>
<td width="50%" valign="top">

**24. Bouncing Balls** — where it all started.

<a href="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bouncing_balls_demo.mp4"><img src="https://github.com/GaganMishra305/physix2d/releases/download/demo-reel-v1/bouncing_balls_demo.gif" width="100%"></a>

</td>
<td width="50%" valign="top"></td>
</tr>
</table>

> Previews are GIFs; click any to open the full-quality MP4. All clips live on the
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
./examples/build/domino_demo
```

Record a demo to PNG frames:

```bash
PHYSIX_RECORD=1 PHYSIX_OUTDIR=/tmp/frames PHYSIX_FRAMES=300 PHYSIX_SUBSTEPS=2 \
  ./examples/build/tornado_demo
# then: ffmpeg -framerate 30 -i /tmp/frames/frame_%05d.png out.mp4
```

---

## Features

- **Rigid bodies** — circles and convex polygons, SAT narrow phase with face clipping,
  rotational dynamics (angle, angular velocity, torque, moment of inertia).
- **Solver** — impulse resolution with angular terms, Coulomb friction, Baumgarte
  positional correction; inverse-mass static bodies; body sleeping / rest islands.
- **Joints** — DistanceJoint, RevoluteJoint, RopeJoint; union-find island builder.
- **Broad phase** — uniform spatial hash and sweep-and-prune.
- **Continuous collision (CCD)** — analytic swept circle-vs-circle and circle-vs-polygon TOI.
- **Particles & fluids** — ParticleSystem with spatial-hash contacts.
- **Soft bodies** — Verlet integration with position-based distance constraints (rope, cloth, jelly).
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
- [x] Cloth / Fluid / Smoke simulations

---

Built with C++17 + SFML 2. See `Devlogs.md` for the engineering notes behind each phase.
