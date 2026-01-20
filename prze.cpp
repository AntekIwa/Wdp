#include<bits/stdc++.h>

using namespace std;

// --- FUNKCJA HASZUJĄCA DLA WEKTORA ---
// Używamy hasza z StackOverflow (https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector)
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        size_t seed = 0;
        for (int val : v) {
            seed ^= hash<int>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

int n;
vector<int> caps;
vector<int> targets;

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0);

    cin >> n;

    caps.resize(n);
    targets.resize(n);

    for(int i = 0; i < n; i++) cin >> caps[i] >> targets[i];

    // jak nwd nie dzieli docelowych pojemnosci to sie nie da
    
    int total_gcd = caps[0];
    for (int i = 1; i < n; ++i) total_gcd = gcd(total_gcd, caps[i]);
    for (int i = 0; i < n; ++i) {
        if (total_gcd > 0 && targets[i] % total_gcd != 0) {
            cout << -1 << "\n";
            return 0;
        }
    }

    // Stan początkowy
    vector<int> start_node(n, 0);

    // Jeśli start jest celem
    if (start_node == targets) {
        cout << 0 << "\n";
        return 0;
    }

    // --- STRUKTURY DANYCH ---
    // Klucz: wektor stanów
    // Wartość: odległość
    // Hasher:  VectorHash z StackOverflow
    unordered_map<vector<int>, int, VectorHash> dist;
    queue<vector<int>> q;

    // Inicjalizacja BFS
    dist[start_node] = 0;
    q.push(start_node);

    // Zmienne pomocnicze, żeby nie alokować w pętli
    vector<int> next_node; 

    while (!q.empty()) {
        // Pobieramy stan z kolejki (const reference dla szybkości)
        const vector<int>& u = q.front();
        int d = dist[u];

        // Sprawdzamy czy to cel
        if(u == targets){
            cout << d << "\n";
            return 0;
        }
        
        // 1. Napełnianie
        for(int i = 0; i < n; i++){
            if (u[i] < caps[i]) {
                next_node = u;      // Kopia
                next_node[i] = caps[i]; // Zmiana
                
                // Jeśli nie odwiedzono -> dodaj
                if (dist.find(next_node) == dist.end()) {
                    dist[next_node] = d + 1;
                    q.push(next_node);
                }
            }
        }

        // 2. Opróżnianie
        for(int i = 0; i < n; i++){
            if(u[i] > 0){
                next_node = u;
                next_node[i] = 0;

                if(dist.find(next_node) == dist.end()){
                    dist[next_node] = d + 1;
                    q.push(next_node);
                }
            }
        }

        // 3. Przelewanie
        for(int i = 0; i < n; i++){
            if (u[i] == 0) continue;

            for(int j = 0; j < n; j++){
                if (i == j) continue;

                int space = caps[j] - u[j];
                if (space > 0) {
                    int amount = min(u[i], space); 
                    
                    next_node = u;
                    next_node[i] -= amount;
                    next_node[j] += amount;

                    if (dist.find(next_node) == dist.end()) {
                        dist[next_node] = d + 1;
                        q.push(next_node);
                    }
                }
            }
        }
        
        // Usuwamy przetworzony element z kolejki na końcu
        q.pop();
    }

    cout << -1 << "\n";
    return 0;
}
