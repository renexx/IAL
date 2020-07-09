
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/
/**
 *              Vypracoval : René Bolf (xbolfr00@stud.fit.vutbr.cz)
 */
#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
    int retval = 1;
    int keylen = strlen(key);
    for ( int i=0; i<keylen; i++ )
        retval += key[i];
    return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
    for (int i = 0; i < HTSIZE; i++) //všetky prvky poľa nastavíme na hodnotu NULL
        (*ptrht)[i] = NULL;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
    if(ptrht != NULL) // kontrola či tabulka nieje prazdna
    {
        tHTItem *temp = (*ptrht)[hashCode(key)];
        while(temp != NULL)
        {
            if(strcmp(temp->key, key)) // ak sa kluc nezhoduje tak sa posuva na dalsie 
                temp = temp->ptrnext;
            else
                return temp; //ak sa zhoduje vrati hodnotu kde sa zhoduje
        }
        return NULL;    
    }
    else return NULL;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
    if(ptrht != NULL)
    {
        tHTItem *temp = htSearch(ptrht, key); // pomocou funkcie search zistime ci v tabulke existuje polozka s rovnakym klucom
        if(temp != NULL) //ak existuje
            temp->data = data; //aktualizujeme jeho datovu časť
        else // ak neexistuje 
        {
            tHTItem *new = malloc(sizeof(tHTItem)); // naalokujeme si miesto pre novu polozku
            if(new != NULL) //ak sa alokacia podari
            {
                new->key = key; // priradi sa kluc
                new->data = data;
                new->ptrnext = (*ptrht)[hashCode(key)];
                (*ptrht)[hashCode(key)] = new;
            }
            else return;
        }    
    }
}
/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/
tData* htRead ( tHTable* ptrht, tKey key ) {
    if(ptrht != NULL)
    {
        tHTItem *temp = htSearch(ptrht, key); // pomocou funkcie vyhľadáme položku
        if(temp != NULL)
            return &temp->data; //našla sa položka vrátime ju ako ukazateľ na položku
        else return NULL; //položka sa nenašla vrátime funkčú hodnotu NULL
    }
    else return NULL;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
    if(ptrht != NULL)
    {
        tHTItem *temp = (*ptrht)[hashCode(key)];  //do temp sa mi ulozi ukazatel kde ukazuje hashcode key
        tHTItem *prevtemp = NULL;
        if(temp != NULL)
        {
            while(temp != NULL)
            {
                if(strcmp(temp->key, key)) //ak sa nezhoduje tak sa posunie na dalsi
                {
                    prevtemp = temp;
                    temp = temp->ptrnext;
                }
                else //ak sa zhoduje
                {
                    if(prevtemp != NULL) // a ten predtym nieje NULL
                        prevtemp->ptrnext = temp->ptrnext; // tak sa previazu ukazatele
                    else 
                        (*ptrht)[hashCode(key)] = temp->ptrnext; //ak je null tak sa do prveho ukazatela ulozi polozka
                    free(temp); //rusi sa 
                    temp = NULL;
                }   
            }
        }
        else return;
    }
    else return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
    if(ptrht != NULL)
    {
        for(int i = 0; i < HTSIZE; i++) // budeme prechádzať cez každý prvok v tabulke
        {
            tHTItem *temp = (*ptrht)[i];
            while(temp != NULL)
            {
                tHTItem* next = temp->ptrnext; //budeme sa posuvať
                free(temp); //a uvoľňovať
                temp = next;
            }
            (*ptrht)[i] = NULL; //uvedieme tabulku do počiatočného stavu
        }
    }
}

