#include <iostream>

#include <fstream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cmath>
#include "main.h"

using namespace std;

char * last_char(char * string){
    return &string[strlen(string)-1];
}

struct everyone * parse_file(const char * path){
    ifstream file;
    file.open(path);
    char word[6];
    side *men = new side;
    side *women = new side;
    side *current_side = men;
    file >> word; //skip first 1:
    preference_list *pref_list = new preference_list;
    while (file >> word) {
        if (strcmp(":", last_char(word)) == 0) {
            current_side->push_back(pref_list);
            if (strcmp("1:", word) == 0) {
                current_side = women;
            }
            pref_list = new preference_list;
        } else if (strcmp(word,"(") == 0 ) {
            preference *pref = new preference;
            while (file >> word) {
                if (strcmp(word,")") == 0 ) break;
                pref->insert(atoi(word));
            }
            pref_list->push_back(pref);
        } else {
            preference *pref = new preference;
            pref->insert(atoi(word));
            pref_list->push_back(pref);
        }
    }
    struct everyone * mw = new everyone;
    mw->m = men;
    mw->w = women;
    file.close();
    return mw;
}

void print_side(side * s){
    int i = 1;
    for (side::iterator it_side = s->begin(); it_side != s->end(); it_side++){
        cout << i << ": ";
        for (preference_list::iterator it_pref_list = (*it_side)->begin(); it_pref_list != (*it_side)->end(); it_pref_list++) {
            cout << "( ";
            for (preference::iterator it_pref = (*it_pref_list)->begin(); it_pref != (*it_pref_list)->end(); it_pref++) {
                cout << *it_pref << " ";
            }
            cout << ") ";
        }
        cout << '\n';
        i++;
    }
}

void print_all(struct everyone * mw){
    print_side(mw->m);
    print_side(mw->w);
}

double get_pcent() {
    return (double)rand() / (double)RAND_MAX ;
}

marriage * initial_solution(struct everyone * mw){
    unsigned long size = mw->m->size();
    set<int> indexes;
    marriage * m = new marriage;
    while(indexes.size() != size / 2){
        double pcent = get_pcent();
        int n = (int)(pcent * size);
        indexes.insert(n);
    }
    for(set<int>::iterator ite = indexes.begin(); ite != indexes.end(); ite++){
        int * pair = new int[2];
        pair[M] = *ite + 1;
        pair[W] = *ite + 1;
        m->push_back(pair);
    }
    return m;
}

set<int> * get_random_indexes(struct everyone *mw){
    set<int> * indexes = new set<int>();
    unsigned long size = mw->m->size();
    while(indexes->size() != size / 2){
        double pcent = get_pcent();
        int n = (int)(pcent * size);
        indexes->insert(n);
    }
    return  indexes;
}

marriage * initial_solution_2(struct everyone * mw){
    set<int> * woman_indexes = get_random_indexes(mw);
    set<int> * man_indexes = get_random_indexes(mw);
    marriage * m = new marriage;
    set<int>::iterator man_ite, woman_ite;
    for(man_ite = man_indexes->begin(), woman_ite = woman_indexes->begin();
            man_ite != man_indexes->end(), woman_ite != woman_indexes->end();
            man_ite++, woman_ite++){
        int * pair = new int[2];
        pair[M] = *man_ite + 1;
        pair[W] = *woman_ite + 1;
        m->push_back(pair);
    }
    return m;

}

int * preference_values(int p, int match1, int match2, side * p_side){
    int i = 1;
    int * values = new int[2];
    values[0] = INT_MAX;
    values[1] = INT_MAX;
    bool done = false;
    for (side::iterator it_side = p_side->begin(); it_side != p_side->end(); it_side++){
        if(i == p){
            int pref_pos = 0; // 0 is the most
            for (preference_list::iterator it_pref_list = (*it_side)->begin(); it_pref_list != (*it_side)->end(); it_pref_list++) {
                for (preference::iterator it_pref = (*it_pref_list)->begin(); it_pref != (*it_pref_list)->end(); it_pref++) {
                    if( *it_pref == match1){
                        values[0] = pref_pos;
                    }
                    if(*it_pref == match2){
                        values[1] = pref_pos;
                    }
                    if(values[0] != INT_MAX & values[1] != INT_MAX){
                        done = true;
                        break;
                    }
                }
                if (done){
                    break;
                }
                pref_pos++;
            }
            break;
        }
        i++;
    }
    return values;
}

