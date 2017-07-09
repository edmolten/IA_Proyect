#ifndef IA_MAIN_H
#define IA_MAIN_H

#include <list>
#include <set>
using namespace std;

//index for men. The first element of a blocking pair or a match is always the man
#define M 0
//index for women. The first element of a blocking pair or a match is always the woman
#define W 1
// Each slot in a preference list is a list with one number or many in case of tie in that position
typedef set<int> preference;
// Structure for preference list
typedef list<preference*> preference_list;
// A side is all the men or all the women preference lists. The file parsing generates two sides
typedef list<preference_list*> side;
// A marriage is a list of pairs, represented by two integers. A set of blocking pairs
// is also represented with this structure
typedef list<int*> marriage;
// structure to manipulate the two sides
struct everyone {
    side * m;
    side * w;
};
#endif //IA_MAIN_H
