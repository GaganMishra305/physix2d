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

---

# Phase 2
