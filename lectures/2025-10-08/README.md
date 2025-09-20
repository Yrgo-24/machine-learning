# 2025-10-08 - Lektion 17

## Dagordning
* Implementering av dense-lager i mjukvara (del I) (med programkoden från inlämningsuppgift 5):
    * Skapande av en skarp underklass `DenseLayer`, som ersätter befintlig stubbklass.
    * Tillägg av ett fåtal aktiveringsfunktioner.

## Mål med lektionen
* Ha skapat en skarp underklass som ärver det befintliga dense-lager-interfacet.
* Ha implementerat samtliga metoder **förutom** `feedforward`, `backpropagate` samt `optimize` - Dessa kommer att implementeras under nästa lektion.

## Instruktioner
* Använd er inlämnade programkod för inlämningsuppgift 5.
* Skapa en konkret implementation som ärver `ml::dense_layer::Interface`.
* Implementera samtliga metoder förutom de ovannämnda - returnera enbart `true` i dessa.

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Implementering av dense-lager i mjukvara (del II) - feedforward, backpropagation och optimering.