#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ID
{
    char oznacenie;
    int cislovanie;
    char druh;
} ID;

typedef struct POZ
{
    double latitude;
    double longitude;
} POZ;

typedef struct Zaznam
{
    ID id;
    POZ poz;
    char typ[3];
    double hod;
    char cas[5];
    int datum;
    char datumCas[20];
    struct Zaznam *dalsi;
} Zaznam;

void uvolnit_zaznamy(Zaznam **zaz)
{
    Zaznam *akt;
    // Postupne uvolňujeme pamäť pre každý záznam v zozname
    while (*zaz != NULL)
    {
        akt = *zaz;
        *zaz = (*zaz)->dalsi;
        free(akt);
    }
}

void nacitanie(FILE **dataloger, Zaznam **zaz, int *pocet_zaznamov)
{
    // Otvárame súbor pre čítanie
    *dataloger = fopen("dataloger_V2.txt", "r");

    // Kontrola, či sa súbor otvoril správne
    if (*dataloger == NULL)
    {
        printf("Zaznamy neboli nacitane!\n");
        return;
    }

    Zaznam *akt, *posledny = NULL;
    *pocet_zaznamov = 0;

    // Postupne načítavame záznamy zo súboru, pokiaľ nedôjdeme na koniec súboru
    while (!feof(*dataloger))
    {
        // Alokujeme pamäť pre nový záznam
        akt = (Zaznam *)malloc(sizeof(Zaznam));
        char ID_norm[6];
        char poz_norm[17];

        // Načítame údaje zo súboru a kontrolujeme, či sme správne načítali všetky hodnoty
        if (fscanf(*dataloger, "%5s %16s %2s %lf %4s %d",
                   ID_norm, poz_norm, akt->typ, &akt->hod, akt->cas, &akt->datum) == 6)
        {
            // Konvertujeme dátum a čas do jedného reťazca
            sprintf(akt->datumCas, "%d%s", akt->datum, akt->cas);

            // Nastavíme hodnoty identifikátora záznamu
            akt->id.oznacenie = ID_norm[0];
            akt->id.druh = ID_norm[4];

            // Extrahujeme stredných päť číslic zo značky a konvertujeme ich na celé číslo
            char middleThree[3];
            for (int i = 1; i < 4; i++)
            {
                middleThree[i - 1] = ID_norm[i];
            }
            akt->id.cislovanie = atoi(middleThree);

            // Extrahujeme súradnice latitude a longitude zo reťazca
            char latitude[7], longitude[7];

            for (int i = 1; i < 8; i++)
            {
                latitude[i - 1] = poz_norm[i];
            }
            for (int i = 9; i < 16; i++)
            {
                longitude[i - 9] = poz_norm[i];
            }

            // Konvertujeme reťazce na čísla a prípadne meníme znamienko podľa orientácie
            akt->poz.latitude = atof(latitude);
            if (poz_norm[0] == '-')
            {
                akt->poz.latitude = -1 * akt->poz.latitude;
            }

            akt->poz.longitude = atof(longitude);
            if (poz_norm[8] == '-')
            {
                akt->poz.longitude = -1 * akt->poz.longitude;
            }

            // Nastavíme dátový pointer pre ďalší záznam v zozname
            akt->dalsi = NULL;
            if (posledny != NULL)
            {
                posledny->dalsi = akt;
            }
            else
            {
                // Ak ide o prvý záznam, nastavíme ho ako hlavný pointer zoznamu
                *zaz = akt;
            }

            // Posun na posledný načítaný záznam
            posledny = akt;

            // Zvýšime počet načítaných záznamov
            (*pocet_zaznamov)++;
        }
        else
        {
            // Ak sa nenačítal očakávaný počet hodnôt, uvoľníme pamäť pre aktuálny záznam a ukončíme načítavanie
            free(akt);
            break;
        }
    }

    // Zatvoríme súbor po dokončení načítavania
    fclose(*dataloger);

    // Vypíšeme počet načítaných záznamov
    printf("Nacitalo sa %d zaznamov\n", *pocet_zaznamov);
}


