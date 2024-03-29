#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cmath>
#include "main.h"

using namespace std;

// ALL THE DEBUG PRINTS ARE COMMENTED OUT. UNCOMMENT FOR VERBOSE OUTPUT (REALLY LONG)

// Gets the last character of a string
char * last_char(char * string){
    size_t index = strlen(string)-1;
    return &string[index];
}

// Frees all memory allocated in the marriage
void free_marriage(marriage * marr ){
    for(marriage::iterator it = marr->begin();it != marr->end();it++){
        delete *it;
    }
    marr->clear();
    delete marr;
}

// Parses file and fill the everyone structure
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
            // if another "1:" is read, change side. Line jump is irrelevant
            if (strcmp("1:", word) == 0) {
                current_side = women;
            }
            pref_list = new preference_list;
        } else if (strcmp(word,"(") == 0 ) { //tie slot, begin sub iterations
            preference *pref = new preference;
            while (file >> word) {
                if (strcmp(word,")") == 0 ) break;
                pref->insert(atoi(word));
            }
            pref_list->push_back(pref);
        } else { //single preference slot
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

// Prints one side in a similar way as the file, but the single preferences are between parenthesis too
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

// Prints both sides
void print_all(struct everyone * mw){
    print_side(mw->m);
    print_side(mw->w);
}

// Gets a random number between 0 and 1
double get_pcent() {
    return (double)rand() / (double)RAND_MAX ;
}

// Gets a set of random integers from 0 to the size of one side
// The size is the same as one side
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

// Generates a random marriage of lenght equal to one side
marriage * initial_random_marriage(struct everyone *mw){
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
    man_indexes->clear();
    delete man_indexes;
    woman_indexes->clear();
    delete woman_indexes;
    return m;

}

// Returns the preference position of p for match1 and for match2 in p_side,
// in an array of two ints. Greater numbers means less preference
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

// Returns if (m1,w2) is blocking pair. w1 is m1 match and m2 is w2 match.
bool is_blocking_pair(int m1, int w1, int m2, int w2, everyone * mw) {
    int *m_preferences = preference_values(m1, w1, w2, mw->m);
    int *w_preferences = preference_values(w2, m1, m2, mw->w);
    bool m1_prefers_w2 = m_preferences[0] > m_preferences[1];
    bool w2_prefers_m1 = w_preferences[0] < w_preferences[1];
    delete m_preferences;
    delete w_preferences;
    return (m1_prefers_w2 && w2_prefers_m1);
}

// Get all blocking pairs related with match (p, p_match) (or (p_match, p) depending of p_gerder)
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

// Copy and appends elements of l2 to l1
void add_to_list_by_copy(marriage *l1, marriage *l2){
    for(marriage::iterator it = l2->begin(); it != l2->end(); it++){
        int m = (*it)[M];
        int w = (*it)[W];
        int * pair = new int[2];
        pair[M] = m;
        pair[W] = w;
        l1->push_back(pair);
    }
}

// Gets all the blocking pairs obtained from checking all the matches in marr, and appends them to blocking_pairs.
// Also stores the married men and women in married_men and married_women respectively
void blocking_pairs_from_matchs(marriage *blocking_pairs, set<int> *married_men, set<int> *married_women,
                                marriage *marr, everyone *mw) {
    for (marriage::iterator it = marr->begin(); it != marr->end(); it++) {
        int m = (*it)[M];
        married_men->insert(m);
        int w = (*it)[W];
        married_women->insert(w);
        marriage * blocking_pairs_from_married = get_blocking_pairs_of_married(m, w, marr, M, mw);
        add_to_list_by_copy(blocking_pairs,blocking_pairs_from_married);
        free_marriage(blocking_pairs_from_married);
    }
}

// Returns a set with all the singles, based on the size of one side and a set with all the currently engaged
// ones of one gerder,
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

// Returns whether a match between p and p2 is factible
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

// Returns whether (p, single) (or (single, p) depending on p_gerder) is a blocking pair, knowing  p's match
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

// Gets all blocking pairs related with singles, of gerder single_gerder, in marr, and appends them to blocking_pairs
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

// Gets all the blocking pairs related to all singles in both sides by checking just their mutual acceptance
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

// Prints the set
void print_singles(set<int> *singles){
    for(set<int>::iterator ite = singles->begin(); ite != singles->end(); ite++){
        cout << *ite << " ";
    }
    cout << endl;
}

// Gets the number of singles in singles, of gerder gerder, that don't participate in an blocking pair in blocking_pairs
unsigned long get_not_blocking_singles(set<int> * singles, int gerder, marriage * blocking_pairs){
    unsigned long total = 0;
    bool is_blocking_single;
    int single;
    for(set<int>::iterator single_it = singles->begin(); single_it != singles->end(); single_it++){
        single = *single_it;
        is_blocking_single = false;
        for(marriage::iterator blocking_pair_it = blocking_pairs->begin(); blocking_pair_it != blocking_pairs->end(); blocking_pair_it++) {
            int p = (*blocking_pair_it)[gerder];
            if( p == single) {
                is_blocking_single = true;
                break;
            }
        }
        if(!is_blocking_single){
            total++;
        }
    }
    return total;
}

// Gets all the blocking pairs related to the sinlges in marr. Optionally stores the number of singles in single_number
void blocking_pairs_from_singles(marriage *blocking_pairs, set<int> *married_men, set<int> *married_women,
                                 marriage *marr, everyone *mw, unsigned long * singles_number) {
    unsigned long size = mw->m->size();
    set<int> * single_men = get_singles(married_men,size);
    set<int> * single_women = get_singles(married_women,size);
     if(singles_number != NULL){
        *singles_number = get_not_blocking_singles(single_men, M, blocking_pairs) + get_not_blocking_singles(single_women,W,blocking_pairs);
    }
    blocking_pairs_one_side(single_men,marr,mw,blocking_pairs,M);
    blocking_pairs_one_side(single_women,marr,mw,blocking_pairs,W);
    blocking_pairs_mix_singles(single_men,single_women,mw,blocking_pairs);
    single_men->clear();
    delete single_men;
    single_women->clear();
    delete single_women;
}

// Gets all locking pairs of marr
marriage * get_all_blocking_pairs(marriage * marr, everyone * mw, unsigned long * singles_number) {
    marriage *blocking_pairs = new marriage;
    set<int> *married_men = new set<int>;
    set<int> *married_women = new set<int>;
    blocking_pairs_from_matchs(blocking_pairs, married_men, married_women, marr, mw);
    blocking_pairs_from_singles(blocking_pairs,married_men,married_women,marr,mw, singles_number);
    married_men->clear();
    delete married_men;
    married_women->clear();
    delete married_women;
    return blocking_pairs;
}

//Copy marrige marr, without (m, w) match
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

//Performs the movement by eliminating a random blocking pair of blocking_pairs from marr
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
            //cout << "Randomly selected blocking pair: (" << m << " , " << w << ")" << endl;
            new_marriage = copy_marriage_with_breakups(marr, m, w);
            new_marriage->push_back(blocking_pair);
            break;
        }
        index++;
    }
    return  new_marriage;
}

