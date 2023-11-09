#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Zaznam {
    char id[6];
    char poz[17];
    char typ[3];
    double hod;
    char cas[5];
    int datum;
    struct Zaznam *dalsi; // Ukazovateľ na ďalší záznam
} Zaznam;

void nacitanie(FILE **dataloger, Zaznam **zaz, int *pocet_zaznamov) {
    
    *dataloger = fopen("dataloger_V2.txt", "r");

    if (*dataloger == NULL) {
        printf("Zaznamy neboli nacitane!\n");
        return;
    }

    Zaznam *akt, *posledny = NULL;
    *pocet_zaznamov = 0;

    while (!feof(*dataloger)) {
        akt = (Zaznam *)malloc(sizeof(Zaznam));

        if (fscanf(*dataloger, "%5s %16s %2s %lf %4s %d",
                   akt->id, akt->poz, akt->typ,
                   &akt->hod, akt->cas, &akt->datum) == 6) {
            akt->dalsi = NULL;
            if (posledny != NULL) {
                posledny->dalsi = akt;
            } else {
                *zaz = akt;
            }
            posledny = akt;
            (*pocet_zaznamov)++;
        } else {
            free(akt);
            break;
        }
    }
    fclose(*dataloger);
    printf("Nacitalo sa %d zaznamov\n", *pocet_zaznamov);
}

void vypis_zaznamov(Zaznam *zaz) {
    Zaznam *akt = zaz;
    int index = 1;
    while (akt != NULL) {
        printf("%d:\n", index);
        printf("ID: %s\t%s\t%2lf\n", akt->id, akt->typ, akt->hod);
        printf("Poz: %.8s\t%.8s\n", akt->poz, akt->poz + 8);
        printf("DaC: %d\t%s\n", akt->datum, akt->cas);

        akt = akt->dalsi;
        index++;
    }
}

// void novy_zaznam(Zaznam **zaz, int *pocet_zaznamov) {
//     int miesto;
//     scanf("%d", &miesto);
//     Zaznam *akt = (Zaznam *)malloc(sizeof(Zaznam));
//     akt = *zaz;
//     int index = 1;
//     int prov = 0;
//     Zaznam novyZaznam;
//     Zaznam *zap;
    
//     scanf("%5s %16s %2s %lf %4s %d", novyZaznam.id, novyZaznam.poz, novyZaznam.typ, &novyZaznam.hod, novyZaznam.cas, &novyZaznam.datum);
//     while (akt!= NULL) {
//         if (index == miesto) {
//             prov = 1;
//             zap = akt;
//             akt = novyZaznam;
//             akt = akt->dalsi;
//         }
        
//         if (prov == 1) {
//             break;
//         }
//         akt = akt->dalsi;
//         index++;
//     }
// }

void uvolnit_zaznamy(Zaznam **zaz) {
    Zaznam *akt;
    while (*zaz != NULL) {
        akt = *zaz;
        *zaz = (*zaz)->dalsi;
        free(akt);
    }
}

int main() {
    FILE *dataloger;
    Zaznam *zaz = NULL;
    int pocet_zaznamov = 0;

    char command;
    do {
        scanf(" %c", &command); // Pridaný medzera pred %c odstraňuje whitespace znaky pred príkazom
        switch (command) {
            case 'n':
                uvolnit_zaznamy(&zaz); // Uvolníme predchádzajúce záznamy ak existujú
                nacitanie(&dataloger, &zaz, &pocet_zaznamov);
                break;
            case 'v':
                vypis_zaznamov(zaz);
                break;
            case 'p':
                // novy_zaznam(&zaz, &pocet_zaznamov);
            case '0':
                uvolnit_zaznamy(&zaz);
                break;
            default:
                printf("Neznamy prikaz!\n");
        }
    } while (command != '0');

    return 0;
}
