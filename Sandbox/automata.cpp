#include <bits/stdc++.h>
#include "parser.h"
#include "automata.h"
using namespace std;

set <int> inst_des;

void initialize_inst_des(){
    inst_des.insert(start_node);
    inst_des=epsilon_closure(inst_des,adj);
}

bool check_equal_sets(set<int> &a, set<int> &b){
    if(a.size()!=b.size())
        return false;
    for(auto e: a){
        if(b.find(e)==b.end())
            return false;
    }
    return true;
}

set <int> epsilon_closure(set <int> &id ,vector<map<int,string>> &adj){
    set <int> curr_id;
    for (auto e: id){
        curr_id.insert(e);
    }
    set <int> prev_id;
    do{
        prev_id=curr_id;
        set <int> temp;
        for (auto e: prev_id){
            temp.insert(e);
        }
        for(auto x: prev_id){
            for(auto y: adj[x]){
                if(y.second=="epsilon")
                    temp.insert(y.first);
            }
        }
        curr_id=temp;
    }while(!check_equal_sets(prev_id,curr_id));
    return curr_id;
}

set <int> setOfPossibleAddressOnInput(string input){
    set <int> new_inst_des;
    for(auto x: inst_des){
        for(auto y: adj[x]){
            if(y.second==input){
                new_inst_des.insert(x);
            }
        }
    }
    new_inst_des=epsilon_closure(new_inst_des,reverseAdj);
    set<int> addresses;
    for (auto x: new_inst_des){
        if(handle_to_label.find(x)!=handle_to_label.end()){
            //element found
            addresses.insert(handle_to_label[x]);
        }
    }
    return addresses;
}

// bool isMovePossible(string input){
//     set <int> new_inst_des;
//     for(auto x: inst_des){
//         for(auto y: adj[x]){
//             if(y.second==input){
//                 new_inst_des.insert(y.first);
//             }
//         }
//     }
//     new_inst_des=epsilon_closure(new_inst_des, adj);
//     return (new_inst_des.size()!=0);
// }

void makeMove(string input){
    set <int> new_inst_des;
    for(auto x: inst_des){
        for(auto y: adj[x]){
            if(y.second==input){
                new_inst_des.insert(y.first);
            }
        }
    }
    inst_des=epsilon_closure(new_inst_des, adj);
}