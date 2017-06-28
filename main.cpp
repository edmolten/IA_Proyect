#include <iostream>

#include <fstream>
#include <cstring>
#include <cstdlib>
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
        pair[M] = *ite;
        pair[W] = *ite;
        m->push_back(pair);
    }
    return m;
}

int * preference_values(int p, int match1, int match2, side * s){
    int i = 0;
    int * values = new int[2];
    values[0] = -1;
    values[1] = -1;
    bool done = false;
    for (side::iterator it_side = s->begin(); it_side != s->end(); it_side++){
        if(i+1 == p){
            int pref_pos = 0; // 0 is the most
            for (preference_list::iterator it_pref_list = (*it_side)->begin(); it_pref_list != (*it_side)->end(); it_pref_list++) {
                for (preference::iterator it_pref = (*it_pref_list)->begin(); it_pref != (*it_pref_list)->end(); it_pref++) {
                    if( *it_pref == match1){
                        values[0] = pref_pos;
                    }
                    else if(*it_pref == match2){
                        values[1] = pref_pos;
                    }
                    if(values[0] != -1 & values[1] != -1){
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

bool is_blocking_pair(int m, int m_match, int w, int w_match, everyone * mw) {
    int *m_preferences = preference_values(m, m_match, w, mw->m);
    if (m_preferences[0] == -1 || m_preferences[1] == -1) {
        return false;
    }
    int *w_preferences = preference_values(w, w_match, m, mw->w);
    if (w_preferences[0] == -1 || w_preferences[1] == -1) {
        return false;
    }
    bool men_prefers_other = m_preferences[0] > m_preferences[1];
    bool women_prefers_other = w_preferences[0] > w_preferences[1];
    return (men_prefers_other && women_prefers_other);
}

marriage * get_blocking_pairs_of_married(int p, int p_match, marriage *marr, int gerder, everyone *mw) {
    marriage * blocking_pairs = new marriage;
    for (marriage::iterator pair = marr->begin(); pair != marr->end(); pair++){
        if(gerder == M){
            int w = (*pair)[W];
            int w_match = (*pair)[M];
            if(is_blocking_pair(p,p_match,w,w_match,mw)){
                int * blocking_pair = new int[2];
                blocking_pair[M] = p;
                blocking_pair[W] = w;
                blocking_pairs->push_back(blocking_pair);
            }
        }
        else{
            int m = (*pair)[M];
            int m_match = (*pair)[W];
            if(is_blocking_pair(m,m_match, p, p_match, mw)){
                int * blocking_pair = new int[2];
                blocking_pair[M] = m;
                blocking_pair[W] = p;
                blocking_pairs->push_back(blocking_pair);
            }
        }

    }
}

marriage * get_all_blocking_pairs(marriage * marr, everyone * mw) {
    marriage *blocking_pairs = new marriage;
    set<int> *married_men = new set<int>;
    set<int> *married_women = new set<int>;
    for(marriage::iterator it = marr->begin(); it != marr->end(); it++) {
        int m = (*it)[M];
        married_men->insert(m);
        int w = (*it)[W];
        married_women->insert(w);
        get_blocking_pairs_of_married(m, w, marr, M, mw);
        get_blocking_pairs_of_married(w, m, marr, W, mw);
    }
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
int main() {
    srand((unsigned)time(NULL));
    const char * path = "C:\\Users\\eddox\\Documents\\GitHub\\IA_Proyect\\IA\\instances\\Chart1\\instance_3.txt";
    everyone * mw = parse_file(path);
    initial_solution(mw);
    /*
    marriage* m= new marriage;
    int *pair1 = new int[2];
    pair1[0] = 1;
    pair1[1] = 3; //(hombre 1 con mujer 3)
    m->push_back(pair1);*/

}