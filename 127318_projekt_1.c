#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 1000

void checkOpenFile(FILE **Dataloger, int *kol_v, int kol_n, int *numberOfRecords, char ***ID, char ***pozicia, char ***typ, double **hodnota, char ***cas, int **datum)
{
    if (*kol_v == 0)
    {
        *Dataloger = fopen("dataloger.txt", "r"); // Ak súbor nebol otvorený, otvorí súbor "dataloger.txt" na čítanie a nastaví 'kol_v' na 1
        *kol_v = 1;
    }

    if (*Dataloger == NULL)
    {
        printf("Neotvoreny subor\n"); // Ak 'Dataloger' je NULL, vypíše "Neotvoreny subor" a vráti sa
        return;
    }

    if (kol_n == 1)
    {
        // Ak už boli vytvorené dynamické polia, vypíše informácie pre každý záznam z polia
        for (int i = 0; i < *numberOfRecords; i++)
        {
            printf("ID. mer. modulu: %s\n", (*ID)[i]);
            printf("Pozicia modulu: %s\n", (*pozicia)[i]);
            printf("Typ mer. veliciny: %s\n", (*typ)[i]);
            printf("Hodnota: %lf\n", (*hodnota)[i]);
            printf("Cas merania: %s\n", (*cas)[i]);
            printf("Datum merania: %d\n", (*datum)[i]);
            printf("\n");
        }
    }
    else
    {
        // Ak neboli vytvorené dynamické polia, prechádza súborom a vypisuje informácie pre každý záznam zo súboru
        while (1)
        {
            char s[20];
            int w;
            double d;
            if (fscanf(*Dataloger, "%s", s) != EOF)
            {
                printf("ID. mer. modulu: %s\n", s);

                if (fscanf(*Dataloger, "%s", s) != EOF)
                {
                    printf("Pozicia modulu: %s\n", s);

                    if (fscanf(*Dataloger, "%s", s) != EOF)
                    {
                        printf("Typ mer. veliciny: %s\n", s);

                        if (fscanf(*Dataloger, "%lf", &d) != EOF)
                        {
                            printf("Hodnota: %lf\n", d);

                            if (fscanf(*Dataloger, "%s", s) != EOF)
                            {
                                printf("Cas merania: %s\n", s);

                                if (fscanf(*Dataloger, "%d", &w) != EOF)
                                {
                                    printf("Datum merania: %d\n", w);
                                }
                            }
                        }
                    }
                }
            }
            else if (s[0] == '\n')
            {
                continue; // Ak načíta prázdny riadok, pokračuje ďalej
            }
            else
            {
                break; // Ak sa nedá načítať ďalšia hodnota, ukončí cyklus
            }
            printf("\n");
        }
        rewind(*Dataloger);
    }
}

// Táto funkcia sa používa na čistenie pamäte a zabránenie úniku pamäte, keď už nie sú potrebné žiadne údaje.

void freedom(char ***ID, char ***pozicia, char ***typ, char ***cas, double **hodnota, int **datum, int numberOfRecords)
{
    for (int i = 0; i < numberOfRecords; i++)
    {
        free((*ID)[i]);
        free((*pozicia)[i]);
        free((*typ)[i]);
        free((*cas)[i]);
    }

    free(*ID);
    free(*pozicia);
    free(*typ);
    free(*cas);
    free(*hodnota);
    free(*datum);
}

