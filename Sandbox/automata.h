#ifndef AUTOMATA_H
#define AUTOMATA_H

using namespace std;
extern set<int> inst_des;

void initialize_inst_des();

bool check_equal_sets(set <int>&a, set<int> &b);

set <int> epsilon_closure(set <int> &id, vector<map<int,string>> &adj);

//bool isMovePossible(string input);

void makeMove(string input);

set<int> setOfPossibleAddressOnInput(string input);
#endif