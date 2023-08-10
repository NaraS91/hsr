#include <iostream>
#include <random>
#include <chrono>
#include <unordered_map>
using namespace std;

typedef uint_fast32_t relic_t;

enum Substats { HP, HP_P, ATK, ATK_P, DEF, DEF_P, EF_RES, EHR, BREAK, CR, CD, SPD };
unordered_map<string, Substats> main_to_enum = { {"HP", HP}, {"HP_P", HP_P}, {"ATK", ATK}, {"ATK_P", ATK_P}, {"DEF", DEF}, {"DEF_P", DEF_P}, {"EHR", EHR}, {"BREAK", BREAK}, {"CR", CR}, {"CD", CD}, {"SPD", SPD} };

int stat_prob[12] = {10,10,10,10,10,10,8,8,8,6,6,4};

relic_t* simulate(long n, int main_stat){
  srand(42);
  double stat_cum_prob[12] = {10,20,30,40,50,60,68,76,84,90,96,100};
  relic_t* relics = (relic_t*) calloc(n, sizeof(relic_t));
  for (int i = 0; i < n; i++){
    for(int j = 0; j < 4; j++){
      int r = rand() % 100;
      int k = 0;
      // transition from r to k here is manually engineered based on stat_cum_prob.
      // if probabilities change, this part will be depricated
      // either do simple while(r >= stat_cum_prob[k]) k++; or derive a similar formula
      if (r <= 67)
        k = r / 10;
      else if (r <= 89){
        k = (r - 60) / 8 + 6;
      } else {
        k = 10 + (r - 90) / 6;
      }
      if (relics[i] & 2 << k || k == main_stat)
        j--;
      else
        relics[i] = relics[i] | 2 << k;
    }
    if (i % 10000000 == 0){
      cout << "simulation at the " << i+1 << "th relic\n";
    }
  }

  return relics;
}


relic_t double_crit_m = 2 << CR | 2 << CD;
relic_t spd_m = 2 << SPD;
relic_t spd_ehr_m = spd_m | 2 << EHR;
relic_t spd_atk_m = spd_m | 2 << ATK_P;
relic_t spd_defp_m = spd_m | 2 << DEF_P;
relic_t spd_hpp_m = spd_m | 2 << HP_P;
relic_t spd_be_m = spd_m | 2 << BREAK;
relic_t spd_atk_ehr_m = spd_atk_m | 2 << EHR;
relic_t atk_m = 2 << ATK_P;
relic_t ehr_m = 2 << EHR;
relic_t atk_ehr_m = atk_m | ehr_m;

void analize(relic_t* relics, int n){
  cout << "analizing...\n";
  int single_crit = 0;
  int single_crit_spd = 0;
  int single_crit_spd_atk = 0;
  int single_crit_spd_ehr = 0;
  int single_crit_spd_atk_ehr = 0;
  int single_crit_atk = 0;
  int single_crit_ehr = 0;
  int single_crit_atk_ehr = 0;
  int double_crit = 0;
  int double_crit_spd = 0;
  int double_crit_spd_atk = 0;
  int double_crit_spd_ehr = 0;
  int double_crit_atk = 0;
  int double_crit_ehr = 0;
  int double_crit_atk_ehr = 0;
  int spd = 0;
  int spd_be = 0;
  int spd_hpp = 0;
  int spd_defp = 0;

  for(int i = 0; i < n; i++){
    int relic = relics[i];
    if ((relic & spd_m) == spd_m) spd++;
    if ((relic & spd_hpp_m) == spd_hpp_m) spd_hpp++;
    if ((relic & spd_defp_m) == spd_defp_m) spd_defp++;
    if ((relic & spd_be_m) == spd_be_m) spd_be++;
    if(((relic & double_crit_m) != double_crit_m) &&
        relic & (2 << CR | 2 << CD)){
      single_crit++;
      if (relic & spd_m) single_crit_spd++;
      if ((relic & spd_atk_m) == spd_atk_m) single_crit_spd_atk++;
      if ((relic & spd_ehr_m) == spd_ehr_m) single_crit_spd_ehr++;
      if ((relic & spd_atk_ehr_m) == spd_atk_ehr_m) single_crit_spd_atk_ehr++;
      if ((relic & atk_m) == atk_m) single_crit_atk++;
      if ((relic & ehr_m) == ehr_m) single_crit_ehr++;
      if ((relic & atk_ehr_m) == atk_ehr_m) single_crit_atk_ehr++;
    } else if ((relic & double_crit_m) == double_crit_m){
      double_crit++;
      if (relic & spd_m) double_crit_spd++;
      if ((relic & spd_atk_m) == spd_atk_m) double_crit_spd_atk++;
      if ((relic & spd_ehr_m) == spd_ehr_m) double_crit_spd_ehr++;
      if ((relic & atk_m) == atk_m) double_crit_atk++;
      if ((relic & ehr_m) == ehr_m) double_crit_ehr++;
      if ((relic & atk_ehr_m) == atk_ehr_m) double_crit_atk_ehr++;
    }
  }

  cout << n << " samples\n\n"
    << "single crit: " << (double) single_crit / n << endl
    << "with spd: " << (double) single_crit_spd / n << endl
    << "with spd & atk: " << (double) single_crit_spd_atk / n << endl
    << "with spd & ehr: " << (double) single_crit_spd_ehr / n << endl
    << "with spd & ehr & atk: " << (double) single_crit_spd_atk_ehr / n << endl
    << "with atk: " << (double) single_crit_atk / n << endl
    << "with ehr: " << (double) single_crit_ehr / n << endl
    << "with atk & ehr: " << (double) single_crit_atk_ehr / n << endl << endl
    << "double crit: " << (double) double_crit / n << endl
    << "with spd: " << (double) double_crit_spd / n << endl
    << "with spd & atk: " << (double) double_crit_spd_atk / n << endl
    << "with spd & ehr: " << (double) double_crit_spd_ehr / n << endl
    << "with atk: " << (double) double_crit_atk / n << endl
    << "with ehr: " << (double) double_crit_ehr / n << endl
    << "with atk & ehr: " << (double) double_crit_atk_ehr / n << endl
    << "spd: " << (double) spd / n << endl
    << "spd be: " << (double) spd_be / n << endl
    << "spd defp: " << (double) spd_defp / n << endl
    << "spd hpp: " << (double) spd_hpp / n << endl;
  
  free(relics);
}

int main(){
  long n;
  cout << "input number of samples\n";
  cin >> n;
  cout << "input main stat\n";
  string main_stat;
  cin >> main_stat;
  int m_stat = main_to_enum.find(main_stat) != main_to_enum.end() ? main_to_enum[main_stat] : 20;
  relic_t *relics = simulate(n, m_stat);
  analize(relics, n);

  return 0;
}
