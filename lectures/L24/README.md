# L24 - Konvolutionella neurala nätverk (del II)

## Dagordning
* Träning av konvolutionella neurala nätverk för hand.
* Arbete med `Inlämningsuppgift 6 - Konvolutionellt neuralt nätverk.`

## Mål med lektionen
* Kunna träna enkla konvolutionella neurala nätverk för hand.
* Påbörja arbete med `Inlämningsuppgift 6`, exempelvis genom att lägga till headerfilen för klassen `ml::conv_layer::Conv`.

## Instruktioner
* Se [bilaga A](#bilaga-a---träning-av-ett-litet-konvolutionellt-neuralt-nätverk) nedan för information om
träning av konvolutionella neurala nätverk.

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Implementation av conv-lager i mjukvara.

## Bilaga A - Träning av ett litet konvolutionellt neuralt nätverk

Vi har följande konvolutionella neurala nätverk:
- Input: **4×4**
- Conv-lager:
  - Kernel: **2×2**
  - Stride = 1 *(kerneln flyttas ett pixelsteg i taget)*
  - Zero-padding *(vi lägger till nollor runt bilden så att bildstorleken behålls trots extraktionen)*
  - Aktivering: **ReLU** *(alla negavtiva värden ersätts med noll)*
  - Lärhastighet: **LR** = 0.001, dvs. 1 %.
- Maxpool:
  - Poolstorlek = 2
  - Stride = 2 *(poolerna är icke-överlappande)*
- Flatten:
  - 2×2 → 1×4
---

### 1. Passering genom conv-lagret

#### a) - Padding av inputbilden

Anta att vi har följande inputbild, som efterliknar siffran 0 i form av ettor:

```
1 1 1 1
1 0 0 1
1 0 0 1
1 1 1 1
```


Det första steget är att använda *zero-padding*. Det innebär att vi lägger till nollor runt bilden så att dess storlek behålls, även efter att vi applicerat kernel-filtret. På så sätt får conv-lagrets utdata samma dimensioner som indatan, vilket i detta fall är **4×4**.

Antalet nollor `n` vi ska lägga till åt respektive håll beräknas enligt nedan:

```
n = kernel_size / 2,
```


där `kernel_size` är storleken på kernel-filtret, som är `2` i detta exempel. Det betyder att vi lägger till en nolla på varje sida. Den paddade input-bilden får då storleken **(6×6)**, som visas nedan:

```
0 0 0 0 0 0
0 1 1 1 1 0
0 1 0 0 1 0
0 1 0 0 1 0
0 1 1 1 1 0
0 0 0 0 0 0
```

---


#### b) - Kernel och bias


Vi använder en kernel (ett filter med vikter) och en bias. Här har vi valt enkla, fasta värden för att göra beräkningarna lätta att följa:

**Kernel-vikter:**
```
0.2 0.4
0.6 0.8
```

**Bias:**
```
0.5
```

---

#### c) - Feedforward genom conv-lagret


För varje position i bilden beräknas summan av elementvis multiplikation mellan kernel och motsvarande del av bilden, plus bias. Därefter appliceras ReLU-aktivering (alla negativa värden ersätts med noll):


```
sum = a·k00 + b·k01 + c·k10 + d·k11 + bias
output = ReLU(sum)
```

där:
- **a, b, c, d** är pixelvärden från den aktuella 2×2-delen av bilden.
- **k** står för kernel (filtret), och t.ex. `k00` betyder värdet på rad 0, kolumn 0 i kernel-matrisen.
- **bias** är ett konstant värde som läggs till summan.

Exempel: `k00` är värdet i övre vänstra hörnet av kernel.


**Fullständig Conv-output (4×4):**


När vi har applicerat kernel och bias på hela bilden får vi följande utdata från conv-lagret:

```
1.3 1.9 1.9 1.9
1.7 1.7 1.1 1.9
1.7 1.3 0.5 1.7
1.7 2.1 1.9 2.3
```

---

#### d) - Maxpool-lagret (feedforward)

Maxpool-lagret söker efter det största värdet i varje 2×2-pool (icke-överlappande) från conv-lagrets utdata.
Syftet är att sampla ned bilden: vi tar bort detaljer men behåller de viktigaste dragen.
Detta minskar mängden data och gör modellen mindre känslig för små variationer i bilden.

Vi delar därför upp conv-lagrets utdata i följande pooler:

**1 - Det övre vänstra hörnet**

Poolen i det övra vänstra hörnet har max-värde `1.9`, som därför skickas vidare:

```
1.3 1.9
1.7 1.7
```

**2 - Det övre högra hörnet**

Poolen i det övra högra hörnet har max-värde `1.9` på två ställen. Vi skickar vidare den första instansen:

```
1.9 1.9
1.1 1.9
```

**3 - Det nedre vänstra hörnet**

Poolen i det nedre vänstra hörnet har max-värde `2.1`, som därför skickas vidare:

```
1.7 1.3
1.7 2.1
```

**4 - Det nedre högra hörnet**

Poolen i det nedre högra hörnet har max-värde `2.3`, som därför skickas vidare:

```
0.5 1.7
1.9 2.3
```

Maxpool-lagrets utdata blir därmed följande:

```
1.9 1.9
2.1 2.3
```

---

#### e) - Flatten (feedforward)


Maxpool-lagrets utdata plattas sedan ut till en vektor (flatten), så att den kan matas vidare till nästa lager:

```
[1.9, 1.9, 2.1, 2.3]
```

---

### 2. Backpropagation

Anta att dense-lagret skickar tillbaka följande gradienter:
```
[10, 20, 30, 40]
```

---

#### a) - Flatten (backpropagation)


Gradienterna från det täta lagret (dense) formas tillbaka till en matris:

```
10 20
30 40
```

---

#### b) - Maxpool (backpropagation)


Gradienterna sprids tillbaka till rätt positioner i maxpool-lagret, det vill säga till de platser där max-värdena låg i varje pool.

Vi har följande gradienter att sprida tillbaka:

```
10 20
30 40
```

Maxpooling-lagrets indata visas nedan:

```
1.3 1.9 1.9 1.9
1.7 1.7 1.1 1.9
1.7 1.3 0.5 1.7
1.7 2.1 1.9 2.3
```

Vi sprider tillbaka de fyra gradienterna till positionen för max-värdena i respektive pool. Har vi två max-värden i en pool
sprider vi tillbaka värden till den första positionen. Resterande gradienter sätts till noll.

**1 - Det övre vänstra hörnet**

Vi sprider tillbaka gradianten `10` till positionen för maxvärdet. I denna pool är maxvärdet `1.9` 
längst upp till höger. Vi sprider därmed tillbaka `10` på motsvarande position bland gradienterna:

```
0 10
0  0
```

**2 - Det övre högra hörnet**

Vi sprider tillbaka gradianten `20` till positionen för maxvärdet. I denna pool förekommer 
maxvärdet `1.9` på två ställen, varvid vi använder oss utav den första positionen,
vilket är längst upp till vänster. Vi sprider därmed tillbaka `20` på motsvarande position bland gradienterna:

```
20  0
0  0
```

**3 - Det nedre vänstra hörnet**

Vi sprider tillbaka gradianten `30` till positionen för maxvärdet. I denna pool är maxvärdet `2.1` 
längst ned till höger. Vi sprider därmed tillbaka `30` på motsvarande position bland gradienterna:

```
0  0 
0 30
```


**4 - Det nedre högra hörnet**

Vi sprider tillbaka gradianten `40` till positionen för maxvärdet. I denna pool är maxvärdet `2.3` 
längst ned till höger. Vi sprider därmed tillbaka `40` på motsvarande position bland gradienterna:

```
0  0
0 40
```

---

När vi sätter ihop alla poolers gradienter till en hel matris erhålls följande:

```
0 10 20  0
0  0  0  0
0  0  0  0
0 30  0 40
```

---

#### c) - Conv-lager (backpropagation)



Nu beräknar vi gradienten för bias och kernel utifrån de fel som skickats bakåt:

**Bias-gradient:**

Bias-gradienten beräknas som summan av alla värden i gradientmatrisen:

```math
biasGradient = 0 + 10 + 20 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 30 + 0 + 40 = 100
```

**Kernel-gradienter:**

För varje element i kernel summerar vi produkten av varje patch i den paddade inputbilden och motsvarande värde i gradientmatrisen. Här är alla steg:

Vi använder den paddade inputbilden (6x6):

```
0 0 0 0 0 0
0 1 1 1 1 0
0 1 0 0 1 0
0 1 0 0 1 0
0 1 1 1 1 0
0 0 0 0 0 0
```

och gradientmatrisen från pooling-lagret (4x4):

```
0 10 20  0
0  0  0  0
0  0  0  0
0 30  0 40
```

För varje position (i, j) i gradientmatrisen, extraheras en 2x2-patch ur inputbilden. Varje kernel-element multipliceras med motsvarande värde i patchen och summeras över alla positioner.

Vi visar nu alla multiplikationer och summeringar för varje kernel-element:

**För k00** (övre vänstra kernel-vikten):

```math
k00 = 0\cdot0 + 0\cdot10 + 0\cdot20 + 0\cdot0 \\
\quad + 0\cdot0 + 1\cdot10 + 1\cdot20 + 1\cdot0 \\
\quad + 0\cdot0 + 1\cdot0 + 0\cdot0 + 0\cdot0 \\
\quad + 0\cdot0 + 1\cdot30 + 1\cdot0 + 1\cdot40,
```

vilket kan förenklas till

```math
k00 = 0 + 0 + 0 + 0 + 0 + 10 + 20 + 0 + 0 + 0 + 0 + 0 + 0 + 30 + 0 + 40 = 100
```

**För k01** (övre högra kernel-vikten):

```math
k01 = 0\cdot0 + 0\cdot10 + 0\cdot20 + 0\cdot0 \\
\quad + 1\cdot0 + 1\cdot10 + 1\cdot20 + 0\cdot0 \\
\quad + 1\cdot0 + 0\cdot0 + 0\cdot0 + 1\cdot0 \\
\quad + 1\cdot0 + 1\cdot30 + 1\cdot0 + 0\cdot40,
```

vilket kan förenklas till

```math
k01 = 0 + 0 + 0 + 0 + 0 + 10 + 20 + 0 + 0 + 0 + 0 + 0 + 0 + 30 + 0 + 0 = 60
```

**För k10** (nedre vänstra kernel-vikten):

```math
k10 = 0\cdot0 + 1\cdot10 + 1\cdot20 + 1\cdot0 \\
\quad + 0\cdot0 + 1\cdot0 + 0\cdot0 + 0\cdot0 \\
\quad + 0\cdot0 + 1\cdot30 + 1\cdot0 + 1\cdot40 \\
\quad + 0\cdot0 + 0\cdot0 + 0\cdot0 + 0\cdot0,
```

vilket kan förenklas till

```math
k10 = 0 + 10 + 20 + 0 + 0 + 0 + 0 + 0 + 0 + 30 + 0 + 40 + 0 + 0 + 0 + 0 = 100
```

**För k11** (nedre högra kernel-vikten):

```math
k11 = 1\cdot0 + 1\cdot10 + 0\cdot20 + 1\cdot0 \\
\quad + 1\cdot0 + 0\cdot10 + 0\cdot20 + 1\cdot0 \\
\quad + 1\cdot0 + 1\cdot0 + 1\cdot0 + 0\cdot0 \\
\quad + 1\cdot0 + 1\cdot30 + 1\cdot0 + 0\cdot40,
```

vilket kan förenklas till

```math
k11 = 0 + 10 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 0 + 30 + 0 + 0 = 40
```

Därmed erhålls följande kernel-gradienter:

```
k00 = 100
k01 = 60
k10 = 100
k11 = 40,
```

vilket kan skrivas om till en matris:

```
100 60
100 40
```

---

**Input-gradienter (paddad input):**

Nu beräknar vi hur felet sprids bakåt till indata, dvs. vi beräknar gradienten med avseende på den paddade inputen. Detta görs genom att för varje position i gradientmatrisen "sprida ut" kernelns vikter multiplicerade med gradientvärdet på rätt plats i en ny matris (av samma storlek som den paddade inputen). Detta är en så kallad full convolution med kernelns roterade matris.

Vi använder:

- Kernel (roterad 180°):
  ```
  0.8 0.6
  0.4 0.2
  ```

- Gradientmatris från pooling-lagret (4x4):

  ```
  0 10 20  0
  0  0  0  0
  0  0  0  0
  0 30  0 40
  ```

Vi placerar ut varje kernel multiplicerat med respektive gradientvärde, och summerar överlappande positioner.

**Steg-för-steg:**
1. Skapa en 6x6-matris fylld med nollor (input-gradientmatrisen).
2. För varje position (i, j) i gradientmatrisen:
   - Lägg till kernel * grad[i, j] på position (i, j) i input-gradientmatrisen.

**Explicit uträkning för några positioner:**

- På position (0,1) i input-gradientmatrisen (från grad[0,1]=10):
  - 0.8 * 10 på (0,1)
  - 0.6 * 10 på (0,2)
  - 0.4 * 10 på (1,1)
  - 0.2 * 10 på (1,2)
- På position (0,2) i input-gradientmatrisen (från grad[0,2]=20):
  - 0.8 * 20 på (0,2)
  - 0.6 * 20 på (0,3)
  - 0.4 * 20 på (1,2)
  - 0.2 * 20 på (1,3)
- På position (3,1) i input-gradientmatrisen (från grad[3,1]=30):
  - 0.8 * 30 på (3,1)
  - 0.6 * 30 på (3,2)
  - 0.4 * 30 på (4,1)
  - 0.2 * 30 på (4,2)
- På position (3,3) i input-gradientmatrisen (från grad[3,3]=40):
  - 0.8 * 40 på (3,3)
  - 0.6 * 40 på (3,4)
  - 0.4 * 40 på (4,3)
  - 0.2 * 40 på (4,4)

Fortsätt på detta sätt för alla positioner där grad[i,j] ≠ 0.

**Slutlig input-gradientmatris (paddad):**

Efter att ha summerat alla bidrag får vi:
```
0.0  8.0 22.0 12.0  0.0  0.0
0.0  4.0 16.0 16.0  4.0  0.0
0.0  0.0  8.0  8.0  0.0  0.0
0.0 12.0 24.0 32.0 24.0 24.0
0.0 12.0 12.0 16.0  8.0  8.0
0.0  0.0  0.0  0.0  0.0  0.0
```

**Extrahera opaddade gradienter:**

Vi tar bort yttersta raden och kolumnen runt om för att få en 4x4-matris som matchar originalbilden:
```
4.0 16.0 16.0  4.0
0.0  8.0  8.0  0.0
12.0 24.0 32.0 24.0
12.0 12.0 16.0  8.0
```

Detta är gradienten med avseende på indata, som skickas vidare bakåt i nätverket.

#### d) - Optimering


Slutligen uppdaterar vi kernel och bias med hjälp av lärhastigheten `LR`:

```
kernel = kernel − LR · kernelGradient
bias   = bias   − LR · biasGradient
```

**Uppdaterad kernel:**
```
0.17 0.36
0.50 0.70
```

**Uppdaterad bias:**
```
-0.1
```

---

### 3. Sammanfattning

Om du kan följa alla steg i detta dokument förstår du exakt hur ett CNN arbetar internt.
