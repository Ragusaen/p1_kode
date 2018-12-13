Programmeringsstils regler
==========================

## Generelle retningslinjer
  - Alle navne på funktioner og variabler samt sprog i kommentarer osv. foregår på engelsk.
  - Programmet skrives efter ISO standarden ANSI C (C89) (compile derfor med flags -ansi og -pedantic)
  - De forskellige dele af programmet abstraheres væk i header filer med tilhørende .c filer.
  - Der benyttes altid "top-down programmering med trinvis forfinelse".

## Variabler
  - Variabelnavn er i "snake case", f.eks. `some_variable_name`.
  - Der vælges altid meningsfulde og hensigtsmæssige variabelnavne
  - Der må **ikke** benyttes globale variable. Hvis dette skulle friste, så overvej brug af pointers!
  - Dereferencing operatoren skrives ved pointers altid sammenhængende med variblenavnet. F.eks. `char *some_pointer`.

## Funktioner
  - Funktionsnavne er i "snake case", feks. `void some_function_name( int some_var )`.
  - Hvis funktionen er en underfunktion af en generel funktion, hvis det ikke er meningen man selv skal kalde den, så starter navnet med `_`. F.eks. `void _some_subfunction_name`.

## Structs
  - Structs typedef(ines) altid til ét keyword.
  - Navnet skrives med PascalCase, f.eks.
```c 
typedef struct SomeStruct {
  int some_int;
  char* some_string;
} SomeStruct;
```

## Header filer
  - Hver header-fil indeholder header guards. `#ifndef HEADERFILE_H`, `#define HEADERFILE_H` og `#endif`.
  - Headerfilens struktur er som følger:
    1. Inkludering af nødvendige bibliotekter.
    2. Inkludering af nødvendige header filer.
    3. Deklarering af structs.
    4. Overfunktionsprototyper.
    5. Subfunktionsprototyper.

Eksempel:
```c
#ifndef HEADERNAME_H
#define HEADERNAME_H

#include <stdlib.h>

#include "otherheader.h"

typedef struct StructName {
  int var_name;
  char* string_name;
} StructName;

void some_main_function( int parameter );

int _some_sub_function( int parameter, char* string_param );

#endif HEADERNAME_H
```


Dataset information
===================
TOTAL 474, TRAINING 332 (70.04%), TEST 142 (29.96%)


Confusion matrix vocabulary
===========================

| Abbr | Description | Abbr | Description | Abbr | Description | Abbr | Description |
| ---: | ----------- | ---: | ----------- | ---: | ----------- | ---: | ----------- |
| CP | Condition positive | TP | True positive | PPV | Positive predictive value, *Precision | TPR | True positive rate, **Recall |
| CN | Condition negative | FP | False positive | FDR | False discovery rate | FPR | False positive rate, Fall-out |
| PCP | Predicted condition positive | FN | False negative | FOR | False omission rate | FNR | False negative rate, Miss rate |
| PCN | Predicted condition negative | TN | True negative | NPV | Negative predictive value | TNR | True negative rate, Selectivity |

| Abbr | Description |
| ---: | ----------- |
| ACC | Accuracy |
| LR+ | Positive likelihood ratio |
| LR- | Negative likelihood ratio |
| DOR | Diagnostic odds ratio |
| MCC | Matthews correlation coefficient [-1, 1] |