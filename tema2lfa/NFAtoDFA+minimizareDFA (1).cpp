#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>

class automat{
    int stare_init{};
    std::vector<std::string> alfabet;
    std::unordered_set<int> stari_finale;
    std::unordered_set<int> stari_total;
    virtual void display(std::ostream& os) const = 0;
protected:
    void addstari(int new_state){
        stari_total.insert(new_state);
    }

    explicit automat(int stare_int_, const  std::vector<std::string>& alfabet_,const std::unordered_set<int> &stariFinale, const  std::unordered_set<int>& stari_total_) :
    stare_init(stare_int_),
    alfabet(alfabet_),
    stari_finale(stariFinale),
    stari_total(stari_total_){}
    automat(const automat& other) = default;
    automat& operator=(const automat& other) = default;
    friend std::ostream &operator<<(std::ostream &os, const automat& other){
        os << "Starea initiala a automatului: " << other.stare_init << "\n";
        os << "Multimea de stari a automatului: ";
        for(const auto& stare: other.stari_total)
            os << stare << " ";
        os << "\n" << "Starile finale ale automatului: ";
        for(const auto& stare: other.stari_finale)
            os << stare << " ";
        os << "\n";
        other.display(os);
        return os;
    }
public:
    automat() {}
    int getstareint(){
        return stare_init;
    }
    std::vector<std::string> getteralfabet(){
        return alfabet;
    }
    std::unordered_set<int> getterstarifinale(){
        return stari_finale;
    };
    std::unordered_set<int> gettersatritotale(){
        return stari_total;
    }
    void setterstarifinale(std::unordered_set<int> s){
        stari_finale=s;
    }
    void setterstariinit(int i){
        stare_init=i;
    }
    void setterstaritotale(std::unordered_set<int> t){
        stari_total=t;
    }
    virtual ~automat()=default;
};

class NFA: public automat{
    std::vector<std::unordered_map<std::string, std::unordered_set<int>>> TranNFA;
    void display(std::ostream& os) const override{
        os << "Tranzitii NFA: \n";
        for(unsigned int i = 0; i < TranNFA.size(); i ++){
//            trnzitie->litera,multime de stari
            for(const auto& tranzitie : TranNFA[i]){
                os << "(" << i << "," << tranzitie.first << ") : ";
                for(const auto& stare: tranzitie.second)
                    std::cout << stare << " ";
                os << "\n";
            }
        }
    }
public:
    std::vector<std::unordered_map<std::string, std::unordered_set<int>>> gettertrannfa(){
        return TranNFA;
    }

    NFA(int stareInt, const std::vector<std::string> &alfabet, const std::unordered_set<int> &stariFinale,
        const std::unordered_set<int> &stariTotal,
        const std::vector<std::unordered_map<std::string, std::unordered_set<int>>> &tranNfa) : automat(stareInt,
                                                                                                        alfabet,
                                                                                                        stariFinale,
                                                                                                        stariTotal),
                                                                                                TranNFA(tranNfa) {}

