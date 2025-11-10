#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Potrzebne dla INT_MAX


int min(int a, int b){
    if(a < b) return a;
    return b;
}

int max(int a, int b){
    if(a < b) return b;
    return a;
}

int main(){
    int n;
    if (scanf("%d", &n) != 1) {
        return 1; // Zakończ program, jeśli wczytanie się nie powiodło
    }
    // Twoja alokacja pamięci (jest poprawna)
    int *typ = malloc((size_t)n * sizeof(int));
    int *pozycja = malloc((size_t)n * sizeof(int));
    int **last_pref = malloc((size_t)n * sizeof(int *));
    int **last_suf = malloc((size_t)n * sizeof(int *));
    int **typ_pref = malloc((size_t)n * sizeof(int *));
    int **typ_suf = malloc((size_t)n * sizeof(int *));

    // Twoja inicjalizacja (jest poprawna)
    for (int i = 0; i < n; i++) {
        last_pref[i] = malloc(3 * sizeof(int));
        last_suf[i] = malloc(3 * sizeof(int));
        typ_pref[i] = malloc(3 * sizeof(int));
        typ_suf[i] = malloc(3 * sizeof(int));
        for (int j = 0; j < 3; j++) {
            last_suf[i][j] = -1;
            last_pref[i][j] = -1;
            typ_suf[i][j] = -1;
            typ_pref[i][j] = -1;
        }
    }

    // Twoje wczytywanie danych (jest poprawne)
    for (int i = 0; i < n; i++) {
    if (scanf("%d %d", &typ[i], &pozycja[i]) != 2) {
        return 1; // Zakończ program, jeśli wczytanie się nie powiodło
    }
}

    if(n < 3){
        printf("0 0\n");
        return 0;
    }

    last_pref[0][0] = pozycja[0];
    typ_pref[0][0] = typ[0];
    for (int i = 1; i < n; i++) {
        last_pref[i][0] = pozycja[i];
        typ_pref[i][0] = typ[i];
        // Sprawdź historię z poprzedniego kroku (i-1)
        if (typ[i] != typ_pref[i - 1][0]) {
            // Nowy motel ma inny typ niż ostatni z i-1
            last_pref[i][1] = last_pref[i - 1][0];
            typ_pref[i][1] = typ_pref[i - 1][0];
            if (typ[i] != typ_pref[i - 1][1]) {
                // Nowy motel ma też inny typ niż drugi ostatni z i-1
                last_pref[i][2] = last_pref[i - 1][1];
                typ_pref[i][2] = typ_pref[i - 1][1];
            } else {
                // Nowy motel ma taki sam typ jak drugi ostatni, więc bierzemy trzeciego
                last_pref[i][2] = last_pref[i - 1][2];
                typ_pref[i][2] = typ_pref[i - 1][2];
            }
        } else {
            // Nowy motel ma taki sam typ jak ostatni z i-1.
            // Po prostu kopiujemy historię.
            last_pref[i][1] = last_pref[i - 1][1];
            typ_pref[i][1] = typ_pref[i - 1][1];
            last_pref[i][2] = last_pref[i - 1][2];
            typ_pref[i][2] = typ_pref[i - 1][2];
        }
    }

    last_suf[n - 1][0] = pozycja[n - 1];
    typ_suf[n - 1][0] = typ[n - 1];
    for(int i = n - 2; i >= 0; i--){
        last_suf[i][0] = pozycja[i];
        typ_suf[i][0] = typ[i];
        if (typ[i] != typ_suf[i + 1][0]) {
            // Nowy motel ma inny typ niż ostatni z i-1
            last_suf[i][1] = last_suf[i + 1][0];
            typ_suf[i][1] = typ_suf[i + 1][0];
            if (typ[i] != typ_suf[i + 1][1]) {
                // Nowy motel ma też inny typ niż drugi ostatni z i-1
                last_suf[i][2] = last_suf[i + 1][1];
                typ_suf[i][2] = typ_suf[i + 1][1];
            } else {
                // Nowy motel ma taki sam typ jak drugi ostatni, więc bierzemy trzeciego
                last_suf[i][2] = last_suf[i + 1][2];
                typ_suf[i][2] = typ_suf[i + 1][2];
            }
        } else {
            // Nowy motel ma taki sam typ jak ostatni z i-1.
            // Po prostu kopiujemy historię.
            last_suf[i][1] = last_suf[i + 1][1];
            typ_suf[i][1] = typ_suf[i + 1][1];
            last_suf[i][2] = last_suf[i + 1][2];
            typ_suf[i][2] = typ_suf[i + 1][2];
        }
    }

    int min_max_dist = INT_MAX;
    for(int i = 1; i < n - 1; i++){
        int B_typ = typ[i];
        int B_pos = pozycja[i];
        for(int k = 1; k < 3; k++){
            for(int j = 1; j < 3; j++){
                int A_pos = last_pref[i][k];
                int A_typ = typ_pref[i][k];
                int C_pos = last_suf[i][j];
                int C_typ = typ_suf[i][j];
                if(A_typ != - 1 && B_typ != -1 && C_typ != -1 && A_typ != B_typ && A_typ != C_typ && B_typ != C_typ)
                    min_max_dist = min(max(B_pos - A_pos, C_pos - B_pos), min_max_dist);
            }
        }
    }

    int *first_pos = malloc(3 * sizeof(int));
    int *first_typ = malloc(3 * sizeof(int));
    int *last_pos = malloc(3 * sizeof(int));
    int *last_typ = malloc(3 * sizeof(int));
    for(int i = 0; i < 3; i++){
        first_pos[i] = -1;
        first_typ[i] = -1;
        last_pos[i] = -1;
        last_typ[i] = -1;
    }
    first_pos[0] = pozycja[0];
    first_typ[0] = typ[0];
    for(int i = 1; i < n; i++){
        if(first_typ[1] == -1 && typ[i] != typ[0]){
            first_typ[1] = typ[i];
            first_pos[1] = pozycja[i];
            continue;
        }
        if(first_typ[2] == -1 && typ[i] != typ[0] && typ[i] != first_typ[1]){
            first_typ[2] = typ[i];
            first_pos[2] = pozycja[i];
        }
    }
    last_pos[0] = pozycja[n - 1];
    last_typ[0] = typ[n - 1];
    for(int i = n - 2; i >= 0; i--){
        if(last_typ[1] == -1 && typ[i] != typ[n - 1]){
            last_typ[1] = typ[i];
            last_pos[1] = pozycja[i];
            continue;
        }
        if(last_typ[2] == -1 && typ[i] != typ[n - 1] && typ[i] != last_typ[1]){
            last_typ[2] = typ[i];
            last_pos[2] = pozycja[i];
        }
    }
//     fprintf(stderr, "--- Debug: first_pos / first_typ ---\n");
//     for(int i = 0; i < 3; i++){
//         fprintf(stderr, "Indeks %d: Pozycja = %d, Typ = %d\n", i, first_pos[i], first_typ[i]);
//     }
//
//     fprintf(stderr, "\n--- Debug: last_pos / last_typ ---\n");
//     for(int i = 0; i < 3; i++){
//         fprintf(stderr, "Indeks %d: Pozycja = %d, Typ = %d\n", i, last_pos[i], last_typ[i]);
//     }
//     fprintf(stderr, "---------------------------------------\n\n");
    // ----- KONIEC BLOKU DEBUGOWANIA -----
    int max_min_dist = -1;
    for(int i = 1; i < n - 1; i++){
        for(int k = 0; k < 3; k++){
            for(int j = 0; j < 3; j++){
                int A_pos = first_pos[k];
                int A_typ = first_typ[k];
                int B_pos = pozycja[i];
                int B_typ = typ[i];
                int C_pos = last_pos[j];
                int C_typ = last_typ[j];
                if(A_typ != -1 && B_typ != -1 && C_typ != -1){
                    if(A_pos <= B_pos && B_pos <= C_pos && A_typ != B_typ && C_typ != B_typ &&  C_typ != A_typ){
                        max_min_dist = max(max_min_dist, min(B_pos - A_pos, C_pos - B_pos));
                        //printf("%d %d\n", max_min_dist, i);
                    }
                }
            }
        }
    }
    // 4. WYPISANIE WYNIKÓW
    if (max_min_dist == -1)
        printf("0 0\n");
    else
        printf("%d %d\n", min_max_dist, max_min_dist);


    // 5. ZWALNIANIE PAMIĘCI (bardzo ważne!)
    for (int i = 0; i < n; i++) {
        free(last_pref[i]);
        free(last_suf[i]);
        free(typ_pref[i]);
        free(typ_suf[i]);
    }
    free(last_pref);
    free(last_suf);
    free(typ_pref);
    free(typ_suf);
    free(typ);
    free(pozycja);

    return 0; // Dobra praktyka
}
