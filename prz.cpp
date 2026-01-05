#include<bits/stdc++.h>

using namespace std;

template<typename T, typename Compare>
struct MonotonicDeque{
    deque<T> dq;
    Compare is_worse;
    void push(T val){
        while(!dq.empty() && is_worse(dq.back(), val)) dq.pop_back();
        dq.push_back(val);
    }
    T& front() { return dq.front(); }
    void pop_front() { dq.pop_front(); }
    bool empty() { return dq.empty(); }
    T& back() { return dq.back(); }
};

struct PointVal{
    long long val, idx;
};

struct Segment{
    int l, r;
    long long dx; // różnica x (xr - xl)
    long long len; // długość (r - l + 1)
};

struct RemoveSmaller{ 
    bool operator()(const PointVal& a, const PointVal& b) const { return a.val <= b.val; }
};

struct RemoveLarger { 
    bool operator()(const PointVal& a, const PointVal& b) const { return a.val >= b.val; }
};

struct RemoveWorseQuality {
    bool operator()(const Segment& a, const Segment& b) const {
        // Chcemy sprawdzić czy jakość A < jakość B.
        // Jakość to dx / sqrt(len).
        // Porównujemy kwadraty: dx^2 / len.
        // Czyli: a.dx^2 / a.len < b.dx^2 / b.len
        // Mnożąc na krzyż: a.dx^2 * b.len < b.dx^2 * a.len
        
        // Ponieważ wynik mnożenia (~10^24) przekracza long long, 
        // a nie mamy int128, używamy long double do obliczenia iloczynu.
        // Jest to bezpieczniejsze i szybsze niż używanie sqrt().
        return (long double)a.dx * a.dx * b.len < (long double)b.dx * b.dx * a.len;
    }
};

int main(){
    int n; 
    long long U; 
    cin >> n >> U;
    
    vector<pair<long long, long long>> punkty(n);
    for(auto &[x, y] : punkty) cin >> x >> y;
    
    MonotonicDeque<PointVal, RemoveSmaller> MaxQ;
    MonotonicDeque<PointVal, RemoveLarger> MinQ;

    int left = 0;
    vector<int> longest(n);

    for(int i = 0; i < n; i++){
        MaxQ.push({punkty[i].second, i});
        MinQ.push({punkty[i].second, i});
        
        while(!MaxQ.empty() && !MinQ.empty() && MaxQ.front().val - MinQ.front().val > U){
            left++;
            if(MaxQ.front().idx < left) MaxQ.pop_front();
            if(MinQ.front().idx < left) MinQ.pop_front();
        }
        longest[i] = i - left + 1;
    }

    vector<Segment> candidates;
    for(int i = 0; i < n; i++){
        if(i == n - 1 || longest[i + 1] != longest[i] + 1){
            int r = i;
            int l = i - longest[i] + 1;
            
            long long dx = punkty[r].first - punkty[l].first;
            long long len = r - l + 1;
            
            candidates.push_back({l, r, dx, len});
        }
    }

    MonotonicDeque<Segment, RemoveWorseQuality> Q;
    int ptr = 0;
    for(int i = 0; i < n; i++){
        // Dodajemy kandydatów
        while(ptr < (int)candidates.size() && candidates[ptr].l <= i){
            Q.push(candidates[ptr]);
            ptr++;
        }
        // Usuwamy nieaktywnych
        while(!Q.empty() && Q.front().r < i) Q.pop_front();
        
        if(!Q.empty()) {
            cout << Q.front().l + 1 << " " << Q.front().r + 1 << "\n";
        }
    }
    
    return 0;
}