    ~NFA() override {

    }

};
class DFA:public automat{
    std::vector<std::unordered_map<std::string, int>> TranDFA;
    void display(std::ostream& os) const override{
        os << "Tranzitii DFA: \n";
        for(unsigned int i = 0; i < TranDFA.size(); i ++){
            for(const auto&j : TranDFA[i]){
                os << "(" << i << "," << j.first << ") : " << j.second << "\n";
            }
        }
    }
public:
    DFA(int stareInt, const std::vector<std::string> &alfabet, const std::unordered_set<int> &stariFinale,
        const std::unordered_set<int> &stariTotal, const std::vector<std::unordered_map<std::string, int>> &tranDfa)
            : automat(stareInt, alfabet, stariFinale, stariTotal), TranDFA(tranDfa) {}
    DFA()=default;
    ~DFA() override {

    }
    static DFA NFA_to_DFA(NFA &nfa){
        int stare_initiala=0;
        std::vector<std::string> alfabet=nfa.getteralfabet();
        std::unordered_set<int> stariFinale=nfa.getterstarifinale();
        std::vector<std::unordered_map<std::string, std::unordered_set<int>>> tranNfa=nfa.gettertrannfa();
        std::vector<std::unordered_map<std::string,int>> tranzitiiDFA;
        std::unordered_set<int> toate_starile;
        std::unordered_set<int> stari_finale_dfa;
        std::unordered_map<int,std::unordered_set<int>> mapare_stari;
        std::queue<std::unordered_set<int>> unfeldebfs;
        unfeldebfs.push({stare_initiala});
        mapare_stari[0]={stare_initiala};
        int id=1;
        while(!unfeldebfs.empty()){
            std::unordered_set<int> multime_curenta= unfeldebfs.front();
            unfeldebfs.pop();
            for(const auto &a:alfabet){
                std::unordered_set<int> multime_rezultanta;
                for(const auto &m:multime_curenta){
                    for(const auto &t:tranNfa[m][a]){
                        multime_rezultanta.insert(t);
                    }
                }
                bool ok=false;
                int mapare_mul_rez,mapare_mul_cur;
                for(const auto &cauta:mapare_stari){
                    if(cauta.second==multime_rezultanta) {
                        ok = true;
                        mapare_mul_rez= cauta.first;
                    }
                    if(cauta.second==multime_curenta){
                        mapare_mul_cur=cauta.first;
                    }
                }
                tranzitiiDFA.resize(tranzitiiDFA.size()+1);
                if(ok){
                    tranzitiiDFA[mapare_mul_cur][a]=mapare_mul_rez;

                }
                else{
                    unfeldebfs.push(multime_rezultanta);
                    mapare_stari[id]=multime_rezultanta;
                    tranzitiiDFA[mapare_mul_cur][a]=id;
                    id++;
                }
            }
        }
        for(const auto &p:mapare_stari){
            toate_starile.insert(p.first);
            bool ok=false;
            for(const auto &Final:p.second){
                for(const auto &f:stariFinale)
                    if(f==Final)
                        ok=true;
            }
            if(ok){
                stari_finale_dfa.insert(p.first);
            }
        }
        return DFA{0,alfabet,stari_finale_dfa,toate_starile,tranzitiiDFA};
    }
    void complete_DFA(){
        std::unordered_set<int> stari = this->gettersatritotale();
        bool adaug = false;
        int stare_noua = stari.size();
        for(int i = 0; i < stari.size(); i++){
            for(auto& litera: this->getteralfabet()){
                bool ok = false;
                for(auto& l_tranz: TranDFA[i])
                    if(l_tranz.first == litera)
                        ok = true;
                if(!ok){
                    TranDFA[i][litera] = stare_noua;
                    adaug = true;
                }
            }
        }
        if(adaug){
            for(auto& litera: this->getteralfabet()){
                TranDFA[stare_noua][litera] = stare_noua;
            }
            this->addstari(stare_noua);
        }
    }
    void remove_unreachable_states (){
        std::queue<int> coada;
        std::vector<int> vizitati;
        vizitati.resize(this->gettersatritotale().size());
        vizitati[this->getstareint()] = 1;
        for(int i = 1; i < vizitati.size(); i++)
            vizitati[i] = 0;
        coada.push(this->getstareint());
        while(!coada.empty()){
            int stare_curenta = coada.front();
            coada.pop();
            for(auto& litera: this->getteralfabet()){
                if(TranDFA[stare_curenta].find(litera) != TranDFA[stare_curenta].end() and vizitati[TranDFA[stare_curenta][litera]] == 0){
                    vizitati[TranDFA[stare_curenta][litera]] = 1;
                    coada.push(TranDFA[stare_curenta][litera]);
                }
            }
        }
        std::unordered_set<int> new_multime_stari;
        std::unordered_set<int> new_stari_finale;
        for(int i = 0; i < vizitati.size(); i++){
            if(vizitati[i] == 1) {
                new_multime_stari.insert(i);
                bool ok = false;
                for(auto& F: this->getterstarifinale())
                    if(i == F)
                        ok = true;
                if(ok)
                    new_stari_finale.insert(i);
            }
            else{
                TranDFA[i].clear();
            }
        }
        this->setterstaritotale(new_multime_stari);
        this->setterstarifinale(new_stari_finale);
    }
    void remove_dead_states(){
        std::unordered_set<int> stari_nefinale;
        for(auto&stare : this->gettersatritotale()){
            bool este_finala = false;
            for(auto& F: this->getterstarifinale())
                if(F == stare)
                    este_finala = true;
            if(!este_finala)
                stari_nefinale.insert(stare);
        }
        std::unordered_set<int> new_multime_stari;
        for(auto& stare: stari_nefinale){
            bool este_dead_state = true;
            std::queue<int> coada;
            std::vector<int> vizitati;
            vizitati.resize(this->gettersatritotale().size());
            for(int& i: vizitati)
                i = 0;
            vizitati[stare] = 1;
            coada.push(stare);
            while(!coada.empty()){
                int stare_curenta = coada.front();
                coada.pop();
                for(auto& litera: this->getteralfabet()){
                    if(TranDFA[stare_curenta].find(litera) != TranDFA[stare_curenta].end() and vizitati[TranDFA[stare_curenta][litera]] == 0){
                        vizitati[TranDFA[stare_curenta][litera]] = 1;
                        coada.push(TranDFA[stare_curenta][litera]);
                    }
                }
            }
            for(int i = 0; i < vizitati.size(); i++){
                if(vizitati[i] == 1){
                    for(auto& F: this->getterstarifinale())
                        if(i == F)
                            este_dead_state = false;
                }
            }
            if(!este_dead_state)
                new_multime_stari.insert(stare);
            else{
               TranDFA[stare].clear();
                for(auto& stare_curenta : this->gettersatritotale()){
                    std::unordered_map<std::string,int> tranz;
                    for(auto& litera: this->getteralfabet()){
                        if(TranDFA[stare_curenta][litera] != stare)
                            tranz[litera] = TranDFA[stare_curenta][litera];
                    }
                    TranDFA[stare_curenta] = tranz;
                }
            }
        }
        for(auto& F: this->getterstarifinale())
            new_multime_stari.insert(F);
        this->setterstaritotale(new_multime_stari);
    }
    void minimize_DFA(){
        this->complete_DFA();
        this->remove_unreachable_states();
        std::unordered_set<int> stari_finale;
        std::unordered_set<int> stari_nefinale;
        for(const auto& stare: this->getterstarifinale())
            stari_finale.insert(stare);
        for(const auto& stare: this->gettersatritotale()){
            bool ok = false;
            for(const auto& F: stari_finale){
                if(F == stare){
                    ok = true;
                    break;
                }
            }
            if(!ok)
                stari_nefinale.insert(stare);
        }
        std::vector<std::unordered_set<int>> partitii_anterioare;
        std::vector<std::unordered_set<int>> partitii_curente;
        partitii_curente.push_back(stari_nefinale);
        partitii_curente.push_back(stari_finale);
        do{
            partitii_anterioare = partitii_curente;
            partitii_curente.clear();
            for(auto& partitie: partitii_anterioare){
                std::vector<std::unordered_set<int>> spargeri;
                spargeri.push_back({*partitie.begin()});
                for(auto& stare: partitie){
                    bool adaug_noua_spargere = true;
                    if(stare != *partitie.begin()){
                        for(auto& spargere_curenta: spargeri){
                            bool ok = true;
                            for(auto& litera: this->getteralfabet()){
                                int j = 0;
                                int cod_partitie_stare_spagere_curenta;
                                int cod_partitie_stare_partitie_mare;
                                for(auto& k: partitii_anterioare){
                                    for(auto& p: k){
                                        if(p == TranDFA[stare][litera])
                                            cod_partitie_stare_partitie_mare = j;
                                        if(p == TranDFA[*spargere_curenta.begin()][litera])
                                            cod_partitie_stare_spagere_curenta = j;
                                    }
                                    j++;
                                }
                                if(cod_partitie_stare_partitie_mare != cod_partitie_stare_spagere_curenta)
                                    ok = false;
                            }
                            if(ok){
                                spargere_curenta.insert(stare);
                                adaug_noua_spargere = false;
                            }
                        }
                        if(adaug_noua_spargere){
                            spargeri.push_back({stare});
                        }
                    }
                }
                for(auto& spargere: spargeri)
                    partitii_curente.push_back(spargere);
            }

        } while (partitii_curente != partitii_anterioare);
        std::unordered_set<int> new_multime_stari;
        std::unordered_set<int> new_stari_finale;
        std::vector<std::unordered_map<std::string,int>> new_tranzitii;
        std::unordered_map<int,std::unordered_set<int>> etichete;
        int i = 0;
        for(auto& partitie: partitii_anterioare){
            std::cout << "Nodul " << i << ": ";
            for(auto& elem: partitie)
                std::cout << elem << " ";
            std::cout << "\n";
            etichete[i] = partitie;
            new_multime_stari.insert(i);
            i++;
        }
        new_tranzitii.resize(partitii_anterioare.size());
        int new_stare_initiala;
        for(i = 0; i < partitii_anterioare.size(); i++){
            auto partitie = etichete[i];
            bool este_stare_finala = false;
            for(auto& stare: partitie) {
                if (stare == this->getstareint())
                    new_stare_initiala = i;
                for(auto& F: this->getterstarifinale())
                    if(stare == F)
                        este_stare_finala = true;
            }
            if(este_stare_finala)
                new_stari_finale.insert(i);
            for(auto& litera: this->getteralfabet()){
                int k;
                for(k = 0; k < partitii_anterioare.size(); k++){
                    auto multime = etichete[k];
                    for(auto& element: multime)
                        if(element == TranDFA[*partitie.begin()][litera])
                            new_tranzitii[i][litera] = k;
                }
            }
        }
        this->setterstariinit(new_stare_initiala);
        this->setterstarifinale(new_stari_finale);
        this->setterstaritotale(new_multime_stari);
        TranDFA= new_tranzitii;
        this->remove_dead_states();
        std::cout << *this;
    }

};
int main()
{  std::vector<std::string> alfabet = {"a","b"};
    std::unordered_set<int> stari_totale_nfa = {0,1,2};
    std::unordered_set<int> stari_finale_nfa = {2};
    std::unordered_set<int> stari_totale_dfa = {0,1,2,3,4,5,6};
    std::unordered_set<int> stari_finale_dfa = {6};
    std::vector<std::unordered_map<std::string,std::unordered_set<int>>> tranzitii_NFA;
    std::vector<std::unordered_map<std::string,int>> tranzitii_DFA;
    tranzitii_NFA.resize(3);
    tranzitii_NFA[0]["a"].insert(0);
    tranzitii_NFA[0]["b"].insert(0);
    tranzitii_NFA[0]["a"].insert(1);
    tranzitii_NFA[1]["a"].insert(1);
    tranzitii_NFA[1]["a"].insert(2);
    tranzitii_NFA[1]["b"].insert(1);
    tranzitii_NFA[2]["a"].insert(2);
    tranzitii_NFA[2]["b"].insert(2);
    NFA nfa{0,alfabet,stari_finale_nfa,stari_totale_nfa,tranzitii_NFA};
    std::cout << nfa << "\n";
    DFA dfa1;
    dfa1.NFA_to_DFA(nfa);
    std::cout<<dfa1<<"\n";
    tranzitii_DFA.resize(7);
    tranzitii_DFA[0]["a"] = 1;
    tranzitii_DFA[0]["b"] = 3;
    tranzitii_DFA[1]["a"] = 3;
    tranzitii_DFA[1]["b"] = 2;
    tranzitii_DFA[2]["b"] = 2;
    tranzitii_DFA[2]["a"] = 3;
    tranzitii_DFA[3]["b"] = 5;
    tranzitii_DFA[3]["a"] = 6;
    tranzitii_DFA[5]["a"] = 6;
    tranzitii_DFA[5]["b"] = 2;
    tranzitii_DFA[4]["b"] = 5;
    tranzitii_DFA[4]["a"] = 6;
    tranzitii_DFA[6]["a"] = 4;
    tranzitii_DFA[6]["b"] = 5;
    DFA dfa2{0,alfabet,stari_finale_dfa,stari_totale_dfa,tranzitii_DFA};
    std::cout << "\n" << dfa2 << "\n";
    dfa2.minimize_DFA();
    std::cout << dfa2;

    return 0;

}