#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Neviem na čo, ale vytvárame túto štruktúru
typedef struct ID
{
    char oznacenie;
    int cislovanie;
    char druh;
} ID;

// Neviem na čo, ale vytvárame túto štruktúru
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
    while (*zaz != NULL)
    {
        akt = *zaz;
        *zaz = (*zaz)->dalsi;
        free(akt);
    }
}

void nacitanie(FILE **dataloger, Zaznam **zaz, int *pocet_zaznamov)
{

    *dataloger = fopen("dataloger_V2.txt", "r");

    if (*dataloger == NULL)
    {
        printf("Zaznamy neboli nacitane!\n");
        return;
    }

    Zaznam *akt, *posledny = NULL;
    *pocet_zaznamov = 0;

    while (!feof(*dataloger))
    {
        akt = (Zaznam *)malloc(sizeof(Zaznam));
        char ID_norm[6];
        char poz_norm[17];
        if (fscanf(*dataloger, "%5s %16s %2s %lf %4s %d",
                   ID_norm, poz_norm, akt->typ,
                   &akt->hod, akt->cas, &akt->datum) == 6)
        {
            sprintf(akt->datumCas, "%d%s", akt->datum, akt->cas);

            akt->id.oznacenie = ID_norm[0];
            akt->id.druh = ID_norm[4];
            char middleThree[3];
            for (int i = 1; i < 4; i++)
            {
                middleThree[i - 1] = ID_norm[i];
            }
            akt->id.cislovanie = atoi(middleThree);

            char latitude[7], longitude[7];
            // printf("%s\n", poz_norm);

            for (int i = 1; i < 8; i++)
            {
                latitude[i - 1] = poz_norm[i];
            }
            for (int i = 9; i < 16; i++)
            {
                longitude[i - 9] = poz_norm[i];
            }

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

            // printf("%.4lf\n", akt->poz.latitude);

            akt->dalsi = NULL;
            if (posledny != NULL)
            {
                posledny->dalsi = akt;
            }
            else
            {
                *zaz = akt;
            }
            posledny = akt;
            (*pocet_zaznamov)++;
        }
        else
        {
            free(akt);
            break;
        }
    }
    fclose(*dataloger);
    printf("Nacitalo sa %d zaznamov\n", *pocet_zaznamov);
}

void vypis_zaznamov(Zaznam *zaz)
{
    Zaznam *akt = zaz;
    int index = 1;
    while (akt != NULL)
    {
        printf("%d:\n", index);
        printf("ID: %c%d%c\t%s\t%2lf\n", akt->id.oznacenie, akt->id.cislovanie, akt->id.druh, akt->typ, akt->hod);
        if (akt->poz.latitude > 0)
        {
            printf("Poz: +%.4lf\t", akt->poz.latitude);
        }
        else
        {
            printf("Poz: -%.4lf\t", akt->poz.latitude);
        }
        if (akt->poz.longitude > 0)
        {
            printf("+%.4lf\n", akt->poz.longitude);
        }
        else
        {
            printf("Lon: -%.4lf\n", akt->poz.longitude);
        }
        printf("DaC: %d\t%s\n", akt->datum, akt->cas);
        // printf("%s\n", akt->datumCas);
        akt = akt->dalsi;
        index++;
    }
}