void createArrays(FILE **Dataloger, int *kol_n, int *numberOfRecords, char ***ID, char ***pozicia, char ***typ, double **hodnota, char ***cas, int **datum)
{
    // Kontrola, či je súbor otvorený
    if (*Dataloger == NULL)
    {
        printf("Neotvoreny subor\n");
        return;
    }

    // Ak sme už raz prečítali dáta, uvoľníme pamäť
    if (*kol_n == 1)
    {
        freedom(&*ID, &*pozicia, &*typ, &*cas, &*hodnota, &*datum, *numberOfRecords);
        rewind(*Dataloger);
        *numberOfRecords = 0;
    }

    double d;
    int w;
    char *strdup(const char *s);

    // Alokácia pamäte pre dynamické polia
    *ID = (char **)malloc(sizeof(char *));
    *pozicia = (char **)malloc(sizeof(char *));
    *typ = (char **)malloc(sizeof(char *));
    *hodnota = (double *)malloc(sizeof(double));
    *cas = (char **)malloc(sizeof(char *));
    *datum = (int *)malloc(sizeof(int));

    while (1)
    {
        char s[6], s1[15], s2[3], s3[5];

        // Čítanie ID
        if (fscanf(*Dataloger, "%s", s) != EOF && s[0] != '\n')
        {
            // Realokácia pamäte
            *ID = (char **)realloc(*ID, (*numberOfRecords + 1) * sizeof(char *));
            *pozicia = (char **)realloc(*pozicia, (*numberOfRecords + 1) * sizeof(char *));
            *typ = (char **)realloc(*typ, (*numberOfRecords + 1) * sizeof(char *));
            *hodnota = (double *)realloc(*hodnota, (*numberOfRecords + 1) * sizeof(double));
            *cas = (char **)realloc(*cas, (*numberOfRecords + 1) * sizeof(char *));
            *datum = (int *)realloc(*datum, (*numberOfRecords + 1) * sizeof(int));

            (*numberOfRecords)++;
            (*ID)[*numberOfRecords - 1] = strdup(s);

            // Čítanie pozície
            if (fscanf(*Dataloger, "%s", s1) != EOF)
            {
                (*pozicia)[*numberOfRecords - 1] = strdup(s1);

                // Čítanie typu
                if (fscanf(*Dataloger, "%s", s2) != EOF)
                {
                    (*typ)[*numberOfRecords - 1] = strdup(s2);

                    // Čítanie hodnoty
                    if (fscanf(*Dataloger, "%lf", &d) != EOF)
                    {
                        (*hodnota)[*numberOfRecords - 1] = d;

                        // Čítanie času
                        if (fscanf(*Dataloger, "%s", s3) != EOF)
                        {
                            (*cas)[*numberOfRecords - 1] = strdup(s3);

                            // Čítanie dátumu
                            if (fscanf(*Dataloger, "%d", &w) != EOF)
                            {
                                (*datum)[*numberOfRecords - 1] = w;
                            }
                        }
                    }
                }
            }
        }
        else if (s[0] == '\n')
        {
            continue;
        }
        else
        {
            break;
        }
    }
    *kol_n = 1;
}

void checkMonthes(int kol_v, int kol_n, int numberOfRecords, char **ID, int *datum)
{
    // Kontrola, či sú polia vytvorené
    if (kol_n == 0)
    {
        printf("Polia nie sú vytvorené\n");
        return;
    }
    // Kontrola, či je súbor otvorený
    else if (kol_v == 0)
    {
        printf("Neotvorený súbor\n");
        return;
    }

    int y;
    // Vstup od užívateľa - počet mesiacov
    scanf("%d", &y);

    // Otvorenie súboru "ciachovanie.txt" pre čítanie
    FILE *ciachovanie = fopen("ciachovanie.txt", "r");
    int allDataCorrect = 1;
    int *mamAleboNemam = (int *)malloc(numberOfRecords * sizeof(int));
    for (int i = 0; i < numberOfRecords; i++)
    {
        mamAleboNemam[i] = 0;
    }
    while (1)
    {
        char c_ID[6];
        int c_datum;
        int neskordatum = 0;
        int ans = 0;

        // Čítanie ID a dátumu zo súboru "ciachovanie.txt"
        if (fscanf(ciachovanie, "%s", c_ID) != EOF)
        {
            ans = 0;
            fscanf(ciachovanie, "%d", &c_datum);

            // Prechádzanie záznamov v poli ID
            for (int i = 0; i < numberOfRecords; i++)
            {
                if (strcmp(c_ID, ID[i]) == 0)
                {
                    mamAleboNemam[i] = 1;
                    int c_mesiac = 0, c_rok = 0, mesiac = 0, rok = 0;
                    c_mesiac = c_datum % 10000 / 100;
                    c_rok = c_datum / 10000;
                    mesiac = datum[i] % 10000 / 100;
                    rok = datum[i] / 10000;

                    int c_monthes = c_rok * 12 + c_mesiac;
                    int monthes = rok * 12 + mesiac;
                    int difference = abs(c_monthes - monthes);

                    // Porovnanie počtu mesiacov
                    if (neskordatum <= monthes)
                    {
                        neskordatum = monthes;
                        ans = difference;
                    }
                }
            }

            if (ans - y >= 0)
            {
                printf("ID mer. modulu [%s] má %d mesiacov po ciachovani.\n", c_ID, ans - y);
            }
            else
            {
                allDataCorrect = 0;
            }
        }
        else if (c_ID[0] == '\n')
        {
            continue;
        }
        else
        {
            break;
        }
    }

    for (int i = 0; i < numberOfRecords; i++)
    {
        int boloAleboNebolo = 0;
        if (mamAleboNemam[i] == 0)
        {
            for (int j = 0; j < i; j++)
            {
                if (strcmp(ID[i], ID[j]) == 0)
                {
                    boloAleboNebolo = 1;
                }
            }
            if (boloAleboNebolo == 0)
            {
                printf("ID mer. modulu [%s] nie je ciachovany.\n", ID[i]);
                allDataCorrect = 0;
            }
        }
    }

    if (allDataCorrect == 1)
    {
        printf("Data sú korektné.\n");
    }
    free(mamAleboNemam);
    fclose(ciachovanie);
}

