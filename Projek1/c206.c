
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/
/*
**          Vypracoval : René Bolf (xbolfr00@stud.fit.vutbr.cz)
**/
#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->First = NULL; //prvý prvok zoznamu nastavíme na null
    L->Act = NULL; // aktívny prvok zoznamu nastavíme na null
    L->Last = NULL; // posledný prvok zoznamu nastavíme na null
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/
    while(L->First != NULL)  //bude sa to opakovať dokym sa nezrušia všetky prvky zoznamu
    {
        tDLElemPtr temp = L->First;
        L->First = L->First->rptr; // do ukazatela na prvý prvok uložime pravý ukazatel prvku, ktorý uvoľňujeme
        free(temp);
    }
    L->Act = NULL; //aktivny prvok a posledný prvok nastavíme na null aby sa zoznam nachadzal v takom stave ako pred jeho prvým použitím
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr temp = malloc(sizeof(struct tDLElem));
    if (temp != NULL)
    {
        temp->data = val;  //vlozime data do val
        temp->rptr = L->First; // prvok napravo ukazuje na prvy prvok
        temp->lptr = NULL; // lavy prvok ukazuje na NULL
        if(L->First != NULL)
            L->First->lptr = temp; //prvy bude ukazovat do lava na nový prvok
        else L->Last = temp; //posledny prvok je co sme my vlozili
        L->First = temp;   //prvy prvok je co sme my vlozili
    }
    else DLError();
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
tDLElemPtr temp = malloc(sizeof(struct tDLElem));
    if (temp != NULL)
    {
        temp->data = val;
        temp->lptr = L->Last;
        temp->rptr = NULL;
        if(L->Last != NULL)
            L->Last->rptr = temp;
        else L->First = temp;
        L->Last = temp;
    }
    else DLError();
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;  //nastavenie aktivity na prvý prvok
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;  //nastavenie aktivity na posledný prvok
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if(L->First == NULL) //ak je zoznam prázdny zavola sa funkcia DLError()
        DLError();
    else *val = L->First->data; // prostredníctvom val vrátime hodnotu prvého prvku zoznamu
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if(L->First == NULL)
        DLError();
    else *val = L->Last->data; // prostredníctvom val vrátime hodnotu posledného prvku
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L->First != NULL) //kontrola či zoznam nieje prazdny
        {
            if (L->Act == L->First)
                L->Act = NULL;  //zrusenie aktivity
            tDLElemPtr temp = L->First;
            if (L->First == L->Last){ // zoznam mal jediný prvok a následne sa zruší
                L->First = NULL;
                L->Last = NULL;
            }
            else{
                L->First = L->First->rptr;  // do ukazatela na prvý prvok uložime pravý ukazatel prvku, ktorý uvoľňujeme a tak zrušíme prvý prvok zoznamu
                L->First->lptr = NULL; //ľavý ukazatel prvého prvku ukazuje na NULL
            }
            free(temp);
        }
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
if (L->First != NULL) //kontrola či zoznam nieje prazdny
    {
        if (L->Act == L->Last)
            L->Act = NULL;  //zrusenie aktivity
        tDLElemPtr temp = L->Last;
        if (L->First == L->Last){
            L->First = NULL;
            L->Last = NULL;
        }
        else{
            L->Last = L->Last->lptr;  //do ukazatela na posledný prvok uložíme ľavý ukazatel prvku, ktorý uvoľňujeme a tak zrušíme posledný prvok zoznamu
            L->Last->rptr = NULL; //pravý ukazateľ posledného prvku ukazuje na NULL
        }
        free(temp);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    if(L->Act != NULL) //kontrola či je aktivny
    {
        if(L->Act->rptr != NULL){ //kontrola či aktivny prvok nieje posledným prvok zoznamu a či je čo rušiť
        tDLElemPtr temp = L->Act->rptr; //ukazatel na prvok ktory chceme rušit
        L->Act->rptr = temp->rptr;
        if(temp == L->Last) //ak je rušený prvok posledný
            L->Last = L->Act; // posledný bude aktivny
        else
            temp->rptr->lptr = L->Act; //ľavý ukazateľ prvku, ktorý leží pravo od rušeného prvku odkážeme na aktívny prvok
        free(temp);
        }
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if(L->Act != NULL) //kontrola či je aktivny
    {
        if(L->Act->lptr != NULL){ // kontrola či aktívny prvok nieje prvým prvkom zoznamu
        tDLElemPtr temp = L->Act->lptr; //ukazatel na prvok ktory chceme rušit
        L->Act->lptr = temp->lptr;
        if(temp == L->First) //ak je rušený prvok prvý
            L->First = L->Act; // aktívny bude prvý
        else
            temp->lptr->rptr = L->Act; // pravý ukazateľ prvku, ktorý leží v ľavo od rušeného prvku odkážeme na aktívny prvok
        free(temp);
        }
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if(L->Act != NULL)
    {
        tDLElemPtr temp = malloc(sizeof(struct tDLElem));
        if (temp != NULL)
        {
            temp->data = val;
            temp->rptr = L->Act->rptr; //do pravého ukazateľa nového prvku vložíme pravý ukazateľ aktívneho prvku
            temp->lptr = L->Act; //do ľavého ukazateľa uložíme ukazateľ na aktívny prvok
            L->Act->rptr = temp; // do pravého ukazateľa aktívneho prvku uložime novy
            if(L->Act == L->Last) // ak je aktívny posledný prvok tak sa vloži za neho
                L->Last = temp;
            else
                temp->rptr->lptr = temp;
        }
        else DLError();
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if(L->Act != NULL)
    {
        tDLElemPtr temp = malloc(sizeof(struct tDLElem));
        if (temp != NULL)
        {
            temp->data = val;
            temp->lptr = L->Act->lptr; //do ľavého ukazateľa nového prvku vložíme ľavý ukazateľ aktívneho prvku
            temp->rptr = L->Act; //do pravého vložíme ukazateľ na aktívny prvok
            L->Act->lptr = temp; // do ľavého ukazateľa aktívneho prvku vložime nový
            if(L->Act == L->First) //ak je aktivny prvok prvý tak sa vloži novy prvok pred aktivny
                L->First = temp;
            else
                temp->lptr->rptr = temp;
        }
    else DLError();
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if(L->Act == NULL)
        DLError();
    else *val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if(L->Act != NULL)
        L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    if(L->Act != NULL)
    {
        if(L->Act != L->Last) //ak nieje aktivny prvok posledný
            L->Act = L->Act->rptr; //aktivita sa posunie do prava
        else  L->Act = NULL; //ak je posledný tak sa aktivita stratí
    }
}

void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    if(L->Act != NULL)
    {
        if(L->Act != L->First)  //ak nieje aktivny prvý
            L->Act = L->Act->lptr; //tak sa aktivita posunie do ľava
        else L->Act = NULL; // ak je tak sa aktivita stratí
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return (L->Act != NULL ? 1 : 0);
}

/* Konec c206.c*/
