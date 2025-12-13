# Physix2D
A lightweight 2D physics engine in C++, built as a learning journey and passion project.  

---

## 🚀 Getting Started
1. Build the library: `./build_lib.sh`  
2. Create a demo in the `examples/src/my_demo` folder and build it: `./examples/build_examples.sh`  
3. Run your demo: `./build/my_demo`  

---

---

## 🎥 Demos
### 1. Bouncing Balls  
<video controls src="assets/bouncing_balls.mp4" title="Bouncing Balls Demo"></video>  

---

### 2. Colliding Balls  
<video controls src="assets/colliding_balls.mp4" title="Bouncing Balls Demo"></video>  

---
---

## ✅ Development Roadmap

### Phase 1: Core Physics + Visual Debug
- [x] `Vec2` (basic vector math)  
- [x] `Body` (circle-only: pos, vel, mass, radius)  
- [x] `World` (gravity + integration loop)  
- [x] `DebugDraw` (SFML/SDL/OpenGL)  
- [x] Falling ball demo 🌕  
- [x] Rain of balls demo 🌧  
- [x] Bouncing off ground demo ⬇️  

---

### Phase 2: Collision Detection + Resolution
- [ ] `Shape` base class  
- [ ] `CircleShape`, `PolygonShape`  
- [x] Collision detection (circle–circle, polygon–polygon)  
- [x] Contact manifold  
- [x] Simple solver (impulse resolution)  
- [x] Ball pit demo ⚪  
- [x] Box stack demo 📦  
- [ ] Domino chain demo 🧱  
- [ ] Ball inside box demo 🏀  

---

### Phase 3: Joints & Constraints
- [ ] `Joint` base class  
- [ ] `RevoluteJoint`, `DistanceJoint`  
- [ ] Island solver  
- [ ] Pendulum demo 🔗  
- [ ] Rope bridge demo 🌉  
- [ ] Ragdoll demo 🕺  
- [ ] Newton’s cradle demo 🪀  

---

### Phase 4: Advanced Collisions (Broad Phase + CCD)
- [ ] Broad-phase (dynamic tree, sweep & prune)  
- [ ] Continuous collision detection (TOI)  
- [ ] Bullet through stack demo 💥  
- [ ] Loop track demo 🎢  
- [ ] Stress test with thousands of objects 🌊  

---

### Phase 5: Particles & Fluids
- [ ] `Particle` + `ParticleSystem`  
- [ ] Particle contacts  
- [ ] Water fountain demo 💧  
- [ ] Sand pile demo 🏖  
- [ ] Smoke effect demo ☁️  
- [ ] Liquid in a box demo 🌊  

---

### Phase 6: Soft Bodies (Rope, Cloth, Deformables)
- [ ] `Rope`, `RopeJoint`  
- [ ] `SoftBody`, `Cloth`  
- [ ] Rope swing demo 🪢  
- [ ] Flag in wind demo 🏴  
- [ ] Jelly cube demo 🍮  
- [ ] Cloth + fluid interaction demo 🌊  

---

### Phase 7: Polishing + Showcase
- [ ] World callbacks (begin/end contact)  
- [ ] Profiler & memory manager  
- [ ] Mixed scene demo 🎮  
- [ ] Destructible bridge demo 🏗  
- [ ] Tornado effect demo 🌀  

---

### BONUS: Advanced Simulations -
- [ ] Clothe simulation
- [ ] Fluid simulation
- [ ] Smoke simulation

---