// Funkcia pre výpis všetkých záznamov v zozname
void vypis_zaznamov(Zaznam *zaz)
{
    Zaznam *akt = zaz;
    int index = 1;

    // Postupne vypisujeme informácie o každom zázname
    while (akt != NULL)
    {
        printf("%d:\n", index);
        printf("ID: %c%d%c\t%s\t%2lf\n", akt->id.oznacenie, akt->id.cislovanie, akt->id.druh, akt->typ, akt->hod);

        // Kontrola znamienka pre súradnicu latitude
        if (akt->poz.latitude > 0)
        {
            printf("Poz: +%.4lf\t", akt->poz.latitude);
        }
        else
        {
            printf("Poz: -%.4lf\t", akt->poz.latitude);
        }

        // Kontrola znamienka pre súradnicu longitude
        if (akt->poz.longitude > 0)
        {
            printf("+%.4lf\n", akt->poz.longitude);
        }
        else
        {
            printf("Lon: -%.4lf\n", akt->poz.longitude);
        }

        printf("DaC: %d\t%s\n", akt->datum, akt->cas);
        akt = akt->dalsi;
        index++;
    }
}

// Funkcia pre pridanie nového záznamu do zoznamu
void novy_zaznam(Zaznam **zaz, int *pocet_zaznamov)
{
    int miesto;
    
    // Načítame pozíciu, na ktorej chceme pridať nový záznam
    scanf("%d", &miesto);

    // Kontrola, či pozícia nie je väčšia ako počet záznamov, ak áno, pridáme na koniec
    if (miesto > (*pocet_zaznamov))
    {
        miesto = (*pocet_zaznamov) + 1;
    }

    int index = 1;
    int prov = 0;
    Zaznam *novyZaznam = (Zaznam *)malloc(sizeof(Zaznam));

    char ID_norm[6];
    char poz_norm[17];

    // Načítame údaje pre nový záznam zo vstupu
    scanf("%5s %16s %2s %lf %4s %d", ID_norm, poz_norm, novyZaznam->typ, &novyZaznam->hod, novyZaznam->cas, &novyZaznam->datum);

    // Nastavíme hodnoty identifikátora záznamu
    novyZaznam->id.oznacenie = ID_norm[0];
    novyZaznam->id.druh = ID_norm[4];
    char middleThree[3];

    // Extrahujeme stredných päť číslic zo značky a konvertujeme ich na celé číslo
    for (int i = 1; i < 4; i++)
    {
        middleThree[i - 1] = ID_norm[i];
    }
    novyZaznam->id.cislovanie = atoi(middleThree);

    char latitude[7], longitude[7];

    // Extrahujeme súradnice latitude a longitude zo vstupu
    for (int i = 1; i < 8; i++)
    {
        latitude[i - 1] = poz_norm[i];
    }
    for (int i = 9; i < 16; i++)
    {
        longitude[i - 9] = poz_norm[i];
    }

    // Konvertujeme reťazce na čísla a prípadne meníme znamienko podľa orientácie
    novyZaznam->poz.latitude = atof(latitude);
    if (poz_norm[0] == '-')
    {
        novyZaznam->poz.latitude = -1 * novyZaznam->poz.latitude;
    }

    novyZaznam->poz.longitude = atof(longitude);
    if (poz_norm[8] == '-')
    {
        novyZaznam->poz.longitude = -1 * novyZaznam->poz.longitude;
    }

    sprintf(novyZaznam->datumCas, "%d%s", novyZaznam->datum, novyZaznam->cas);

    novyZaznam->dalsi = NULL;

    // Kontrola, či pridávame na začiatok zoznamu
    if (miesto == 1)
    {
        novyZaznam->dalsi = *zaz;
        *zaz = novyZaznam;
    }
    else
    {
        // Prechádzame zoznamom a vložíme nový záznam na požadované miesto
        Zaznam *akt = (*zaz);
        while (akt != NULL)
        {
            if (index == miesto - 1)
            {
                prov = 1;
                novyZaznam->dalsi = akt->dalsi;
                akt->dalsi = novyZaznam;
                break;
            }
            akt = akt->dalsi;
            index++;
        }
    }

    // Zvýšime počet záznamov a vypíšeme informáciu
    (*pocet_zaznamov)++;
    printf("Nový záznam bol vytvorený\n");
}


