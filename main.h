#ifndef IA_MAIN_H
#define IA_MAIN_H

#include <list>
#include <set>
using namespace std;

#define M 0
#define W 1
typedef set<int> preference;
typedef list<preference*> preference_list;
typedef list<preference_list*> side;
typedef list<int*> marriage;

struct everyone {
    side * m;
    side * w;
};
#endif //IA_MAIN_H