void novy_zaznam(Zaznam **zaz, int *pocet_zaznamov)
{
    int miesto;
    scanf("%d", &miesto);
    if (miesto > (*pocet_zaznamov))
    {
        miesto = (*pocet_zaznamov) + 1;
    }
    int index = 1;
    int prov = 0;
    Zaznam *novyZaznam = (Zaznam *)malloc(sizeof(Zaznam));

    char ID_norm[6];
    char poz_norm[17];
    scanf("%5s %16s %2s %lf %4s %d", ID_norm, poz_norm, novyZaznam->typ, &novyZaznam->hod, novyZaznam->cas, &novyZaznam->datum);

    novyZaznam->id.oznacenie = ID_norm[0];
    novyZaznam->id.druh = ID_norm[4];
    char middleThree[3];
    for (int i = 1; i < 4; i++)
    {
        middleThree[i - 1] = ID_norm[i];
    }
    novyZaznam->id.cislovanie = atoi(middleThree);

    char latitude[7], longitude[7];

    for (int i = 1; i < 8; i++)
    {
        latitude[i - 1] = poz_norm[i];
    }
    for (int i = 9; i < 16; i++)
    {
        longitude[i - 9] = poz_norm[i];
    }

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

    novyZaznam->dalsi = NULL;
    if (miesto == 1)
    {
        novyZaznam->dalsi = *zaz;
        *zaz = novyZaznam;
    }
    else
    {
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
    (*pocet_zaznamov)++;
    printf("Nový záznam bol vytvorený\n");
}

void vymaz_zaznamov(Zaznam **zaz, int *pocet_zaznamov)
{
    char ID_norm[6];
    scanf("%5s", ID_norm);

    char oznacenie_vym = ID_norm[0];
    char druh_vym = ID_norm[4];
    char middleThree[3];
    for (int i = 1; i < 4; i++)
    {
        middleThree[i - 1] = ID_norm[i];
    }
    int cislovanie_vym = atoi(middleThree);
    printf("%d\n", cislovanie_vym);
    Zaznam *akt = *zaz;
    Zaznam *pred = NULL;
    while (akt != NULL)
    {
        if (akt->id.cislovanie == cislovanie_vym && akt->id.oznacenie == oznacenie_vym && akt->id.druh == druh_vym)
        {

            if (pred == NULL)
            {
                *zaz = akt->dalsi;
            }
            else
            {
                pred->dalsi = akt->dalsi;
            }
            Zaznam *pam = akt;
            akt = akt->dalsi;

            free(pam);
            (*pocet_zaznamov)--;

            printf("Zaznam pre ID: %c%d%c bol vymazany.\n", oznacenie_vym, cislovanie_vym, druh_vym);
            continue;
        }
        pred = akt;
        akt = akt->dalsi;
    }
}

Zaznam *spojenie(Zaznam *prvy, Zaznam *druhy)
{
    if (prvy == NULL)
        return druhy;
    if (druhy == NULL)
        return prvy;
    if (strcmp(prvy->datumCas, druhy->datumCas) < 0)
    {
        prvy->dalsi = spojenie(prvy->dalsi, druhy);
        return prvy;
    }
    else
    {
        druhy->dalsi = spojenie(prvy, druhy->dalsi);
        return druhy;
    }
}

Zaznam *oddelenie(Zaznam *akt)
{
    Zaznam *fast = akt;
    Zaznam *slow = akt;
    Zaznam *temp = NULL;
    while (fast != NULL && fast->dalsi != NULL)
    {
        fast = fast->dalsi->dalsi;
        temp = slow;
        slow = slow->dalsi;
    }
    if (temp != NULL)
    {
        temp->dalsi = NULL;
    }
    return slow;
}

Zaznam *usporiadanie(Zaznam *akt)
{
    if (akt == NULL || akt->dalsi == NULL)
    {
        return akt;
    }
    Zaznam *druhy = oddelenie(akt);
    akt = usporiadanie(akt);
    druhy = usporiadanie(druhy);

    return spojenie(akt, druhy);
}

void usporiadanie_zaznamov(Zaznam **zaz, int *pocet_zaznamov)
{
    if (zaz != NULL && *zaz != NULL)
    {
        *zaz = usporiadanie(*zaz);
        printf("Zaznamov bol usporiadane.\n");
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

Zaznam *probeh(Zaznam *akt, int k, int pocet_zaznamov)
{
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
void zamena(Zaznam **zaz, Zaznam *zap, Zaznam *zap_old, int k, int pocet_zaznamov)
{
    Zaznam *akt = *zaz;
    Zaznam **pred = NULL;
    if (k == 1)
    {
        *zaz = zap;
        (*zaz)->dalsi = zap_old;
        return;
    }
    for (int i = 1; i < k - 1 && akt != NULL; i++)
    {
        akt = akt->dalsi;
    }

    if (akt == NULL)
    {
        return;
    }
    akt->dalsi = zap;
    akt->dalsi->dalsi = zap_old;
}

void zmena_zaznamov(Zaznam **zaz, int pocet_zaznamov)
{
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

    Zaznam *zap1 = probeh(*zaz, c1, pocet_zaznamov);
    Zaznam *zap2 = probeh(*zaz, c2, pocet_zaznamov);

    if (c1 == c2 - 1)
    {
        if (c1 == 1)
        {
            Zaznam *temp;
            zap1->dalsi = zap2->dalsi;
            temp = NULL;
            temp = zap2;
            temp->dalsi = zap1;
            *zaz = temp;
            return;
        }
        Zaznam *temp;
        zap1->dalsi = zap2->dalsi;
        temp = NULL;
        temp = zap2;
        temp->dalsi = zap1;
        Zaznam *akt = *zaz;
        for (int i = 1; i < c1 - 1 && akt != NULL; i++)
        {
            akt = akt->dalsi;
        }
        if (akt == NULL)
        {
            return;
        }
        akt->dalsi = temp;
        return;
    }

    if (zap1 == NULL || zap2 == NULL)
    {
        return; // Один из узлов не найден
    }

    Zaznam *zap1_old_next = zap1->dalsi;
    Zaznam *zap2_old_next = zap2->dalsi;

    zamena(zaz, zap1, zap2_old_next, c2, pocet_zaznamov); // Вставляем zap1 на новое место

    zamena(zaz, zap2, zap1_old_next, c1, pocet_zaznamov); // Вставляем zap2 на новое место

    // Вывод списка для проверки
    vypis_zaznamov(*zaz);
}

int main()
{
    FILE *dataloger;
    Zaznam *zaz = NULL;
    int pocet_zaznamov = 0;

    char command;
    do
    {
        scanf(" %c", &command); // Pridaný medzera pred %c odstraňuje whitespace znaky pred príkazom
        switch (command)
        {
        case 'n':
            uvolnit_zaznamy(&zaz); // Uvolníme predchádzajúce záznamy ak existujú
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
    } while (command != 'k');

    return 0;
}
