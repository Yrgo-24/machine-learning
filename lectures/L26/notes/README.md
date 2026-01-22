# L26 - Anteckningar

Enkel implementation av ett maxpooling-lager i C++ via en strukt döpt `MaxPoolingLayer`.  
För att hålla implementationen enkel har enbart en källkodsfil [max_pool_demo.cpp](./max_pool_demo.cpp) skapats:
* Input-storleken har satts till 4x4.
* Pool-storleken har satts till 2x2.
* Output-storleken blir 4 / 2 = 2x2.


Feedforward körs för att extrahera maxvärdet ur respektive pool i den givna indatan:

```bash
2.0, 1.0, 6.0, 1.0
3.0, 0.0, 4.0, 6.0
1.0, 2.0, 4.0, 5.0
3.0, 4.0, 7.0, 7.0
```

Poolingen medför följande utdata:

```bash
3.0, 6.0
4.0, 7.0
```

Backpropagation körs med följande matris innehållande utdata-gradienter:

```bash
1.0, 2.0
3.0, 4.0
```

Dessa gradienter matas bakåt till de celler som innehöll de extraherade maxvärdena. Indata-gradienterna
efter backpropagation visas nedan:

```bash
0.0, 0.0, 2.0, 0.0
1.0, 0.0, 0.0, 0.0
0.0, 0.0, 0.0, 0.0
0.0, 3.0, 4.0, 0.0
```

Nedan visas programmets output:

```bash
Pooling input data (2D):
        2.0, 1.0, 6.0, 1.0
        3.0, 0.0, 4.0, 6.0
        1.0, 2.0, 4.0, 5.0
        3.0, 4.0, 7.0, 7.0

Pooled output (2D):
        3.0, 6.0
        4.0, 7.0

Pooling output gradients (2D):
        1.0, 2.0
        3.0, 4.0

Input gradients after backpropagation (2D):
        0.0, 0.0, 2.0, 0.0
        1.0, 0.0, 0.0, 0.0
        0.0, 0.0, 0.0, 0.0
        0.0, 3.0, 4.0, 0.0
```

### Kompilering samt exekvering av programmet

---

Kör programmet genom att skriva kommandot `make` i terminalen:

```bash
make
```


