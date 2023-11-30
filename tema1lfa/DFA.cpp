#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <utility>

std::ifstream fin  ("input.txt");
std::ofstream fout ("output.txt");

int main() {
    int N,x,M,y,s,nrf,nrcuv,stare_curenta,stare_urm;
    char l;
    std::string c;
    fin>>N;
    std::vector <std:: pair < int,char> > Tran[N];
    std::unordered_map <int, int> Dict;
    std::vector<int>sfinal;
    for(int i =0; i<N; i++) {
        fin >> x;
        Dict[x]=i;
    }
    fin>>M;
    for(int i=0;i<M;i++)
    {
        fin>>x>>y>>l;
        Tran[Dict[x]].push_back(std::make_pair(Dict[y],l));
    }
    fin>>s>>nrf;
    for(int i=0;i<nrf;i++)
    {
        fin>>x;
        sfinal.push_back(Dict[x]);
    }
    fin>>nrcuv;
    for(int i=0;i<nrcuv;i++)
    {
        fin>>c;
        stare_curenta=Dict[s];
        for(int j=0;j<c.size();j++)
        {
            for(const auto &l : Tran[stare_curenta])
            {
                if(l.second==c[j])
                {
                    stare_urm=l.first;
                    break;
                }
            }
            stare_curenta=stare_urm;
        }
        bool ok=true;
        for(const auto &k : sfinal)
            if(stare_curenta==k)
            {
                fout<<"DA"<<"\n";
                ok= false;
                break;
            }
        if(ok)
            fout<<"NU"<<"\n";
    }
    fin.close();
    fout.close();
    return 0;
}