bool is_blocking_pair(int m1, int w1, int m2, int w2, everyone * mw) {
    int *m_preferences = preference_values(m1, w1, w2, mw->m);
    int *w_preferences = preference_values(w2, m1, m2, mw->w);
    bool m1_prefers_w2 = m_preferences[0] > m_preferences[1];
    bool w2_prefers_m1 = w_preferences[0] < w_preferences[1];
    return (m1_prefers_w2 && w2_prefers_m1);
}

marriage * get_blocking_pairs_of_married(int p, int p_match, marriage *marr, int p_gerder, everyone *mw) {
    marriage * blocking_pairs = new marriage;
    for (marriage::iterator pair = marr->begin(); pair != marr->end(); pair++){
        if(p_gerder == M){
            int m2 = (*pair)[M];
            int w2 = (*pair)[W];
            if(is_blocking_pair(p,p_match,m2,w2,mw)){
                int * blocking_pair = new int[2];
                blocking_pair[M] = p;
                blocking_pair[W] = w2;
                blocking_pairs->push_back(blocking_pair);
            }
        }
        else{
            int m2 = (*pair)[M];
            int w2 = (*pair)[W];
            if(is_blocking_pair(m2,w2, p_match, p, mw)){
                int * blocking_pair = new int[2];
                blocking_pair[M] = m2;
                blocking_pair[W] = p;
                blocking_pairs->push_back(blocking_pair);
            }
        }

    }
    return blocking_pairs;
}

void add_to_list(marriage * l1, marriage * l2){
    for(marriage::iterator it = l2->begin(); it != l2->end(); it++){
        l1->push_back(*it);
    }
}

void blocking_pairs_from_matchs(marriage *blocking_pairs, set<int> *married_men, set<int> *married_women,
                                marriage *marr, everyone *mw) {
    for (marriage::iterator it = marr->begin(); it != marr->end(); it++) {
        int m = (*it)[M];
        married_men->insert(m);
        int w = (*it)[W];
        married_women->insert(w);
        add_to_list(blocking_pairs, get_blocking_pairs_of_married(m, w, marr, M, mw));
    }
}

set<int> * get_singles(set<int> * married, unsigned long size){
    set<int> * singles = new set<int>;
    set<int>::iterator it = married->begin();
    for(int i = 1; i <= size; i++) {
        if ((*it) == i) {
            it++;
        } else {
             singles->insert(i);
        }
    }
    return singles;
}

bool acepts(int p, int p2, side *p_side) {
    int i = 0;
    for (side::iterator it_side = p_side->begin(); it_side != p_side->end(); it_side++){
        if(i+1 == p){
            for (preference_list::iterator it_pref_list = (*it_side)->begin(); it_pref_list != (*it_side)->end(); it_pref_list++) {
                for (preference::iterator it_pref = (*it_pref_list)->begin(); it_pref != (*it_pref_list)->end(); it_pref++) {
                    if (*it_pref == p2) {
                        return true;
                    }
                }
            }
        }
        i++;
    }
    return false;
}

bool is_single_blocking_pair(int p, int p_match, int single, int p_gerder, everyone * mw){
    int * p_preferences;
    bool single_acepts_p;
    if(p_gerder == M){
        p_preferences = preference_values(p, p_match, single, mw->m);
        single_acepts_p = acepts(single,p,mw->w);
    }
    else{
        p_preferences = preference_values(p, p_match, single, mw->w);
        single_acepts_p = acepts(single,p,mw->m);
    }
    bool p_prefers_single = p_preferences[0] > p_preferences[1];
    return p_prefers_single && single_acepts_p;
}

void blocking_pairs_one_side(set<int> * singles, marriage *marr, everyone *mw, marriage *blocking_pairs, int single_gerder) {
    for(set<int>::iterator it = singles->begin(); it != singles->end(); it++) {
        for(marriage::iterator it2 = marr->begin(); it2 != marr->end(); it2++) {
            int m = (*it2)[M];
            int w = (*it2)[W];
            if(single_gerder == M) {
                if (is_single_blocking_pair(w, m, (*it), W, mw)) {
                    int *pair = new int[2];
                    pair[M] = (*it);
                    pair[W] = w;
                    blocking_pairs->push_back(pair);
                }
            }
            else {
                if (is_single_blocking_pair(m, w, (*it), M, mw)) {
                    int *pair = new int[2];
                    pair[M] = m;
                    pair[W] = (*it);
                    blocking_pairs->push_back(pair);
                }
            }
        }
    }
}

