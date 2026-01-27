# L27 - Anteckningar

Enkel implementation av ett flatten-lager i C++ via en strukt döpt `FlattenLayer`.  
För att hålla implementationen enkel har enbart en källkodsfil [flatten_demo.cpp](./flatten_demo.cpp) skapats:
* Input-storleken har satts till 4x4.
* Output-storleken har därmed satts till 4*4 = 16.

Feedforward körs för att platta den givna indatan från 2D till 1D:

```bash
2.0, 1.0, 6.0, 1.0
3.0, 0.0, 4.0, 6.0
1.0, 2.0, 4.0, 5.0
3.0, 4.0, 7.0, 7.0
```

Tillplattningen medför följande utdata:

```bash
2.0, 1.0, 6.0, 1.0, 3.0, 0.0, 4.0, 6.0, 1.0, 2.0, 4.0, 5.0, 3.0, 4.0, 7.0, 7.0
```

Backpropagation körs med följande matris innehållande utdata-gradienter:

```bash
1.0, 2.0, 3.0, 4.0, 8.0, 7.0, 6.0, 5.0, 0.0, 2.0, 4.0, 8.0, 9.0, 7.0, 5.0, 3.0
```

Dessa gradienter formas om från 1D till 2D, såsom visas nedan:

```bash
1.0, 2.0, 3.0, 4.0
8.0, 7.0, 6.0, 5.0
0.0, 2.0, 4.0, 8.0
9.0, 7.0, 5.0, 3.0
```

Nedan visas programmets output:

```bash
Flattening input data (2D -> 1D):
        2.0, 1.0, 6.0, 1.0
        3.0, 0.0, 4.0, 6.0
        1.0, 2.0, 4.0, 5.0
        3.0, 4.0, 7.0, 7.0

Resulting flattened output (1D):
        2.0, 1.0, 6.0, 1.0, 3.0, 0.0, 4.0, 6.0, 1.0, 2.0, 4.0, 5.0, 3.0, 4.0, 7.0, 7.0

Applying backpropagation (1D -> 2D):
        1.0, 2.0, 3.0, 4.0, 8.0, 7.0, 6.0, 5.0, 0.0, 2.0, 4.0, 8.0, 9.0, 7.0, 5.0, 3.0

Resulting unflattened input gradients (2D):
        1.0, 2.0, 3.0, 4.0
        8.0, 7.0, 6.0, 5.0
        0.0, 2.0, 4.0, 8.0
        9.0, 7.0, 5.0, 3.0
```

### Kompilering samt exekvering av programmet

---

Kör programmet genom att skriva kommandot `make` i terminalen:

```bash
make
```


