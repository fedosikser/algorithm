//approximate greedy algorithm
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;

vector<string> FullNameStates {
    "Alabama",
    "Alaska",
    "Arizona",
    "Arkansas",
    "California",
    "Colorado",
    "Connecticut",
    "Delaware",
    "Florida",
    "Georgia",
    "Hawaii",
    "Idaho",
    "Illinois",
    "Indiana",
    "Iowa",
    "Kansas",
    "Kentucky",
    "Louisiana",
    "Maine",
    "Maryland",
    "Massachusetts",
    "Michigan",
    "Minnesota",
    "Mississippi",
    "Missouri",
    "Montana",
    "Nebraska",
    "Nevada",
    "New Hampshire",
    "New Jersey",
    "New Mexico",
    "New York",
    "North Carolina",
    "North Dakota",
    "Ohio",
    "Oklahoma",
    "Oregon",
    "Pennsylvania",
    "Rhode Island",
    "South Carolina",
    "South Dakota",
    "Tennessee",
    "Texas",
    "Utah",
    "Vermont",
    "Virginia",
    "Washington",
    "West Virginia",
    "Wisconsin",
    "Wyoming"
};

vector<string> ShortNameStates {
    "AL",
    "AK",
    "AZ",
    "AR",
    "CA",
    "CO",
    "CT",
    "DE",
    "FL",
    "GA",
    "HI",
    "ID",
    "IL",
    "IN",
    "IA",
    "KS",
    "KY",
    "LA",
    "ME",
    "MD",
    "MA",
    "MI",
    "MN",
    "MS",
    "MO",
    "MT",
    "NE",
    "NV",
    "NH",
    "NJ",
    "NM",
    "NY",
    "NC",
    "ND",
    "OH",
    "OK",
    "OR",
    "PA",
    "RI",
    "SC",
    "SD",
    "TN",
    "TX",
    "UT",
    "VT",
    "VA",
    "WA",
    "WV",
    "WI",
    "WY"
};

int CountStates = 50;

map<string, int> H_S_T_N;
map<string, int> H_LS_T_N;

//HASH TABLE (Long) State to Number
void Set_H_S_T_N(){
    for (int i = 0; i < CountStates; i++) {
        H_S_T_N[ShortNameStates[i]] = i;
        H_LS_T_N[FullNameStates[i]] = i;
    }
}

bool AlreadySetStations = false;

class State {
private:
    string m_name;
    string m_l_name;
    int m_number;
public:
    //Get Set : Number, Name, Long Name
    void SetNu(int number) { m_number = number; }
    int GetNu() const { return m_number; }
    void SetNa(string name) { m_name = name; }
    string GetNa(bool long_name=false) const { 
        if (long_name) return m_l_name; else return m_name; }
    string GetLNa() const { return m_l_name; }

    //Convert 
    int Conv(const string& state) const {
        if (state.length() == 2) {
            auto it = H_S_T_N.find(state);
            if (it != H_S_T_N.end()) {
                return it->second;
            }
        } else {
            auto it = H_LS_T_N.find(state);
            if (it != H_LS_T_N.end()) {
                return it->second;
            }
        }
        return -1; // Not found
    }
    
    string Conv(int number, bool long_name=false) const {
        if (number < 0 || number >= CountStates) return "";
        if (long_name) return FullNameStates[number];
        return ShortNameStates[number];
    }
    
    bool operator<(const State& other) const {
        return m_number < other.m_number;
    }
    
    State(const State& other) : m_name(other.m_name), m_l_name(other.m_l_name), m_number(other.m_number) {}
    
    State& operator=(const State& other) {
        if (this != &other) {
            m_number = other.m_number;
            m_name = other.m_name;
            m_l_name = other.m_l_name;
        }
        return *this;
    }

    State(int number) { 
        if (number >= 0 && number < CountStates) {
            m_number = number;
            m_name = Conv(number); 
            m_l_name = Conv(number, true); 
        } else {
            m_number = 0;
            m_name = "AL"; 
            m_l_name = "Alabama";
        }
    }
    
    State(const string& name) { 
        m_number = Conv(name);
        if (m_number != -1) {
            m_name = Conv(m_number); 
            m_l_name = Conv(m_number, true);
        } else {
            m_number = 0;
            m_name = "AL"; 
            m_l_name = "Alabama";
        }
    }
    //Empty Constructor
    State() { 
        m_number = 0; 
        m_name = "AL"; 
        m_l_name = "Alabama"; 
    }
};

