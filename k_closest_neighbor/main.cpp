#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <locale>

using namespace std;
 

// 18(genres) bits out of 32 bits(int) are used to store movie genres.
using uint = unsigned int;

string genreNames[] = {
    "Action",
    "Adventure",
    "Animation",
    "Children's",
    "Comedy",
    "Crime",
    "Documentary",
    "Drama",
    "Fantasy",
    "Film-Noir",
    "Horror",
    "Musical",
    "Mystery",
    "Romance",
    "Sci-Fi",
    "Thriller",
    "War",
    "Western",
    "(no genres listed)"
};

float MiddleRating_g = 3.0905;

class User ;
class Movie ;
vector<User> users;
vector<Movie> movies;

void RequestHandler(bool& answer, int type) {
    cin.ignore(32767, '\n');
    string StrAns;
    cin >> StrAns;
    if (!(StrAns == "0" || StrAns == "1")) {
        cout << "\nInvalid input. Please enter 0 or 1.\n";
        RequestHandler(answer, type);
    } else answer = (StrAns == "1") ? 1 : 0;
}

string toBinary(uint n) {
    if (n == 0) {
        return "0";
    }
    string result;
    while (n > 0) {
        result.insert(result.begin(), char('0' + (n & 1)));
        n >>= 1;
    }
    return result;
}

uint StringToGenreMask(string& genres) {
    uint mask = 0;
    int start = 0;
    while (start < genres.size()) {
        int end = genres.find('|', start);
        if (end == string::npos) end = genres.size();

        string token = genres.substr(start, end - start);
        
        for (uint8_t i = 0; i < 18; i++) {
            if (token == genreNames[i]) {
                mask |= (1u << i);
                break;
            }
        }

        start = end + 1;
    }

    return mask;
}

inline void AddGenreBit(uint& mask, uint8_t& g) {
    mask |= (uint(1) << g);
}

inline bool HasGenreBit(uint& mask, uint8_t& g) {
    return (mask & (uint(1) << g)) != 0;
}

inline string GenreMaskToString(uint& mask) {
    string s = "";
    for (uint8_t i = 0; i < 18; i++) {
        if (HasGenreBit(mask, i)) {
            s += genreNames[i] + "|";
        }
    }
    return s;
}


class Movie {
private:
    uint id;
    string title;
    uint genre;
    float rating;
    uint ratingCount;
public:
    uint getId() { return id; }
    string getTitle() { return title; }
    uint getGenre() { return genre; }
    float getRating() { return rating; }
    uint getRatingCount() { return ratingCount; }

    Movie(uint id, string title, string sgenre) {
        this->genre = StringToGenreMask(sgenre);
        this->id = id;
        this->title = title;
        this->rating = 0;
        this->ratingCount = 0;
    }
    Movie() {}
    void SetRating(uint8_t r) {
        rating = rating * ratingCount + r;
        ratingCount++;
        rating /= ratingCount;
    }
    void Show () {
        cout << "Movie: " << id << " || " << title << " || " << GenreMaskToString(genre) << " || " << rating << " || " << ratingCount << endl;
    }
};



class User {
private:
    uint id;
    vector < float > genreRating;
    vector < uint > genreRatingCount;

public:
    User(uint id)
        : id(id),
        genreRating(18, 0.0f),
        genreRatingCount(18, 0)
    {}

    User()
        : id(0),
        genreRating(18, 0.0f),
        genreRatingCount(18, 0)
    {}
    uint getId() { return id; }

  
    void SetGenreRating(uint genre, uint8_t rating) {
        for (uint8_t i = 0; i < 18; i++) {
            if (HasGenreBit(genre, i)) {
                genreRating[i] = (genreRating[i] * genreRatingCount[i] + rating) / (genreRatingCount[i] + 1);
                genreRatingCount[i]++;
            }
        }
    }

