#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

#include "parser.h"
using namespace std;

unordered_map <string, int> handle;
vector <map<int,string>> adj;

vector <map<int,string>> reverseAdj;

unordered_map <int, int> handle_to_label;

int start_node;



bool isHandlePresent(string id){
    return handle.find(id)!=handle.end();

}
int getHandle(string id){
    if(!isHandlePresent(id)){
        cout<<"No handle found"<<endl;
        return -1;
    }
    return handle[id];
}

void installNode(string id){
    adj.push_back(map<int,string>());
    int node_id=adj.size()-1;
    handle.insert({id,node_id});
}

void addEdge(string id1, string id2, string label){
    if(!isHandlePresent(id1)){
        installNode(id1);
    }
    if(!isHandlePresent(id2)){
        installNode(id2);
    }
    int x=getHandle(id1);
    int y=getHandle(id2);
    adj[x].insert({y,label});
}

void constructReverseGraph(){
    int size_of_adj=adj.size();
    int i;
    for(i=1;i<=size_of_adj;i++)
        reverseAdj.push_back(map<int,string>());

    for (i=0;i<size_of_adj;i++){
        for(auto e:adj[i]){
            reverseAdj[e.first].insert({i,e.second});
        }
    }
}

void parse(string dot_file_name){
    ifstream inputFile(dot_file_name);
    char s[1024];
    string id1, id2, label;
    int index_of_first_space;
    int index_of_second_space;
    int index_of_third_space;
    inputFile.getline(s,1024);
    inputFile.getline(s,1024);
    inputFile.getline(s,1024);
    
    inputFile.getline(s,1024);
    string line(s);
    index_of_first_space = line.find_first_of(' ');
    id1=line.substr(1,index_of_first_space-1);
    string entrypoint_id=id1;
    

    inputFile.getline(s,1024);
     
    do{
        string line(s);
        index_of_first_space = line.find_first_of(' ');
        if(line.at(index_of_first_space+1)=='['){
            //end of the edge list encountered
            break;
        }
        index_of_second_space = line.find_first_of(' ',index_of_first_space+1);
        index_of_third_space =  line.find_first_of(' ',index_of_second_space+1);
        id1=line.substr(1,index_of_first_space-1);
        id2=line.substr(index_of_second_space+1,index_of_third_space-index_of_second_space-1);
        int index_of_equal_sign= line.find_first_of('=');
        label= line.substr(index_of_equal_sign+1,line.find_first_of(']')-index_of_equal_sign-1);
        //cout<<id1<<","<<id2<<" "<<label<<endl;
        addEdge(id1,id2,label);
        inputFile.getline(s,1024);
    }while(s[0]!='}');
    
    while(s[0]!='}'){
        string line(s);
        index_of_first_space = line.find_first_of(' ');
        id1=line.substr(1,index_of_first_space-1);
        int index_of_equal_sign= line.find_first_of('=');
        label= line.substr(index_of_equal_sign+1,line.find_first_of(']')-index_of_equal_sign-1);
        handle_to_label.insert({getHandle(id1),stoi(label)});
        //cout<<id1 <<"->"<<label<<endl;
        inputFile.getline(s,1024);
    }
   
    //string entrypoint_id=id2.substr(0,id2.find_first_of('.'))+".1";
    //cout<<entrypoint_id<<endl;
    start_node=getHandle(entrypoint_id);
    constructReverseGraph();
    //cout<<start_node<<endl;

}

// int main(){
//     parse();
//     return 0;
// }
