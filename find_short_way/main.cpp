#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
//BFS - Breadth-First Search

map< string,  vector<pair<string,int>>  > roads; 
map<string,int> coast;

void FindTheWay(const string position, int totalcost, const string finish) {
    
    for (auto it=roads[position].begin(); it != roads[position].end(); it++) {
        if (coast.find(it->first) == coast.end() ||
            coast[it->first] > totalcost + it->second) {

            coast[it->first] = totalcost + it->second;
            FindTheWay(it->first, totalcost + it->second, finish);
        }
    }
   

}
// do not affect the code
void GiveMap(map<string, vector<pair<string, int>>>& myMap) {
    
    myMap["start"] = {{"end", 10}, {"middle", 5},{"lol", 12}};
    string key = "start";
    for (auto it=myMap[key].begin(); it != myMap[key].end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
}

int main() {
    fstream fin("input.txt");

    string start, finish;
    fin >> start >> finish;


    string a,b;
    int cost;

    while (true) {
        fin >> a;
        if (a=="-1") break;
        fin >> b >> cost;

        roads[a].push_back({b,cost}); 
    }
    
    cout << "\nLower coast: ";
    FindTheWay(start,0,finish); 
    cout << coast[finish] << "\n";

    
    return 0;
}
