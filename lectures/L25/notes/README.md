# L25 - Anteckningar

Enkel implementation av ett conv-lager i C++ via en strukt döpt `ConvLayer`.  
För att hålla implementationen enkel har enbart en källkodsfil [conv_demo.cpp](./conv_demo.cpp) skapats:
* Input och output-storleken har satts till 4x4.
* Kernelstorleken har satts till 2x2.
* Feedforward körs med en matris som visar en nolla skriven med ettor.
* Backpropagation körs med en gradient-matris innehållande ettor.

### Kompilering samt exekvering av programmet

---

Kör programmet genom att skriva kommandot `make` i terminalen:

```bash
make
```
