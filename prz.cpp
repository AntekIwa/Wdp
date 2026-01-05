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
    long long num, den;
};

struct RemoveSmaller{ // Robi kolejkę rosnącą (Max)
    bool operator()(const PointVal& a, const PointVal& b) const { return a.val <= b.val; }
};

struct RemoveLarger { // Robi kolejkę rosnącą (Min)
    bool operator()(const PointVal& a, const PointVal& b) const { return a.val >= b.val; }
};

struct RemoveWorseQuality {
    bool operator()(const Segment& a, const Segment& b) const {
        // Porównujemy a < b
        // Jakość to a.num / a.den vs b.num / b.den
        // Sprowadzamy do: a.num * b.den < b.num * a.den
        return (unsigned __int128)a.num * b.den < (unsigned __int128)b.num * a.den;
    }
};




int main(){
    int n,U; cin >> n >> U;
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
            long long num = (punkty[r].first - punkty[l].first)*(punkty[r].first - punkty[l].first);
            long long den = r - l + 1;
            candidates.push_back({l, r, num, den});
        }
    }
    MonotonicDeque<Segment, RemoveWorseQuality> Q;
    int ptr = 0;
    for(int i = 0; i < n; i++){
        while(ptr < (int)candidates.size() && candidates[ptr].l <= i){
            Q.push(candidates[ptr]);
            ptr++;
        }
        while(!Q.empty() && Q.front().r < i) Q.pop_front();
        cout << Q.front().l + 1 << " " <<  Q.front().r + 1 << "\n";
    }
}
