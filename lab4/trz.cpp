#include<bits/stdc++.h>

using namespace std;

const int maxn = 1e6 + 7;
int L[maxn][3], R[maxn][3];
int main(){
    /*
     * iterujemy sie poo srodkowym, znajduejmy najblizszy na lewo typu 1/2/3, i na
     * prawo (liniowo) potem przechodizmy po i (srodkowy hotel) i liczymy wynik
     * To samo robimy dla najdalszych tyle ze szukamynajdlaszego w prawo/lewo typu 1/2/3
     */
    int n; cin >> n;
    vector<pair<int,int>> motel(n);
    for(auto &[x,y] : motel) cin >> x >> y;
    L[0][0] = L[0][1] = L[0][2] = -2e9;
    for(int i = 1; i <= n; i++){
        L[i][0] = L[i - 1][0];
        L[i][1] = L[i - 1][1];
        L[i][2] = L[i - 1][2];
        L[i][motel[i - 1].first - 1] = motel[i - 1].second;
    }
    R[n + 1][0] = R[n + 1][1] = R[n + 1][2] = 2e9;
    for(int i = n; i > 0; i--){
        R[i][0] = R[i + 1][0];
        R[i][1] = R[i + 1][1];
        R[i][2] = R[i + 1][2];
        R[i][motel[i - 1].first - 1] = motel[i - 1].second;
    }
    int minn = 2e9;
    for(int i = 1; i <= n; i++){
        if(motel[i - 1].first == 1){
            minn = min(minn, min(max(motel[i - 1].second - L[i][1], R[i][2] - motel[i - 1].second), max(motel[i - 1].second - L[i][2], R[i][1] - motel[i - 1].second)));
        }
        else if(motel[i - 1].first == 2){
            minn = min(minn, min(max(motel[i - 1].second - L[i][0], R[i][2] - motel[i - 1].second), max(motel[i - 1].second - L[i][2], R[i][0] - motel[i - 1].second)));
        }
        else{
            minn = min(minn, min(max(motel[i - 1].second - L[i][0], R[i][1] - motel[i - 1].second), max(motel[i - 1].second - L[i][1], R[i][0] - motel[i - 1].second)));
        }
    }
    int firstA = 2e9, lastA = -1;
    int firstB = 2e9, lastB = -1;
    int firstC = 2e9, lastC = -1;
    for(auto [x,y] : motel){
        if(x == 1){
            firstA = min(firstA, y);
            lastA = max(lastA, y);
        }
        else if(x == 2){
            firstB = min(firstB, y);
            lastB = max(lastB, y);
        }
        else{
            firstC = min(firstC, y);
            lastC = max(lastC, y);
        }
    }
    int maxx = 0;
    for(int i = 0; i < n; i++){
        int pos = motel[i - 1].second;
        if(motel[i - 1].first == 1){
            if(firstB <= pos && lastC >= pos) maxx = max(maxx, min(pos - firstB, lastC - pos));
            if(firstC <= pos && lastB >= pos) maxx = max(maxx, min(pos - firstC, lastB - pos));
        }
        else if(motel[i - 1].first == 2){
            if(firstA <= pos && lastC >= pos) maxx = max(maxx, min(pos - firstA, lastC - pos));
            if(firstC <= pos && lastA >= pos) maxx = max(maxx, min(pos - firstC, lastA - pos));
        }
        else{
            if(firstB <= pos && lastA >= pos) maxx = max(maxx, min(pos - firstB, lastA - pos));
            if(firstA <= pos && lastB >= pos) maxx = max(maxx, min(pos - firstA, lastB - pos));
        }
    }
    cout << minn << " " << maxx << "\n";
}
