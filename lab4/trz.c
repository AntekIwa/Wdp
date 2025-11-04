#include <stdio.h> 
#include <stdlib.h> // Potrzebne dla malloc i free

// Makra zastępujące std::min i std::max z C++
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// Przenosimy stałe, ale nie same tablice
const int large_pos = 2000000000;
const int large_neg = -2000000000;

int main() {
    int n;
    scanf("%d", &n);

    // --- Dynamiczna alokacja pamięci na stercie ---

    // Zamiast: int motel_type[n]; (co też mogłoby zawieść dla dużego n w C90)
    int *motel_type = malloc(n * sizeof(int));
    int *motel_pos = malloc(n * sizeof(int));

    // Specjalna składnia dla alokacji "2D"
    // Alokujemy (n + 1) wskaźników do tablic [3] intów
    int (*L)[3] = malloc((n + 1) * sizeof(int[3]));
    
    // Alokujemy (n + 2), bo używamy indeksu R[n + 1]
    int (*R)[3] = malloc((n + 2) * sizeof(int[3]));

    // Sprawdzenie, czy alokacja się powiodła (dobra praktyka)
    if (motel_type == NULL || motel_pos == NULL || L == NULL || R == NULL) {
        fprintf(stderr, "Blad alokacji pamieci!\n");
        return 1; // Zakończ program z błędem
    }

    // --- Koniec alokacji ---


    // Wczytywanie danych do dwóch oddzielnych tablic
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &motel_type[i], &motel_pos[i]);
    }

    // Obliczanie tablicy L (najbliżsi po lewej)
    L[0][0] = L[0][1] = L[0][2] = large_neg;
    for (int i = 1; i <= n; i++) {
        L[i][0] = L[i - 1][0];
        L[i][1] = L[i - 1][1];
        L[i][2] = L[i - 1][2];
        L[i][motel_type[i - 1] - 1] = motel_pos[i - 1];
    }

    // Obliczanie tablicy R (najbliżsi po prawej)
    R[n + 1][0] = R[n + 1][1] = R[n + 1][2] = large_pos;
    for (int i = n; i > 0; i--) {
        R[i][0] = R[i + 1][0];
        R[i][1] = R[i + 1][1];
        R[i][2] = R[i + 1][2];
        R[i][motel_type[i - 1] - 1] = motel_pos[i - 1];
    }

    int minn = large_pos;
    // Obliczanie minimalnej odległości
    for (int i = 1; i <= n; i++) {
        int pos = motel_pos[i - 1];
        int type = motel_type[i - 1];
        
        if (type == 1) {
            minn = MIN(minn, MIN(MAX(pos - L[i][1], R[i][2] - pos), MAX(pos - L[i][2], R[i][1] - pos)));
        } else if (type == 2) {
            minn = MIN(minn, MIN(MAX(pos - L[i][0], R[i][2] - pos), MAX(pos - L[i][2], R[i][0] - pos)));
        } else { // type == 3
            minn = MIN(minn, MIN(MAX(pos - L[i][0], R[i][1] - pos), MAX(pos - L[i][1], R[i][0] - pos)));
        }
    }

    // Znajdowanie skrajnych pozycji dla każdego typu
    int firstA = large_pos, lastA = -1;
    int firstB = large_pos, lastB = -1;
    int firstC = large_pos, lastC = -1;

    for (int i = 0; i < n; i++) {
        int type = motel_type[i];
        int pos = motel_pos[i];
        
        if (type == 1) {
            firstA = MIN(firstA, pos);
            lastA = MAX(lastA, pos);
        } else if (type == 2) {
            firstB = MIN(firstB, pos);
            lastB = MAX(lastB, pos);
        } else { // type == 3
            firstC = MIN(firstC, pos);
            lastC = MAX(lastC, pos);
        }
    }

    int maxx = 0;
    // Obliczanie maksymalnej odległości
    for (int i = 0; i < n; i++) {
        int pos = motel_pos[i];
        int type = motel_type[i];

        if (type == 1) {
            if (firstB <= pos && lastC >= pos) maxx = MAX(maxx, MIN(pos - firstB, lastC - pos));
            if (firstC <= pos && lastB >= pos) maxx = MAX(maxx, MIN(pos - firstC, lastB - pos));
        } else if (type == 2) {
            if (firstA <= pos && lastC >= pos) maxx = MAX(maxx, MIN(pos - firstA, lastC - pos));
            if (firstC <= pos && lastA >= pos) maxx = MAX(maxx, MIN(pos - firstC, lastA - pos));
        } else { // type == 3
            if (firstB <= pos && lastA >= pos) maxx = MAX(maxx, MIN(pos - firstB, lastA - pos));
            if (firstA <= pos && lastB >= pos) maxx = MAX(maxx, MIN(pos - firstA, lastB - pos));
        }
    }

    // Wypisanie wyników
    printf("%d %d\n", minn, maxx);

    // --- Zwolnienie pamięci ---
    // Bardzo ważne jest, aby zwolnić pamięć, którą alokowaliśmy
    free(motel_type);
    free(motel_pos);
    free(L);
    free(R);
    // --- Koniec zwalniania ---

    return 0;
}
