# Obserwacje
### Zmienne
Zmienne trzymane w mapie 

``` c++
struct Zmienna{
    std::string name;
    var_type type;
    int scope_lvl;
}

enum var_type{
    natywna,
    indirect,
}
```

Gdy wchodzimy głębiej w blok np. przez procedurę. Zwiększamy globalny scope_lvl. Po wyjściu z procedury zmniejszamy go i usuwamy z listy wszystkie zmienne z poprzedniego poziomu. 
Przy tworzeniu zmiennej jej nazwa to będzie _identifier(ProcedureName)_
Procedure name jest opcjonalne. Sprawdzany będzie 

###
### Procedury
Każda procedura ma rejestr powrotu (do przemyślenia)

Zmienne deklarowane w headerze procedury będę przechowywane jako adresy. Operacje w procedurze mogą być wykonywane na dwa sposoby:
- direct, zwykłe operacje dla zmiennych lokalnych  
- indirect, dla operacji na zmiennych headerowych
  

### Stany

Mamy kilka stanów programu:
- program
- procedure
- if
- else
- while
- until
  
# Pytania

1. Czy dozwolona jest rekurencja nie wprost? Procedura A wywołuje procedure B która wywołuje A


# Pomysły na optymalizacje
1. kończenie odejmowania gdy przekraczamy liczbę ujemną