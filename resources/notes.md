# Obserwacje

### ETAPY KOMPILACJI
1. Budowa struktury AST
2. Indeksowanie zmiennych
3. Indeksowanie Procedur
4. (Obliczanie skoków)
5. translacja na kod maszynowy


### BLOKI

Compiler oparty na translacji na basic_blocks z użeciem AST

```c++
#define GENERIC_BLOCK = 1
#define MAIN_BLOCK = 10
#define 
#define PROCEDURE_BLOCK = 2
#define PROC_HEAD_BLOCK = 2
#define VAR_BLOCK = 2

```

### Zmienne
Zmienne trzymane w mapie 

``` c++
struct Zmienna{
    std::string _proc_name;
    var_type type;
    int scope_lvl;
}

enum var_type{
    natywna,
    indirect,
}
```
Przy tworzeniu zmiennej jej nazwa to będzie _identifier(ProcedureName)_
W bloku PROGRAM i PROCEDURE trzymany będzie słownik
###
### Procedury
Każda procedura ma rejestr powrotu (do przemyślenia)

Zmienne deklarowane w headerze procedury będę przechowywane jako adresy. Operacje w procedurze mogą być wykonywane na dwa sposoby:
- direct, zwykłe operacje dla zmiennych lokalnych  
- indirect, dla operacji na zmiennych headerowych
  

### Stany

Mamy kilka stanów programu:
- program
- Procedure
- if
- else
- while
- until
  
# Pytania

1. Czy dozwolona jest rekurencja nie wprost? Procedura A wywołuje Procedure B która wywołuje A


# Pomysły na optymalizacje
1. kończenie odejmowania gdy przekraczamy liczbę ujemną