// Funkcia pre vymazanie záznamu podľa identifikátora
void vymaz_zaznamov(Zaznam **zaz, int *pocet_zaznamov)
{
    char ID_norm[6];
    
    // Načítame identifikátor pre vymazanie zo vstupu
    scanf("%5s", ID_norm);

    char oznacenie_vym = ID_norm[0];
    char druh_vym = ID_norm[4];
    char middleThree[3];

    // Extrahujeme stredných päť číslic zo značky a konvertujeme ich na celé číslo
    for (int i = 1; i < 4; i++)
    {
        middleThree[i - 1] = ID_norm[i];
    }
    int cislovanie_vym = atoi(middleThree);

    // Prechádzame zoznamom a hľadáme záznam na vymazanie
    Zaznam *akt = *zaz;
    Zaznam *pred = NULL;

    while (akt != NULL)
    {
        if (akt->id.cislovanie == cislovanie_vym && akt->id.oznacenie == oznacenie_vym && akt->id.druh == druh_vym)
        {
            // Ak sme na začiatku zoznamu, aktualizujeme hlavný pointer
            if (pred == NULL)
            {
                *zaz = akt->dalsi;
            }
            else
            {
                // Inak aktualizujeme pointer predošlého záznamu
                pred->dalsi = akt->dalsi;
            }

            (*pocet_zaznamov)--;

            // Uvoľníme pamäť pre vymazaný záznam
            Zaznam *pam = akt;
            akt = akt->dalsi;
            free(pam);

            printf("Zaznam pre ID: %c%d%c bol vymazany.\n", oznacenie_vym, cislovanie_vym, druh_vym);

            // Pokračujeme na ďalší záznam
            continue;
        }

        // Posunieme pointery na ďalší záznam
        pred = akt;
        akt = akt->dalsi;
    }
}

// Funkcia pre spojenie dvoch zoznamov záznamov do jedného usporiadaného zoznamu
Zaznam *spojenie(Zaznam *prvy, Zaznam *druhy)
{
    // Ak je prvý zoznam prázdny, vrátime druhý
    if (prvy == NULL)
        return druhy;
    
    // Ak je druhý zoznam prázdny, vrátime prvý
    if (druhy == NULL)
        return prvy;

    // Porovnáme dva záznamy podľa dátumu a času
    if (strcmp(prvy->datumCas, druhy->datumCas) > 0)
    {
        // Ak je druhý záznam menší, spojíme ho s usporiadaným zvyškom druhých záznamov
        druhy->dalsi = spojenie(prvy, druhy->dalsi);
        return druhy;
    }
    else
    {
        // Ak je prvý záznam menší alebo rovnaký, spojíme ho s usporiadaným zvyškom prvých záznamov
        prvy->dalsi = spojenie(prvy->dalsi, druhy);
        return prvy;
    }
}

// Funkcia na oddelenie zoznamu na polovicu (uprostred) pre usporiadanie
Zaznam *oddelenie(Zaznam *akt)
{
    // Pomocné pointery pre rýchle a pomalé prechádzanie zoznamom
    Zaznam *posl = NULL;
    Zaznam *fast = akt;
    Zaznam *slow = akt;

    // Rýchly pointer sa posúva o dve pozície, pomalý o jednu, kým rýchly nedosiahne koniec zoznamu
    while (fast != NULL && fast->dalsi != NULL)
    {
        posl = slow;
        fast = fast->dalsi->dalsi;
        slow = slow->dalsi;
    }

    // Ak sme nenašli prostredný bod, zoznam už je prázdny alebo obsahuje iba jeden prvok
    if (posl != NULL)
    {
        posl->dalsi = NULL;
    }

    // Vraciame pointer na začiatok druhej polovice zoznamu
    return slow;
}

// Funkcia na usporiadanie záznamov v zozname (merge sort)
Zaznam *usporiadanie(Zaznam *akt)
{
    // Ak je zoznam prázdny alebo obsahuje iba jeden prvok, vrátime ho
    if (akt == NULL || akt->dalsi == NULL)
    {
        return akt;
    }

    // Oddelíme zoznam na polovicu
    Zaznam *druhy = oddelenie(akt);

    // Rekurzívne usporiadame oba polovice zoznamu
    akt = usporiadanie(akt);
    druhy = usporiadanie(druhy);

    // Spojíme dva usporiadané zoznamy do jedného
    return spojenie(akt, druhy);
}

// Funkcia pre usporiadanie záznamov v zozname
void usporiadanie_zaznamov(Zaznam **zaz, int *pocet_zaznamov)
{
    // Kontrola, či existuje zoznam a obsahuje aspoň jeden záznam
    if (zaz != NULL && *zaz != NULL)
    {
        // Aplikujeme algoritmus usporiadania (merge sort) na zoznam
        *zaz = usporiadanie(*zaz);
        
        // Vypíšeme informáciu o úspešnom usporiadaní
        printf("Spajany zoznam bol usporiadany.\n");
    }
}


// Funkcia pre výmenu hodnôt dvoch premenných
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Funkcia pre prechod na záznam na k-tej pozícii v zozname
Zaznam *probeh(Zaznam *akt, int k, int pocet_zaznamov)
{
    // Prejde zoznam a vráti ukazovateľ na záznam na k-tej pozícii
    for (int i = 1; i <= pocet_zaznamov; i++)
    {
        if (i == k)
        {
            return akt;
        }
        
        akt = akt->dalsi;
    }
    return NULL;
}

