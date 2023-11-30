#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <string>

std::ifstream fin  ("input.txt");
std::ofstream fout ("output.txt");
std::vector<std::pair<int, char> > Tran[100];
std::unordered_map<int, int> Dict;
std::vector<int> sfinal;
 void dfs(int nod_curent,std::string cuvant,bool & ok)
 {
    for(const auto &k: Tran[nod_curent]) {
        if (cuvant.size() != 0) {
            if (k.second == cuvant[0]) {
                std::string cuvi(cuvant.begin() + 1, cuvant.end());
                dfs(k.first, cuvi, ok);
            }
        }
    }
    if(cuvant.size()==0)
        for(const auto &l:sfinal)
            if(l==nod_curent)
                ok=true;

}

int main() {
    int N, x, M, y, s, nrf, nrcuv, stare_curenta, stare_urm;
    char l;
    bool ok;
    std::string c;
    fin >> N;
    Tran->resize(N);
    for (int i = 0; i < N; i++) {
        fin >> x;
        Dict[x] = i;
    }
    fin >> M;
    for (int i = 0; i < M; i++) {
        fin >> x >> y >> l;
        Tran[Dict[x]].push_back(std::make_pair(Dict[y], l));
    }

    fin >> s >> nrf;
    for (int i = 0; i < nrf; i++) {
        fin >> x;
        sfinal.push_back(Dict[x]);
    }
    fin >> nrcuv;
    for(int i=0;i<nrcuv;i++)
    {
        fin>>c;
        stare_curenta=Dict[s];
        ok=false;
        dfs(stare_curenta,c,ok);
        if(ok)
            fout<<"DA"<<"\n";
        else fout<<"NU"<<"\n";
    }
    return 0;
}