class Station {
private:
    string m_name;
    int m_number;
    set<State> m_states;
public:
    //Getters and Setters
    void SetNu(int number) { m_number = number; }
    int GetNu() const { return m_number; }
    void SetNa(const string& name) { m_name = name; }
    string GetNa() const { return m_name; }
    void SetSt(const set<State>& states) { m_states = states; }
    const set<State>& GetSt() const { return m_states; }

    bool operator==(const Station& other) const {
        return m_number == other.m_number;
    }
    
    bool operator<(const Station& other) const {
        return m_number < other.m_number;
    }
    void Print(bool long_name=false) {
        cout << endl << m_name << " : ";
        for (auto& state : m_states) {
            cout << state.GetNa(long_name) << ", ";
        }
    }
    Station(int number=0) { m_number = number; }
    
    Station(const string& name, int number, const set<State>& states) : 
        m_name(name), m_number(number), m_states(states) {}
};

set<int> states_needed;
set<int> final_stations;
vector<State> states;
vector<Station> stations;

void SetAllStationsFromFile() {
    if (AlreadySetStations) return;
    ifstream fin("stations.txt");
    if (!fin.is_open()) {
        cout << "Error opening stations.txt" << endl;
        return;
    }
    
    string line;
    int number = 0;
    while (getline(fin, line)) {
        Station station;
        set<State> states_set;

        int i = line.find("-");
        if (i != string::npos) {
            string station_name = line.substr(4, i-5);
            // Trim whitespace from station name
            int first = station_name.find_first_not_of(' ');
            int last = station_name.find_last_not_of(' ');
            if (first != string::npos && last != string::npos) {
                station_name = station_name.substr(first, last - first + 1);
            }
            
            station.SetNa(station_name);
            station.SetNu(number);
            
            int start = i + 2;
            for (int j = i+2; j <= (int)line.length(); j++) {
                if (j == (int)line.length() || line[j] == ',') {
                    string stateStr = line.substr(start, j - start);
                    // Trim whitespace
                    int first = stateStr.find_first_not_of(' ');
                    int last = stateStr.find_last_not_of(' ');
                    if (first != string::npos && last != string::npos) {
                        stateStr = stateStr.substr(first, last - first + 1);
                    }
                    
                    if (!stateStr.empty()) {
                        states_set.insert(State(stateStr));
                    }
                    start = j + 1;
                }
            }
            station.SetSt(states_set);
            stations.push_back(station);
            number++;
        }
    }
    fin.close();
    AlreadySetStations = true;
}

void SetAllStatesNeeded() {
    states_needed.clear();
    for (int i = 0; i < CountStates; i++) {
        states_needed.insert(i);
    }
}

void PrintAllStations(bool long_name = false) {
    for (size_t i = 0; i < stations.size(); i++) {
        stations[i].Print(long_name);
    }
}

int main() {
    Set_H_S_T_N();
    SetAllStatesNeeded();
    SetAllStationsFromFile();
    
    // Greedy algorithm implementation
    set<int> states_needed_local = states_needed;
    vector<int> final_stations_vec;
    
    while (!states_needed_local.empty()) {
        int best_station = -1;
        set<int> states_covered;
        
        // Find the station that covers the most uncovered states
        for (size_t i = 0; i < stations.size(); i++) {
            auto& station_states = stations[i].GetSt();
            set<int> covered;
            
            // Check which needed states this station covers
            for (auto& state : station_states) {
                int state_num = state.GetNu();
                if (states_needed_local.find(state_num) != states_needed_local.end()) {
                    covered.insert(state_num);
                }
            }
            
            // Update best station if this one covers more states
            if (covered.size() > states_covered.size()) {
                best_station = i;
                states_covered = covered;
            }
        }
        
        if (best_station != -1) {
            final_stations_vec.push_back(best_station);
            // Remove covered states from needed states
            for (int state_num : states_covered) {
                states_needed_local.erase(state_num);
            }
        } else {
            // No station covers any remaining states
            break;
        }
    }
    
    cout << "\nCount of final stations: " << final_stations_vec.size() << endl;

    cout << "\nStates needed: " << states_needed_local.size() << endl;
    for (auto to: states_needed_local) {
        State state = State(to);
        cout << state.GetNa() << ", ";
    }

    PrintAllStations(1);

    return 0;
}