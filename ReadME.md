# Physix2D
A lightweight 2D physics engine in C++ — built as a learning journey and passion project.  

---

## 🚀 Getting Started
1. Build the library: `./build_lib.sh`  
2. Create a demo in the `examples/` folder and build it: `./build_examples`  
3. Run your demo: `./build/my_demo`  

---

## ✅ Development Roadmap

### Phase 1: Core Physics + Visual Debug  
- [x] `Vec2` (vector math)  
- [x] `Body` (circle-only: pos, vel, mass, radius)  
- [x] `World` (gravity + integration)  
- [x] `DebugDraw` (SFML/SDL/OpenGL)  
- [x] Falling ball 🌕 | Rain 🌧 | Bounce ⬇️  

---

### Phase 2: Collision Detection & Resolution  
- [ ] `Shape` hierarchy (`Circle`, `Polygon`)  
- [ ] Collision detection (circle–circle, polygon–polygon)  
- [ ] Contact manifold + impulse solver  
- [ ] Demos: Ball pit ⚪ | Box stack 📦 | Domino chain 🧱 | Ball-in-box 🏀  

---

### Phase 3: Joints & Constraints  
- [ ] `Joint` base + revolute & distance joints  
- [ ] Island solver  
- [ ] Demos: Pendulum 🔗 | Rope bridge 🌉 | Ragdoll 🕺 | Newton’s cradle 🪀  

---

### Phase 4: Advanced Collisions  
- [ ] Broad-phase (dynamic tree / sweep & prune)  
- [ ] Continuous collision detection (TOI)  
- [ ] Demos: Bullet-through-stack 💥 | Loop track 🎢 | Stress test 🌊  

---

### Phase 5: Particles & Fluids  
- [ ] `Particle` + `ParticleSystem`  
- [ ] Particle contacts  
- [ ] Demos: Fountain 💧 | Sand pile 🏖 | Smoke ☁️ | Liquid in box 🌊  

---

### Phase 6: Soft Bodies  
- [ ] Ropes, cloth, deformables  
- [ ] Demos: Rope swing 🪢 | Flag in wind 🏴 | Jelly cube 🍮 | Cloth + fluid 🌊  

---

### Phase 7: Polishing & Showcase  
- [ ] Callbacks, profiler, memory manager  
- [ ] Demos: Mixed scene 🎮 | Destructible bridge 🏗 | Tornado 🌀  

---

## 🎥 Demos
### 1. Bouncing Balls  
<video controls src="https://github.com/user-attachments/assets/c6a3d3f9-1e3b-416a-9875-dc1ea4957843" title="Bouncing Balls Demo"></video>  