// Prints the marriage
void print_marriage(marriage * marr){
    for(marriage::iterator it = marr->begin();it != marr->end();it++) {
        //cout << "(" << (*it)[0] << " , " << (*it)[1] << ") ";
    }
    //cout << endl;
}

// Check if there is a duplicated pair in marr
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
        //cout<< i << " ";
    }
}

// Copy the marriage
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

// Sums the number of singles and the number of blocking pairs in marr
long evaluation_function(marriage * marr, everyone * mw, unsigned long * blocking_pairs_number_out) {
    unsigned long singles_number;
    marriage * blocking_pairs = get_all_blocking_pairs(marr, mw,&singles_number);
    unsigned long blocking_pairs_number = blocking_pairs->size();
    if(blocking_pairs_number_out != NULL){
        * blocking_pairs_number_out = blocking_pairs_number;
    }
    long score = blocking_pairs_number + singles_number;
    //cout << "Blocking pairs number: " << blocking_pairs_number;
    //cout << ", Singles number: " << singles_number;
    //cout << ", Score: " << score << endl;
    free_marriage(blocking_pairs);
    return score;
}

// Returns true or false randomply depending on temperature q and delta_score
bool random_acept(long delta_score, double q){
    bool acept = false;
    double pcent = get_pcent();
    if(delta_score < 0) {
        double exponent = delta_score / q;
        double p = exp(exponent);
        acept = p >= pcent;
        //cout << "P: " << p << ", pecent: " << pcent;
        if (acept) {
            //cout << " -> Rondom acepted" << endl;
        } else {
            //cout << " -> NOT Rondom acepted" << endl;
        }
    }
    return acept;
}

