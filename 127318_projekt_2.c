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
    char datumCas[20];
    struct Zaznam *dalsi; // Ukazovateľ na ďalší záznam
} Zaznam;

void uvolnit_zaznamy(Zaznam **zaz) {
    Zaznam *akt;
    while (*zaz != NULL) {
        akt = *zaz;
        *zaz = (*zaz)->dalsi;
        free(akt);
    }
}


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
            sprintf(akt->datumCas, "%d%s", akt->datum, akt->cas);
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
        printf("%s\n", akt->datumCas);
        akt = akt->dalsi;
        index++;
    }
}

void novy_zaznam(Zaznam **zaz, int *pocet_zaznamov) {
    int miesto;
    scanf("%d", &miesto);
    if (miesto > (*pocet_zaznamov)) {
        miesto = (*pocet_zaznamov) + 1;
    }
    int index = 1;
    int prov = 0;
    Zaznam *novyZaznam = (Zaznam *)malloc(sizeof(Zaznam));
    scanf("%5s %16s %2s %lf %4s %d", novyZaznam->id, novyZaznam->poz, novyZaznam->typ, &novyZaznam->hod, novyZaznam->cas, &novyZaznam->datum);
    novyZaznam->dalsi = NULL;
    if (miesto == 1) {
        novyZaznam->dalsi = *zaz;
        *zaz = novyZaznam;
    } else {
        Zaznam *akt = (*zaz);
        while (akt != NULL) {
            if (index == miesto - 1) {
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
    char id[6];
    scanf("%5s", id);
    Zaznam *akt = *zaz;
    Zaznam *pred = NULL;
    while (akt!= NULL) {
        if (strcmp(akt->id, id) == 0) {
            if (pred == NULL) {
                *zaz = akt->dalsi;
            }
            else {
                pred->dalsi = akt->dalsi;
            }
            Zaznam *pam = akt;
            akt = akt->dalsi;

            free(pam);
            (*pocet_zaznamov)--;

            printf("Zaznam pre ID: %s bol vymazany.\n", id);
            continue;
        }
        pred = akt;
        akt = akt->dalsi;
    }

}

Zaznam* spojenie(Zaznam *prvy, Zaznam *druhy) {
    if (prvy == NULL) return druhy;
    if (druhy == NULL) return prvy;
    if (strcmp(prvy->datumCas, druhy->datumCas) < 0){
        prvy->dalsi = spojenie(prvy->dalsi, druhy);
        return prvy;
    }
    else{
        druhy->dalsi = spojenie(prvy, druhy->dalsi);
        return druhy;
    }
}

Zaznam* oddelenie(Zaznam *akt){
    Zaznam *fast = akt;
    Zaznam *slow = akt;
    Zaznam *temp = NULL;
    while (fast != NULL && fast->dalsi != NULL){
        fast = fast->dalsi->dalsi;
        temp = slow;
        slow = slow->dalsi;
    }
    if (temp != NULL){
        temp->dalsi = NULL;
    }
    return slow;
}

Zaznam* usporiadanie(Zaznam *akt) {
    if (akt == NULL || akt->dalsi == NULL){
        return akt;
    }
    Zaznam* druhy = oddelenie(akt);
    akt = usporiadanie(akt);
    druhy = usporiadanie(druhy);

    return spojenie(akt, druhy);
}

void usporiadanie_zaznamov(Zaznam **zaz, int *pocet_zaznamov){
    if (zaz != NULL && *zaz != NULL){
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

Zaznam* probeh(Zaznam *akt, int k, int pocet_zaznamov) {
    for (int i = 1; i <= pocet_zaznamov; i++){
        if (i == k){
            return akt;
        }
        akt = akt->dalsi;
    }
    return NULL;
}
void zamena(Zaznam **zaz, Zaznam *zap, Zaznam *zap_old, int k, int pocet_zaznamov) {
    Zaznam *akt = *zaz;
    Zaznam **pred = NULL;
    if (k == 1){
        *zaz = zap;
        (*zaz)->dalsi = zap_old;
        return;
    }
    for (int i = 1; i < k - 1 && akt != NULL; i++) {
        akt = akt->dalsi;
    }
   
    if (akt == NULL) {
        return;
    } 
    akt->dalsi = zap; 
    akt->dalsi->dalsi = zap_old;
}



void zmena_zaznamov(Zaznam **zaz, int pocet_zaznamov) {
    int c1, c2;
    scanf("%d %d", &c1, &c2);

    if (c1 <= 0 || c2 <= 0 || c1 > pocet_zaznamov || c2 > pocet_zaznamov || c1 == c2) {
        return;
    }

    if (c1 > c2) {
        swap(&c1, &c2);
    }

    Zaznam *zap1 = probeh(*zaz, c1, pocet_zaznamov);
    Zaznam *zap2 = probeh(*zaz, c2, pocet_zaznamov);

    if (c1 == c2 - 1){
        if (c1 == 1){
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
        printf("%s\n", temp->dalsi->poz);
        Zaznam *akt = *zaz;
        for (int i = 1; i < c1 - 1 && akt!= NULL; i++){
            akt = akt->dalsi;
        }
        printf("%s\n", akt->poz);
        if (akt == NULL) {
            return;
        }
        akt->dalsi = temp;
        return;
    }

 

    printf("%s\n", zap1->poz);
    printf("%s\n", zap2->poz);

    if (zap1 == NULL || zap2 == NULL) {
        return; // Один из узлов не найден
    }

    Zaznam *zap1_old_next = zap1->dalsi;
    Zaznam *zap2_old_next = zap2->dalsi;

    zamena(zaz, zap1, zap2_old_next, c2, pocet_zaznamov); // Вставляем zap1 на новое место

    zamena(zaz, zap2, zap1_old_next, c1, pocet_zaznamov); // Вставляем zap2 на новое место

    // Вывод списка для проверки
    vypis_zaznamov(*zaz);
   
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
            case'k':
                uvolnit_zaznamy(&zaz);
                return 0;
                break;
            default:
                printf("Neznamy prikaz!\n");
        }
    } while (command != 'k');

    return 0;
}