void blocking_pairs_mix_singles(set<int> *single_men, set<int> *single_women, everyone *mw, marriage * blocking_pairs) {
    for(set<int>::iterator men_it = single_men->begin(); men_it != single_men->end();men_it++) {
        for(set<int>::iterator women_it = single_women->begin(); women_it != single_women->end();women_it++) {
            int m = (*men_it);
            int w = (*women_it);
            bool woman_acepts_man = acepts(w,m,mw->w);
            bool man_acepts_woman = acepts(m,w,mw->m);
            if(woman_acepts_man && man_acepts_woman){
                int * pair = new int[2];
                pair[M] = m;
                pair[W] = w;
                blocking_pairs->push_back(pair);
            }
        }
    }
}

void print_single(set<int> *singles){
    for(set<int>::iterator ite = singles->begin(); ite != singles->end(); ite++){
        cout << *ite << " ";
    }
    cout << endl;
}

void blocking_pairs_from_singles(marriage *blocking_pairs, set<int> *married_men, set<int> *married_women,
                                 marriage *marr, everyone *mw, unsigned long * singles_number) {
    unsigned long size = mw->m->size();
    set<int> * single_men = get_singles(married_men,size);
    set<int> * single_women = get_singles(married_women,size);
    /*
    cout << "Single men: ";
    print_single(single_men);
    cout << "Single women: ";
    print_single(single_women);
    */
     if(singles_number != NULL){
        *singles_number = single_men->size() + single_women->size();
    }
    blocking_pairs_one_side(single_men,marr,mw,blocking_pairs,M);
    blocking_pairs_one_side(single_women,marr,mw,blocking_pairs,W);
    blocking_pairs_mix_singles(single_men,single_women,mw,blocking_pairs);
}

marriage * get_all_blocking_pairs(marriage * marr, everyone * mw, unsigned long * singles_number) {
    marriage *blocking_pairs = new marriage;
    set<int> *married_men = new set<int>;
    set<int> *married_women = new set<int>;
    blocking_pairs_from_matchs(blocking_pairs, married_men, married_women, marr, mw);
    blocking_pairs_from_singles(blocking_pairs,married_men,married_women,marr,mw, singles_number);
    return blocking_pairs;
}

marriage * copy_marriage_with_breakups(marriage *marr, int m, int w){
    marriage * new_marriage = new marriage;
    for(marriage::iterator it = marr->begin();it != marr->end();it++){
        int * pair = new int[2];
        int new_m = (*it)[M];
        int new_w = (*it)[W];
        if(new_m != m && new_w != w){
            pair[M] = new_m;
            pair[W] = new_w;
            new_marriage->push_back(pair);
        }
    }
    return new_marriage;
}

marriage * random_movement(marriage * blocking_pairs, marriage * marr){
    unsigned long size = blocking_pairs->size();
    double pcent = get_pcent();
    int rand_index = (int) (pcent * size);
    int index = 0;
    int * blocking_pair = new int[2];
    int m ;
    int w;
    marriage * new_marriage;
    for (marriage::iterator it = blocking_pairs->begin(); it != blocking_pairs->end(); it++) {
        if (index == rand_index) {
            m = (*it)[M];
            w = (*it)[W];
            blocking_pair[M] = m;
            blocking_pair[W] = w;
            cout << "Randomly selected blocking pair: (" << m << " , " << w << ")" << endl;
            new_marriage = copy_marriage_with_breakups(marr, m, w);
            new_marriage->push_back(blocking_pair);
            break;
        }
        index++;
    }
    return  new_marriage;
}

void print_marriage(marriage * marr){
    for(marriage::iterator it = marr->begin();it != marr->end();it++) {
        cout << "(" << (*it)[0] << " , " << (*it)[1] << ") ";
    }
    cout << endl;
}

void check_repetition(marriage * marr){
    for(marriage::iterator it = marr->begin();it != marr->end();it++){
        int i = 0;
        int m1 = (*it)[M];
        int w1 = (*it)[W];
        for(marriage::iterator it2 = marr->begin();it2 != marr->end();it2++) {
            int m2 = (*it2)[M];
            int w2 = (*it2)[W];
            if (m1 == m2 && w1 == w2){
                i++;
            }
        }
        cout<< i << " ";
    }
}

