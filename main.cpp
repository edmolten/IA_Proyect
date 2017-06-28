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