// Funkcia pre výmenu dvoch záznamov na pozíciách k1 a k2 v zozname
void zamena(Zaznam **zaz, Zaznam *zap, Zaznam *zap_old, int k, int pocet_zaznamov)
{
    Zaznam *akt = *zaz;
    Zaznam **pred = NULL;

    // Ak ide o výmenu na prvom mieste, aktualizujeme hlavný pointer
    if (k == 1)
    {
        *zaz = zap;
        (*zaz)->dalsi = zap_old;
        return;
    }

    // Prejdeme na pozíciu pred k-tej
    for (int i = 1; i < k - 1 && akt != NULL; i++)
    {
        akt = akt->dalsi;
    }

    // Ak sme prešli cez celý zoznam alebo sme na jeho konci, vrátime sa
    if (akt == NULL)
    {
        return;
    }

    // Nastavíme ukazovateľ na nasledujúci záznam pre k-1-tej pozícii
    akt->dalsi = zap;
    akt->dalsi->dalsi = zap_old;
}

// Funkcia pre zmenu záznamov na pozíciách c1 a c2 v zozname
void zmena_zaznamov(Zaznam **zaz, int pocet_zaznamov)
{
    // Načítame pozície c1 a c2 zo vstupu
    int c1, c2;
    scanf("%d %d", &c1, &c2);

    if (c1 <= 0 || c2 <= 0 || c1 > pocet_zaznamov || c2 > pocet_zaznamov || c1 == c2)
    {
        return;
    }

    if (c1 > c2)
    {
        swap(&c1, &c2);
    }

    // Získame ukazovateľe na záznamy na pozíciách c1 a c2
    Zaznam *zap1 = probeh(*zaz, c1, pocet_zaznamov);
    Zaznam *zap2 = probeh(*zaz, c2, pocet_zaznamov);

    // Prehodíme záznamy, ak susedia
    if (c1 == c2 - 1)
    {
        // Ak susedia na pozíciách (c1, c2-1), potrebujeme špeciálnu manipuláciu

        if (c1 == 1)
        {
            // Ak c1 je na začiatku zoznamu
            Zaznam *temp;
            zap1->dalsi = zap2->dalsi;
            temp = NULL;
            temp = zap2;
            temp->dalsi = zap1;
            *zaz = temp;
            return;
        }

        // Ak c1 nie je na začiatku zoznamu
        Zaznam *temp;
        zap1->dalsi = zap2->dalsi;
        temp = NULL;
        temp = zap2;
        temp->dalsi = zap1;

        // Nájdeme predchádzajúci prvok pred pozíciou c1
        Zaznam *akt = *zaz;
        for (int i = 1; i < c1 - 1 && akt != NULL; i++)
        {
            akt = akt->dalsi;
        }

        if (akt == NULL)
        {
            // Ak nenájdeme predchádzajúci prvok, ukončíme funkciu
            return;
        }

        // Aktualizujeme odkaz z predchádzajúceho prvku na temp
        akt->dalsi = temp;
        return;
    }

    if (zap1 == NULL || zap2 == NULL)
    {
        return;
    }

    // Získame nasledujúce záznamy po zap1 a zap2
    Zaznam *zap1_old_next = zap1->dalsi;
    Zaznam *zap2_old_next = zap2->dalsi;

    // Prehodíme zap1 a zap2 vo zozname
    zamena(zaz, zap1, zap2_old_next, c2, pocet_zaznamov);
    zamena(zaz, zap2, zap1_old_next, c1, pocet_zaznamov);
}




int main()
{
    FILE *dataloger;
    Zaznam *zaz = NULL;
    int pocet_zaznamov = 0;

    char command;
    while (1)
    {
        scanf(" %c", &command);
        switch (command)
        {
        case 'n':
            uvolnit_zaznamy(&zaz); 
            nacitanie(&dataloger, &zaz, &pocet_zaznamov);
            break;
        case 'v':
            vypis_zaznamov(zaz);
            break;
        case 'p':
            novy_zaznam(&zaz, &pocet_zaznamov);
            break;
        case 'z':
            vymaz_zaznamov(&zaz, &pocet_zaznamov);
            break;
        case 'u':
            usporiadanie_zaznamov(&zaz, &pocet_zaznamov);
            break;
        case 'r':
            zmena_zaznamov(&zaz, pocet_zaznamov);
            break;
        case '0':
            uvolnit_zaznamy(&zaz);
            break;
        case 'k':
            uvolnit_zaznamy(&zaz);
            return 0;
            break;
        default:
            printf("Neznamy prikaz!\n");
        }
    }

    return 0;
}