// Funkcia swap slúži na výmenu hodnôt dvoch celočíselných premenných a a b.
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Funkcia sortNajdeneArray zabezpečuje triedenie poľa indexov najdene na základe dátumu a času uložených v poli najdene_datumCas.
int *sortNajdeneArray(int *najdene, char **najdene_datumCas, int k, int *ans_k)
{
    // Vytvorenie dynamického poľa pre výsledky
    int *ans = (int *)malloc(sizeof(int));

    for (int i = 0; i < k; i++)
    {
        int zap = -1;
        for (int j = 0; j < *ans_k; j++)
        {
            // Porovnanie dátumu a času medzi existujúcim a novým záznamom
            if (najdene_datumCas[ans[j]] > najdene_datumCas[najdene[i]])
            {
                zap = j;
                break;
            }
        }

        if (zap != -1)
        {
            int scet = zap;
            int zap1 = najdene[i];

            // Preusporiadanie existujúcich záznamov, aby sa vložil nový záznam na správne miesto
            while (scet < *ans_k)
            {
                swap(&zap1, &ans[scet]);
                scet++;
            }

            ans = (int *)realloc(ans, (*ans_k + 1) * sizeof(int));
            (*ans_k)++;
            ans[*ans_k - 1] = zap1;
        }
        else
        {
            // Ak nový záznam neporovnal existujúci, pridáme ho na koniec výsledkov
            ans = (int *)realloc(ans, (*ans_k + 1) * sizeof(int));
            (*ans_k)++;
            ans[*ans_k - 1] = najdene[i];
        }
    }

    return ans;
}