marriage * copy_marriage(marriage * marr ){
    marriage * new_marriage = new marriage;
    for(marriage::iterator it = marr->begin();it != marr->end();it++){
        int * pair = new int[2];
        int new_m = (*it)[M];
        int new_w = (*it)[W];
        pair[M] = new_m;
        pair[W] = new_w;
        new_marriage->push_back(pair);
    }
    return new_marriage;
}

void free_marriage(marriage * marr ){
    for(marriage::iterator it = marr->begin();it != marr->end();it++){
        delete *it;

    }
    marr->clear();
}

long evaluation_function(marriage * marr, everyone * mw) {
    unsigned long * singles_number_p;
    unsigned long singles_number;
    singles_number_p = &singles_number;
    marriage * blocking_pairs = get_all_blocking_pairs(marr, mw,singles_number_p);
    unsigned long blocking_pairs_number = blocking_pairs->size();
    long score = (mw->m->size()/3)*blocking_pairs_number + singles_number;
    cout << "Blocking pairs number: " << blocking_pairs_number;
    cout << ", Singles number: " << singles_number;
    cout << ", Score: " << score << endl;
    return score;
}

bool random_acept(long delta_score, double q){
    bool acept = exp(delta_score/q) >= get_pcent();
    if(acept){
        cout << "Rondom acepted" << endl;
    }
    return acept;
}

int main() {
    srand((unsigned)time(NULL));
    const char * path = "C:\\Users\\eddox\\Documents\\GitHub\\IA_Proyect\\IA\\instances\\Chart1\\instance_3.txt";
    everyone * mw = parse_file(path);
    marriage * marr = initial_solution_2(mw);
    cout << "Initial marriage: ";
    print_marriage(marr);
    marriage * best_marr = copy_marriage(marr);
    /*cout << "Marriage: " << endl;
    print_marriage(marr);
    marriage * all_blocking_pairs = get_all_blocking_pairs(marr,mw);
    if(all_blocking_pairs != NULL){
        cout << "Blocking pairs: " << endl;
        print_marriage(all_blocking_pairs);
        cout << "Number of blocking pairs: " << all_blocking_pairs->size() << endl;
        //check_repetition(all_blocking_pairs);
        marriage * new_marr = random_movement(all_blocking_pairs, marr);
        cout << "New marriage: " << endl;
        print_marriage(new_marr);
    }
    else{
        cout << "No new marriage";
    }*/
    int T_MAX = 10000;
    long best_score = evaluation_function(best_marr, mw);
    marriage * new_marr;
    long score;
    long delta_score;
    marriage * all_blocking_pairs;
    for(int t = 0; t<T_MAX; t++){
        cout << "#" << t << endl;
        all_blocking_pairs = get_all_blocking_pairs(marr,mw,NULL);
        if(all_blocking_pairs->size() == 0){
            cout << "No blocking pairs" << endl;
            new_marr = initial_solution_2(mw);
            cout << "New random marriage: ";
            print_marriage(new_marr);
        }
        else{
            cout << "Blocking pairs: ";
            print_marriage(all_blocking_pairs);
            new_marr = random_movement(all_blocking_pairs, marr);
            cout << "New marriage: ";
            print_marriage(new_marr);
        }
        free_marriage(all_blocking_pairs);
        score = evaluation_function(new_marr,mw);
        delta_score = best_score - score;
        cout << "Delta score: " << delta_score << endl;
        if(delta_score >= 0 || random_acept(delta_score, mw->m->size())){
            free_marriage(marr);
            marr = copy_marriage(new_marr);
        }
        if(delta_score >= 0){
            cout << "New marriage is better!" << endl;
            free_marriage(best_marr);
            best_marr = copy_marriage(marr);
            best_score = score;
            if(best_score == 0){
                break;
            }
        }
        //teperature()
    }
    cout << endl << "Best found: ";
    print_marriage(best_marr);
}
/*
SA

 init:
 X = initial solution
 tmax = max iterations
 q = initial temperature
 B = X (best solution)
 n = 0 number of solution

 if (t == max){
    break;
 }
 if(no_posible_movement(X)){
    X = random
 }
 else{
    nueva X = random_movement(X)
 }
 calculate_delta(X)
 if(is_better || random_acept){
    X = nueva X
 }
 else continue;
 if(is better){
    B = X
 }
 try_reduce_temperatue
 t = t+1
 */