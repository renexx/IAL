
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/
/**
 *                  Vypracoval : René Bolf (xbolfr00@stud.fit.vutbr.cz)
 */
#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    while(L->First != NULL)
    {
        tElemPtr temp = L->First; //prvy prvok uložime do pomocnej premennej aby sme ho mohli uvolnit
        L->First = L->First->ptr; // do prvého prvku uložime prvok druhý
        free(temp); // uvolníme pomocnu premennú temp čiže prvý prvok
    }
    L->Act = NULL; // aktívny prvok nastavíme na hodnotu NULL
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    tElemPtr temp = malloc(sizeof(struct tElem));
    if (temp != NULL) //ak nenastala chyba
    {
        temp->data = val;  //vlozime data do val
        temp->ptr = L->First; //novy prvok ukazuje tam kam ukazuje začiatok
        L->First = temp;    //začiatok ukazuje na nový prvok
    }
    else Error();
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
    L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
    if(L->First == NULL)  //zistime či je seznam prazdny ak ano zavola sa funkcia Error
        Error();
    else *val = L->First->data;  //ak nieje prazdny vložim data a vratim hodnotu prvého prvku zoznamu
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L->First != NULL) //kontrola či zoznam nieje prazdny
    {
        if (L->Act == L->First) //ak je rušený prvok aktívny
            L->Act = NULL;  //zrušíme aktivitu
        tElemPtr temp = L->First;
        L->First = L->First->ptr;  //zrusenie prveho prvku zoznamu
        free(temp);
    }
}

void PostDelete (tList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
    if(L->Act != NULL) //kontrola či je aktivny
    {
        if(L->Act->ptr != NULL)
        { //kontrola či je aktivny posledný prvok
            tElemPtr temp = L->Act->ptr; //ukazatel na prvok ktory chceme rušit
            L->Act->ptr = temp->ptr;
            free(temp);
        }
    }
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
    if(L->Act != NULL)
    {
        tElemPtr temp = malloc(sizeof(struct tElem));
        if (temp != NULL)
        {
            temp->data = val; // vlozime data do val
            temp->ptr = L->Act->ptr; // novy prvok ukazuje tam kam ukazuje aktivny prvok
            L->Act->ptr= temp; // aktivny ukazuje na nový prvok
        }
        else Error();
    }
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    if(L->Act == NULL) // ak je zoznam aktívny zavolá sa funkcia error()
        Error();
    else *val = L->Act->data; // pomocou val vratime hodnotu aktívneho prvku 
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
    if(L->Act != NULL)
        L->Act->data = val;
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
    if(L->Act != NULL) // kontrola či je zoznam aktivny
    {
        if(L->Act->ptr != NULL)
            L->Act= L->Act->ptr; //ak je aktivny tak sa aktivita posunie na nasledujúci prvok
        else L->Act = NULL; // aktivny zoznam sa stane neaktivnym
    }
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return.
**/
    return (L->Act != NULL ? 1 : 0);  //1 = je aktivny (TRUE) 0=aktivny nieje (FALSE)
}

/* Konec c201.c */
