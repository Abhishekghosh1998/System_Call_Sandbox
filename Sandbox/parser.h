#ifndef PARSER_H
#define PARSER_H

using namespace std;

extern unordered_map <string, int> handle;
extern vector <map<int,string>> adj;
extern vector <map<int, string>> reverseAdj;
extern unordered_map <int, int> handle_to_label;

extern int start_node;

bool isHandlePresent(string id);

int getHandle(string id);

void installNode(string id);

void addEdge(string id1, string id2, string label);

void parse(string dot_file_name);
#endif