// Performs the movement by eliminating the blocking pair of blocking_pairs from marr that generates the best score marriage
marriage *  best_improvement(everyone *mw, marriage *blocking_pairs, marriage *marr, long last_score){
    int * blocking_pair;
    int m ;
    int w;
    marriage * new_marriage = NULL;
    bool better = false;
    long score;
    for (marriage::iterator it = blocking_pairs->begin(); it != blocking_pairs->end(); it++) {
        blocking_pair = new int[2];
        m = (*it)[M];
        w = (*it)[W];
        blocking_pair[M] = m;
        blocking_pair[W] = w;
        new_marriage = copy_marriage_with_breakups(marr, m, w);
        new_marriage->push_back(blocking_pair);
        score = evaluation_function(new_marriage,mw, NULL);
        if(score < last_score){
            better = true;
            break;
        }
        free_marriage(new_marriage);
    }
    if (better){
        return new_marriage;
    }
    else{
        return NULL;
    }
}

// The algorithm itself. The seudoalgorithm shown in the paper is almost equal to this but without
// memory and parameters stuff.
int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    const char * path = argv[1];
    everyone * mw = parse_file(path);
    marriage * marr = initial_random_marriage(mw);
    //cout << "Initial marriage: ";
    print_marriage(marr);
    marriage * best_marr = copy_marriage(marr);
    int T_MAX = atoi(argv[2]);
    long best_score = evaluation_function(best_marr, mw, NULL);
    marriage * new_marr;
    long score;
    long delta_score;
    long best_stable_score;
    marriage * all_blocking_pairs = get_all_blocking_pairs(marr,mw,NULL);
    unsigned long initial_blocking_pairs_number = all_blocking_pairs->size();
    double q_base = mw->m->size();
    double q;
    unsigned long singles;
    bool stable_found = false;
    unsigned long blocking_pairs_number;
    marriage * best_stable_marriage = NULL;
    int t_limit = T_MAX;
    int t;
    for(t = 0; t<T_MAX; t++){
        q = q_base* (1 -(double)t / (double)t_limit);
        //cout << "#" << t << endl;
        if(all_blocking_pairs == NULL) {
            all_blocking_pairs = get_all_blocking_pairs(marr, mw, &singles);
        }
        if(all_blocking_pairs->size() == 0){
            //cout << "No blocking pairs" << endl;
            // save stability
            stable_found = true;
            best_stable_score = best_score < best_stable_score ? best_score : best_stable_score;
            best_stable_marriage = copy_marriage(best_marr);
            if(best_score == 0){
                break;
            }
            // restart
            marr = initial_random_marriage(mw);
            best_score = evaluation_function(marr,mw,NULL);
            t_limit = t;
            //cout << "New random marriage: ";
            print_marriage(marr);
            free_marriage(all_blocking_pairs);
            all_blocking_pairs = NULL;
            continue;
        }
        else{
            //cout << "Blocking pairs: ";
            print_marriage(all_blocking_pairs);
            // check for 5% of the process done
            if(all_blocking_pairs->size() >= 0.05 * initial_blocking_pairs_number){
                new_marr = random_movement(all_blocking_pairs, marr);
            } else{
                new_marr = best_improvement(mw, all_blocking_pairs, marr, score);
                if(new_marr == NULL){
                    new_marr = random_movement(all_blocking_pairs, marr);
                }
            }
            //cout << "New marriage: ";
            //print_marriage(new_marr);
        }
        score = evaluation_function(new_marr,mw, &blocking_pairs_number);
        delta_score = best_score - score;
        //cout << "Delta score: " << delta_score << endl;
        if(delta_score >= 0 || random_acept(delta_score,q)){
            free_marriage(marr);
            marr = copy_marriage(new_marr);
            free_marriage(all_blocking_pairs);
            all_blocking_pairs = NULL;
        }
        if((!stable_found && blocking_pairs_number == 0) ||
                (!stable_found && delta_score >= 0) ||
                (stable_found && blocking_pairs_number == 0 && score < best_stable_score)) {
            //cout << "New marriage is better!" << endl;
            free_marriage(best_marr);
            best_marr = copy_marriage(marr);
            best_score = score;

        }
    }
    // Done, print the result
    if(stable_found) {
        cout << best_stable_marriage->size() << " parejas encontradas" << endl;
        cout << t << " iteraciones realizadas" << endl;
        if(best_score == 0){
            cout << "optimo"<< endl;
        }
        else{
            cout << "no_optimo"<< endl;
        }
        cout << "estable" << endl;
    }
    else{
        cout << best_marr->size() << " parejas encontradas" << endl;
        cout << t << " iteraciones realizadas" << endl;
        cout << "no_optimo" << endl;
        cout << "no_estable" << endl;
    }
    return 0;
}