void sortArray(int numberOfRecords, int kol_n, char **ID, char **typ, int *datum, char **cas, double *hodnota, char **pozicia)
{
    // Kontrola, či boli vytvorené potrebné polia
    if (kol_n == 0)
    {
        printf("Polia nie sú vytvorené\n");
        return;
    }

    // Otvorenie súboru "vystup_S.txt" pre zápis
    FILE *vystup_s = fopen("vystup_S.txt", "w");

    // Kontrola, či sa súbor otvoril správne
    if (vystup_s == NULL)
    {
        printf("Pre daný vstup nie je vytvorený textový súbor.\n");
        return;
    }

    // Načítanie hodnôt s_id a s_typ od užívateľa
    char s_id[6], s_typ[3];
    scanf("%s %s", s_id, s_typ);

    // Inicializácia premenných pre vyhľadávané záznamy
    int *najdene = (int *)malloc(numberOfRecords * sizeof(int));
    char **najdene_datumCas = (char **)malloc(numberOfRecords * sizeof(char *));
    char **ALLnajdene_datumCas = (char **)malloc(numberOfRecords * sizeof(char *));
    int k = 0;

    // Prechádzanie všetkými záznamami
    for (int i = 0; i < numberOfRecords; i++)
    {
        // Porovnanie s_id a s_typ so záznamom
        if (strcmp(s_id, ID[i]) == 0 && strcmp(s_typ, typ[i]) == 0)
        {
            najdene[k] = i;

            // Konverzia dátumu a času na reťazec
            char str[9];
            sprintf(str, "%d", datum[i]);
            najdene_datumCas[k] = (char *)malloc(strlen(str) + strlen(cas[i]) + 1);
            strcpy(najdene_datumCas[k], str);
            strcat(najdene_datumCas[k], cas[i]);

            k++;
        }

        // Konverzia dátumu a času všetkých záznamov na reťazec
        char str[9];
        sprintf(str, "%d", datum[i]);
        ALLnajdene_datumCas[i] = (char *)malloc(strlen(str) + strlen(cas[i]) + 1);
        strcpy(ALLnajdene_datumCas[i], str);
        strcat(ALLnajdene_datumCas[i], cas[i]);
    }

    // Triedenie nájdených záznamov
    int ans_k = 0;
    int *ans = sortNajdeneArray(najdene, najdene_datumCas, k, &ans_k);

    // Inicializácia premenných pre formátovanie pozícií
    char **poz1 = (char **)malloc(numberOfRecords * sizeof(char *));
    char **poz2 = (char **)malloc(numberOfRecords * sizeof(char *));

    // Formátovanie pozícií záznamov
    for (int i = 0; i < numberOfRecords; i++)
    {
        poz1[i] = (char *)malloc(20);
        poz2[i] = (char *)malloc(20);
        int k1 = 0, k2 = 0;

        for (int j = 0; j < 15; j++)
        {
            if (j < 7)
            {
                poz1[i][k1] = pozicia[i][j];
                k1++;
            }
            if (k1 == 3)
            {
                poz1[i][k1] = '.';
                k1++;
            }
            if (j >= 7)
            {
                poz2[i][k2] = pozicia[i][j];
                k2++;
            }
            if (k2 == 3)
            {
                poz2[i][k2] = '.';
                k2++;
            }
        }
    }

    // Výpis výsledkov do súboru a informačná správa
    for (int i = 0; i < ans_k; i++)
    {
        fprintf(vystup_s, "%s %.5lf %s %s\n", ALLnajdene_datumCas[ans[i]], hodnota[ans[i]], poz1[ans[i]], poz2[ans[i]]);
    }
    printf("Pre daný vstup je vytvorený textový súbor.\n");

    // Uvoľnenie alokovanej pamäte
    for (int i = 0; i < k; i++)
    {
        free(najdene_datumCas[i]);
    }
    free(najdene_datumCas);
    free(najdene);
    free(ALLnajdene_datumCas);

    for (int i = 0; i < numberOfRecords; i++)
    {
        free(poz1[i]);
        free(poz2[i]);
    }
    free(poz1);
    free(poz2);

    // Uzavretie súboru
    fclose(vystup_s);
}

void minMax(int numberOfRecords, int kol_n, char **typ, double *hodnota)
{
    // Kontrola, či boli vytvorené potrebné polia
    if (kol_n == 0)
    {
        printf("Polia nie sú vytvorené\n");
        return;
    }

    // Inicializácia dynamických polí pre uchovanie minimálnych a maximálnych hodnôt
    double *min = (double *)malloc(numberOfRecords * sizeof(double));
    double *max = (double *)malloc(numberOfRecords * sizeof(double));
    char **numberOfTyps = (char **)malloc(numberOfRecords * sizeof(char *));
    int *countOfTyps = (int *)malloc(numberOfRecords * sizeof(int));
    int k_num = 0;

    // Prechádzanie všetkými záznamami
    for (int i = 0; i < numberOfRecords; i++)
    {
        int act_number = -1;

        // Prehľadávanie poľa numberOfTyps, či sa daný typ už nachádza
        for (int j = 0; j < k_num; j++)
        {
            if (strcmp(numberOfTyps[j], typ[i]) == 0)
            {
                act_number = j;
                break;
            }
        }

        // Ak sa typ ešte nenachádza v numberOfTyps
        if (act_number == -1)
        {
            k_num += 1;
            numberOfTyps[k_num - 1] = typ[i];
            act_number = k_num - 1;
            min[act_number] = hodnota[i];
            max[act_number] = hodnota[i];
            countOfTyps[act_number] = 1;
        }
        else
        {
            countOfTyps[act_number]++;
            if (min[act_number] > hodnota[i])
            {
                min[act_number] = hodnota[i];
            }
            if (max[act_number] < hodnota[i])
            {
                max[act_number] = hodnota[i];
            }
        }
    }

    // Výpis minimálnych a maximálnych hodnôt pre jednotlivé typy
    for (int i = 0; i < k_num; i++)
    {
        printf("%s %d %.2lf %.2lf\n", numberOfTyps[i], countOfTyps[i], min[i], max[i]);
    }
}

