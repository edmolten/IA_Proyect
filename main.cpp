#include <iostream>

#include <fstream>
#include <cstring>
#include <cstdlib>
#include "main.h"

using namespace std;


char * last_char(char * string){
    return &string[strlen(string)-1];
}

int main() {
    ifstream file;
    file.open("C:\\Users\\eddox\\Desktop\\IA\\instances\\Chart18\\instance_10502.txt");
    char word[5];
    side* men = new side;
    side* women = new side;
    side* current_side = men;
    file >> word; //skip first 1:
    preference_list * pref_list = new preference_list;
    bool got_index = true; //flag to alert if the last word was a number and a :
    while (file >> word) {

        cout << word << endl;

        if ( strcmp(":", last_char(word) )==0) {
            current_side->push_back(pref_list);
            if (strcmp("1:",word)==0) {
                current_side = women;
            }
        }
        else if (word == "(") {
            preference *pref = new preference;
            while (file >> word) {
                if (word == ")") break;
                pref->insert(atoi(word));
            }
            pref_list->push_back(pref);
        } else {
            preference *pref = new preference;
            pref->insert(atoi(word));
            pref_list->push_back(pref);
        }
    }

    marriage* m= new marriage;
    int *pair1 = new int[2];
    pair1[0] = 1;
    pair1[1] = 3; //(hombre 1 con mujer 3)
    m->push_back(pair1);
}