    void SetUserRating(uint movieId, uint8_t rating) {
        movies[movieId].SetRating(rating);
        users[id].SetGenreRating(movies[movieId].getGenre(), rating);
    }
    void PrintFavouriteGenres() {
        uint total_count = 0;
        vector < pair < float, uint8_t > > v;
        for (uint8_t i = 0; i < 18; i++) {
            total_count+=genreRatingCount[i];
        }
        for (uint8_t i = 0; i < 18; i++) {
            v.push_back({genreRating[i]/total_count*genreRatingCount[i], i});
        }
        sort(v.begin(), v.end());
        reverse(v.begin(), v.end());
        cout << "\nFavourite genres: ";
        cout << fixed << setprecision(2);
        for (uint8_t i = 0; i < 18; i++) {
            if (v[i].first > 0)
            cout << endl << genreNames[v[i].second] << " (" << 
            v[i].first*total_count/genreRatingCount[v[i].second] << " || " <<  (double)genreRatingCount[v[i].second]/total_count*100 << "% || " << genreRatingCount[v[i].second] << "), ";
        }
        v.clear();
        cout << endl;
    }
    uint8_t GetFavouriteGenre() {
        uint8_t mx=0;
        for (uint8_t i = 0; i < 18; i++) {
            if (genreRating[i] > genreRating[mx]) {
                mx = i;
            }
        }
        return mx;
    }
    void Show() {
        cout << "\nUser: " << id;
        uint8_t k{};
        for (uint8_t i = 0; i < 18; i++) {
            k+=genreRatingCount[i];
        }
        uint8_t f = GetFavouriteGenre();
        cout << "Count of ratings: " << k*1 << endl;
        cout << "Favourite genre: " << genreNames[f] << endl;
        cout << "Favourite genre rating: " << genreRating[f] << endl;
        cout << "Favourite genre count: " << genreRatingCount[f] << endl;
    
    }
};



void GetRatingDat(bool signToDevelop) {
    //UserID::MovieID::Rating::Timestamp
    FILE* f = fopen("ratings.dat", "r");
    uint userId, movieId, timestamp;
    int rating;
    while (fscanf(f, "%d::%d::%d::%d\n",
                &userId, &movieId, &rating, &timestamp) == 4) {
          users[userId].SetUserRating(movieId, rating); 
         
    } fclose(f);
    if (signToDevelop) { 
        cout << "\nRatings.dat/Loaded " << userId << " users.\n";
        cout << userId << " " << movieId << " " << rating << endl;
    }
}
bool parseRecord(const string& line, uint& id) {

    size_t pos = 0;
    size_t next;

    next = line.find("::", pos);
    if (next == string::npos) return false;
    id = (uint)stoi(line.substr(pos, next - pos));
    pos = next + 2;

    return true;
}
void GetUserDat(bool signToDevelop) {
    //UserID::Gender::Age::Occupation::Zip-code
    ifstream file("users.dat");
    uint userId;
    bool gender;
    uint8_t age;
    uint8_t occupation;
    string s;
    while (getline(file, s)) {
        userId=0;
        if (!parseRecord(s,userId))
        cout << "!!!!!Users.dat/Error parsing record: " << s << endl, exit(0);

        users[userId] = User(userId);

    } file.close();
    if (signToDevelop) {
    cout << "\nUsers.dat/Loaded " << users.size() << " users.";
    cout << " Last loaded user: " << userId << " || " << users[userId].getId() << endl;
    }
}

bool parseRecord(const string& line, uint& id, string& title, string& genres) {
    size_t pos = 0;
    size_t next;

    // A
    next = line.find("::", pos);
    if (next == string::npos) return false;
    id = stoi(line.substr(pos, next - pos));
    pos = next + 2;

    // Title
    next = line.find("::", pos);
    if (next == string::npos) return false;
    title = line.substr(pos, next - pos);
    pos = next + 2;
    
    // Genres
    genres = line.substr(pos);
    return true;

}


void GetMovieDat(bool signToDevelop=0) {
    //MovieID::Title::Genres
    ifstream file("movies.dat");
    uint movieId; 
    string genres, title;

    string s;
    while (getline(file, s)) {
        if (!parseRecord(s,movieId, title, genres))
        cout << "Movies.dat/Error parsing record: " << s << endl, exit(0);
        movies[movieId] = Movie(movieId, title, genres);
       
    } file.close();

    if (signToDevelop) {
    cout << "\nMovies.dat/Loaded " << movies.size() << " movies.";
    cout << " Last loaded movie: " << movieId << endl;
    }
}

void GetData(bool signToDevelop=0) {

    setlocale(LC_ALL, ""); 

    users.push_back(User());
    movies.push_back(Movie()); 
    users.resize(6100);
    movies.resize(4000);

    
    GetUserDat(signToDevelop);
    GetMovieDat(signToDevelop);
    GetRatingDat(signToDevelop);
}

void GetAverageRating() {
    double rate{0.f};
    uint count{};
    for (int i = 0; i < movies.size(); i++) {
        if (movies[i].getId() != 0) {
            count++;
            rate += movies[i].getRating();
        }
    }
    cout << fixed << setprecision(5) << "Average rating: " << rate / count << endl;
    cout << "Number of movies: " << count << endl;
}

int main() {

    GetData();
     
    bool ans{1};
    while (ans)  {
        cout << "Set user id: ";
        int userId;
        cin >> userId;   
        users[userId].PrintFavouriteGenres();
        cout << "Do you want to continue? (1/0)";
        RequestHandler(ans,2);
    }
    

   

}