void delete(int kol_n, int *numberOfRecords, char ***ID, char ***pozicia, char ***typ, double **hodnota, char ***cas, int **datum)
{
    // Kontrola, či boli vytvorené potrebné polia
    if (kol_n == 0)
    {
        printf("Polia nie sú vytvorené\n");
        return;
    }

    // Načítanie ID na vymazanie
    char z_ID[6];
    scanf("%s", z_ID);

    // Inicializácia nových premenných pre údaje bez vymazaných záznamov
    int newNumberOfRecords = 0;
    char **newID = (char **)malloc(*numberOfRecords * sizeof(char *));
    char **newPozicia = (char **)malloc(*numberOfRecords * sizeof(char *));
    char **newTyp = (char **)malloc(*numberOfRecords * sizeof(char *));
    double *newHodnota = (double *)malloc(*numberOfRecords * sizeof(double));
    char **newCas = (char **)malloc(*numberOfRecords * sizeof(char *));
    int *newDatum = (int *)malloc(*numberOfRecords * sizeof(int));

    int vym_k = 0;
    char *strdup(const char *s);
    // Prechádzanie všetkými záznamami
    for (int i = 0; i < *numberOfRecords; i++)
    {
        // Ak sa záznam nemá vymazať
        if (strcmp((*ID)[i], z_ID) != 0)
        {
            newID[newNumberOfRecords] = strdup((*ID)[i]);
            newPozicia[newNumberOfRecords] = strdup((*pozicia)[i]);
            newTyp[newNumberOfRecords] = strdup((*typ)[i]);
            newHodnota[newNumberOfRecords] = (*hodnota)[i];
            newCas[newNumberOfRecords] = strdup((*cas)[i]);
            newDatum[newNumberOfRecords] = (*datum)[i];
            newNumberOfRecords++;
        }
        else
        {
            vym_k++;
        }
    }

    // Uvoľnenie pôvodných polí
    freedom(&*ID, &*pozicia, &*typ, &*cas, &*hodnota, &*datum, *numberOfRecords);

    // Aktualizácia ukazovateľov na nové polia
    *numberOfRecords = newNumberOfRecords;
    *ID = newID;
    *pozicia = newPozicia;
    *typ = newTyp;
    *hodnota = newHodnota;
    *cas = newCas;
    *datum = newDatum;

    printf("Vymazalo sa : %d záznamov !\n", vym_k);
}

int main()
{
    FILE *Dataloger;

    int numberOfRecords = 0;
    int kol_v = 0, kol_n = 0;

    char **ID;
    char **pozicia;
    char **typ;
    double *hodnota;
    char **cas;
    int *datum;

    char command;

    while (1)
    {

        scanf("%c", &command);

        if (command == 'v')
        {
            checkOpenFile(&Dataloger, &kol_v, kol_n, &numberOfRecords, &ID, &pozicia, &typ, &hodnota, &cas, &datum);
        }

        if (command == 'n')
        {
            createArrays(&Dataloger, &kol_n, &numberOfRecords, &ID, &pozicia, &typ, &hodnota, &cas, &datum);
        }

        if (command == 'c')
        {
            checkMonthes(kol_v, kol_n, numberOfRecords, ID, datum);
        }

        if (command == 's')
        {
            sortArray(numberOfRecords, kol_n, ID, typ, datum, cas, hodnota, pozicia);
        }

        if (command == 'h')
        {
            minMax(numberOfRecords, kol_n, typ, hodnota);
        }

        if (command == 'z')
        {
            delete (kol_n, &numberOfRecords, &ID, &pozicia, &typ, &hodnota, &cas, &datum);
        }

        if (command == 'k')
        {
            // Kontrola, či bol súbor otvorený a polia boli vytvorené
            if (kol_n == 1)
            {
                fclose(Dataloger);
                // Uvoľníme pamäť
                freedom(&ID, &pozicia, &typ, &cas, &hodnota, &datum, numberOfRecords);
            }
            else if (kol_v == 1){
                fclose(Dataloger);
            }
            return 0;
        }
    }
}