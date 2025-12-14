# L23 - Konvolutionella neurala nätverk (del I)

## Dagordning
* Introduktion till konvolutionella neurala nätverk (CNN) för bildklassificering.

## Mål med lektionen
* Känna till varför vanliga neurala nätverk inte lämpar sig väl för bildklassificering.
* Känna till övergripande hur konvolutionella lager är uppbyggda.
* Kunna extrahera attribut ur bilder via kernels.

## Instruktioner
* Läs [bilaga A](#bilaga-a---varför-inte-mata-en-bild-direkt-till-ett-vanligt-neuralt-nätverk) för information om
varför vanliga neurala nätverk inte lämpar sig väl för bildklassificering.
* Läs [bilaga B](#bilaga-b---konvolutionella-neurala-nätverk-cnn) för information om konvolutionella neurala nätverk nedan.

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Träning av konvolutionella neurala nätverk för hand.
* Arbete med `Inlämningsuppgift 6 - Konvolutionellt neuralt nätverk.`

## Bilaga A - Varför inte mata en bild direkt till ett vanligt neuralt nätverk?

Ett vanligt neuralt nätverk (bestående enbart av dense-lager) kan teoretiskt sett användas för att bearbeta bilder,
men i praktiken lämpar sig detta sällan väl. Anledningarna till detta förklaras nedan. 
Dessutom beskrivs kortfattat varför konvolutionella neurala nätverk (CNN) lämpar sig bättre för bildklassificering.

---

### Förlust av rumslig struktur

En bild är tvådimensionell, där varje pixel har ett naturligt förhållande till sina grannpixlar.

Om bilden plattas ut till en endimensionell vektor innan den matas in i ett vanligt neuralt nätverk:

* försvinner information om vilka pixlar som ligger nära varandra,
* behandlas alla pixlar som oberoende av varandra,
* måste nätverket själv lära sig alla rumsliga relationer via vikter.

Detta gör inlärningen både svårare och mindre effektiv.

---

### Ett mycket stort antal parametrar (bias och vikter)

Anta att vi har ett nätverk som ska prediktera innehållet på en bild med storleken 1024 × 1024 pixlar.

Om bilden plattas ut och matas direkt in i ett dense-lager innebär detta att vårt nätverk behöver:

* väldigt många inputs - en input per pixel innebär `1024 * 1024 = 1 048 576` inputs,
* en vikt per pixel för varje nod i det första dolda lagret,
* minst lika många noder i efterföljande lager (oftast används fler),
* många dolda lager, då bilddatan är relativt komplex att prediktera på.

Redan ett enda dolt lager med exempelvis 1000 noder skulle då kräva över en miljard vikter.

Detta leder till att nätverket blir:
* mycket stort, 
* minneskrävande,
* långsamt att träna och prediktera med.

---

### Dålig generalisering

I ett vanligt neuralt nätverk behandlas varje pixel som en separat input.

Detta innebär att nätverket inte förstår att samma mönster kan dyka upp på olika platser i bilden,
och därför måste lära sig samma sak flera gånger.

Resultatet blir att nätverket 
* kräver mer träningsdata, 
* riskerar att lära sig träningsdatan utantill istället för att lära sig det som faktiskt är viktigt
* får svårare att känna igen sådant det har sett tidigare i nya bilder som ser lite annorlunda ut än träningsbilderna.  

Därmed riskerar nätverket att endast fungera bra på bilder som ser ut exakt som träningsdatan.

---

### Hur CNN löser dessa problem

Konvolutionella neurala nätverk är specifikt designade för bilddata och löser ovanstående problem genom att:

* bevara bildens rumsliga struktur,
* använda lokala filter (kernels),
* dela vikter över hela bilden,
* kraftigt minska antalet parametrar.

Detta gör CNN:
* mindre och snabbare,
* lättare att träna,
* bättre på att känna igen sådant den sett förut i nya bilder.

---

### Sammanfattning

Att mata en bild direkt in i ett vanligt neuralt nätverk innebär att:

* den rumsliga strukturen går förlorad,
* antalet parametrar blir orimligt stort,
* träning och prediktion blir långsamma,
* förmågan att känna igen sådant som setts tidigare i nya bilder försämras.

Konvolutionella neurala nätverk är därför nödvändiga för att effektivt kunna arbeta med bilddata.

## Bilaga B - Konvolutionella neurala nätverk (CNN)

### Översikt

Ett typiskt CNN består av ett flertal lager:

1. **Konvolutionella lager** (hädanefter kallade *Conv-lager*) används för att identifiera och extrahera lokala mönster och egenskaper i en bild.
2. **Pooling-lager** används för att sampla ned bilden, vilket minskar dess storlek samtidigt som de viktigaste mönstren bevaras.
3. **Flatten-lager** omvandlar (plattar) den tvådimensionella bilden till en endimensionell representation som kan användas av vanliga neurala nätverk.
4. **Dense-lager** används slutligen för att, baserat på dessa egenskaper, göra en prediktion av vad bilden föreställer.

De tre första lagren används för att extrahera attribut ur bilden. Dense-lagret används sedan för den slutliga prediktionen.

---

### Konvolutionella lager (Conv-lager)

#### Vad gör ett Conv-lager?

Ett konvolutionellt lager applicerar en *kernel* (ett litet filter med vikter) över inputbilden.
Samma kernel används över hela bilden, vilket innebär att:

* antalet parametrar hålls nere,
* samma mönster kan kännas igen oavsett position.

Varje kernel letar efter ett specifikt lokalt mönster, till exempel kanter, hörn eller linjer.

#### Viktiga egenskaper

* **Lokalt receptivt fält**: Varje nod ser bara en liten del av indatan.
* **Viktdelning**: Samma vikter används över hela bilden.
* **Aktiveringsfunktion (ReLU)**: Negativa svar ignoreras, positiva förs vidare.

Conv-lagrets output består av en matris och kallas *feature map*. Att extrahera attribut ur en 
bild kallas *feature extraction*.

Ett konvolutionellt lager fungerar i praktiken på samma sätt som ett vanligt neuralt lager,
men med två viktiga skillnader:
* beräkningarna görs lokalt på små delar av bilden,
* samma vikter (kerneln) används över hela bilden.

---

#### Feedforward i ett Conv-lager

Vid feedforward glider kerneln över inputbilden ett steg i taget, oftast med `stride = 1`
(kerneln flyttas ett pixelsteg i taget över bilden).

För varje position i bilden gäller att:
1. Ett lokalt fönster av bilden väljs ut (t.ex. 2×2 eller 3×3).
2. Värdena i fönstret multipliceras med motsvarande kernel-vikter.
3. Resultatet summeras tillsammans med ett bias-värde.
4. En aktiveringsfunktion (vanligtvis ReLU) appliceras.

Resultatet blir att varje position i outputen motsvarar hur starkt kerneln aktiverades på just den platsen i bilden.

---

#### Backpropagation i ett Conv-lager

Vid backpropagation sprids felet från outputen tillbaka genom lagret för att beräkna så kallade *gradienter*,
som indikerar hur mycket och åt vilket håll en vikt ska ändras för att aktuellt fel ska minska.

För varje position i outputen gäller att:
1. Gradient från nästa lager kombineras med derivatan av aktiveringsfunktionen (på samma sätt som 
"fel" tidigare har beräknats för dense-lager i denna kurs).
2. Denna gradient används för att vid optimering uppdatera kernelns gradienter, bias-gradienter samt sprida felet 
tillbaka till rätt positioner i inputen (för att optimiera eventuella föregående lager).

Eftersom samma kernel används över hela bilden:
- ackumuleras gradienter från alla positioner där kerneln applicerats,
- varje kernel-vikt påverkas av många olika delar av bilden.

Backpropagation i ett Conv-lager beräknar därmed hur mycket varje kernel-vikt har bidragit till det totala felet.

---

#### Optimering i ett Conv-lager

Efter att gradienterna har beräknats så uppdateras parametrarna i lagret.

Varje kernel-vikt justeras i motsatt riktning mot gradienten. Justeringens storlek avgörs av lärhastigheten.
Även bias-värdet uppdateras på samma sätt.

Optimeringen fungerar alltså identiskt med vanliga neurala nätverk, men med mycket färre parametrar, då
ett conv-lager bara består av en liten kernel med vikter samt ett bias-värde.

---

#### Sammanfattning

I ett Conv-lager:
- feedforward applicerar samma kernel lokalt över hela bilden,
- backpropagation samlar information från alla positioner där kerneln användes,
- optimering justerar kernelns vikter och bias så att de bättre fångar relevanta mönster nästa gång.

Trots att implementationen kan se mer avancerad ut än ett dense-lager bygger Conv-lager på exakt samma grundprinciper.

---

### Pooling-lager

#### Syfte

Pooling-lager samplar ned bilden, vilket gör att nätverket har mindre data att behandla.
Detta förenklar träning och prediktion samt gör nätverket mer robust mot små variationer i indata.
Pooling-lager har inga träningsbara parametrar.

Vid pooling delas indatan in i block, exempelvis 2×2 eller 3×3. Ur varje pooling-block extraheras enbart ett värde:

Två varianter av pooling är vanligt - *max pooling* samt *average pooling*:
* Vid max pooling extraheras det största värdet i pooling-blocket.
* Vid average pooling extraheras genomsnittvärdet av pooling-blocket.

Max pooling är absolut vanligast och kommer behandlas här.

#### Feedforward i maxpooling-lager

Vid feedforward extraheras det största värdet i respektive pooling-block.
Detta innebär att de mest framträdande attributen i respektive region förs vidare till bekostnad
av att detaljer förloras. Detta fungerar som en bild som samplas ned på en dator - bilden ser normalt
samma ut som tidigare efter nedsamplingen, men gör man bilden större igen ser man att många av detaljerna har försvunnit.

#### Backpropagation i maxpooling-lager

Vid backpropagation skickas gradienter endast tillbaka till de positioner som hade maxvärdet under feedforward. Övriga positioner får gradienten 0.

---

### Flatten-lager

Flatten-lager "plattar" extraherad data från 2D till 1D för att vi ska kunna mata datan till dense-lager
för prediktion.

#### Varför behövs flatten-lager?

Efter konvolution och pooling består datan fortfarande av matriser, dvs. datan är fortfarande tvådimensionell.
Dense-lager kräver däremot som bekant endimensionell data.

Flatten-lagret:
* ändrar endast datans form (2D => 1D),
* utför inga beräkningar,
* har inga vikter.

Vid backpropagation gör flatten-lagret exakt motsatt operation.

---

### Dense-lager i CNN

Dense-lagret fungerar på samma sätt som i ett vanligt neuralt nätverk.
Skillnaden är att inputen nu redan består av extraherade och komprimerade features, i stället för
input-data som har valts ut för att träna modellen.

Dense-lagret används för att:
* väga samman features,
* göra klassificering eller regression.

---

### Mentalt flöde – hur CNN:et "tänker"

Det mentala flödet är ett sätt att förstå vad varje lager försöker avgöra, snarare än hur det räknar.

Anta att vi har följande CNN:
* Ett Conv-lager, som bearbetar 4×4-bilder med en 2×2-kernel.
* Ett 2×2 maxpooling-lager, som samplar ned bilden till 2×2.
* Ett flatten-lager, som plattar bilden till en dimension (2×2 till 1×4).
* Ett dense-lager bestående av en nod och fyra vikter, som predikterar innehållet på inputbilden.

---

#### 1. Input – "Rå data"

"Här finns bara pixlar. Ingen tolkning ännu."

Nätverket ser endast siffror i en matris. Ingen struktur är ännu identifierad.

---

#### 2. Conv-lager – "Finns lokala mönster?"

Conv-lagret tittar på små områden i taget.

Mentalt ställer det frågor som:
* "Finns ett intressant mönster i detta lilla fönster?"
* "Ger detta område en stark aktivering?"

Aktiveringsfunktionen ReLU förstärker detta tänk:
* negativa svar ignoreras,
* positiva svar lyfts fram.

Conv-lagret svarar alltså på:
> *Var i bilden finns ett visst lokalt mönster?*

---

### Maxpooling-lager – "Finns mönstret här, ungefär?"

Maxpooling-lagret bryr sig mindre om exakt position.

Mentalt:
* "Finns mönstret någonstans i detta område?"

Om svaret är ja, behåll signalen.
Små förskjutningar i input påverkar då inte resultatet lika mycket.

---

### Flatten – "Förbered för beslut"

Flatten gör inget intelligent val.
Det säger bara:
> "Nu räcker det med struktur – dags att fatta beslut."

Data görs om till en vektor som kan matas in i ett vanligt neuralt nätverk.

---

### Dense – "Vad föreställer bilden?"

Dense-lagret väger samman de extraherade features och ger en slutlig prediktion.

Här används exakt samma principer som i vanliga neurala nätverk:
* viktsummor,
* aktivering,
* felberäkning.

---

### Backpropagation – samma flöde baklänges

När nätverket gissar fel:

* Dense-lagret justerar hur features viktas.
* Flatten-lagret formar om gradienten.
* Maxpooling-lagret skickar gradienten till rätt position.
* Conv-lagret justerar vikter för de mönster som faktiskt bidrog till prediktionen.

Backpropagation följer alltså samma princip som feedforward, fast baklänges.

---

### Sammanfattning

* Conv-lager letar efter lokala mönster.
* Maxpooling-lagret behåller de mest framträdande attributen och förkastar övriga.
* Flatten-lagret omvandlar extraherad data till en dimension.
* Dense-lagret gör den slutliga tolkningen.

Ett CNN kan ses som ett vanligt neuralt nätverk med intelligenta förbehandlingslager för rumslig data.
