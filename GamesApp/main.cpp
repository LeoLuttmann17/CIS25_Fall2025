#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip> // for std::quoted
#include <set>
// removed picojson

using namespace std;

// Simple CSV line parser (handles quoted commas)
vector<string> splitCSVLine(const string &line) {
    vector<string> cols;
    string cur;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes) {
            cols.push_back(cur);
            cur.clear();
        } else cur.push_back(c);
    }
    cols.push_back(cur);
    return cols;
}

string toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

int main() {
    // User login/signup
    string currentUser;
    string favoritesFile;
    while (true) {
        cout << "Welcome! Type 1 to Login, 2 to Sign Up, or 0 to Exit: ";
        string option; getline(cin, option);
        option = trim(option);
        if (option == "0") return 0;
        if (option == "2") { // Sign up
            cout << "Sign up - enter your username: ";
            string name; getline(cin, name);
            name = trim(name);
            cout << "Enter your password: ";
            string password; getline(cin, password);
            password = trim(password);

            ifstream userCheck("users.csv");
            bool exists = false;
            string uline;
            while (getline(userCheck, uline)) {
                vector<string> fields = splitCSVLine(uline);
                if (fields.size() >= 2 && fields[0] == name) { exists = true; break; }
            }
            if (exists) {
                cout << "That username is already taken. Try again.\n";
                continue;
            }
            ofstream newUser("users.csv", ios::app);
            newUser << name << "," << password << "\n";
            cout << "Sign up successful! Please log in.\n";
        } else if (option == "1") {
            cout << "Login - enter your username: ";
            string name; getline(cin, name);
            name = trim(name);
            cout << "Enter your password: ";
            string password; getline(cin, password);
            password = trim(password);

            ifstream userFile("users.csv");
            bool found = false;
            string uline;
            while (getline(userFile, uline)) {
                vector<string> fields = splitCSVLine(uline);
                if (fields.size() >= 2 && fields[0] == name && fields[1] == password) {
                    found = true; break;
                }
            }
            if (found) {
                cout << "Welcome, " << name << "!\n";
                currentUser = name;
                favoritesFile = "favorites_" + name + ".csv";
                break;
            } else {
                cout << "Incorrect username or password. Try again.\n";
            }
        } else {
            cout << "Invalid option, try again.\n";
        }
    }

    // Greeting will always print before menu
    cout << "Howdy, " << currentUser << "! Welcome to the Roblox Games App" << endl;
    ifstream file("roblox_games.csv");
    if (!file.is_open()) {
        cout << "Oops, I can't find any data" << endl;
        return 1;
    }
    string header;
    if (!getline(file, header) || header.empty()) {
        cout << "Oops, I can't find any data" << endl;
        return 1;
    }
    cout << "Dataset successfully loaded!" << endl;
    cout << "CSV Columns: " << header << endl;

    // Load all rows into memory
    vector<vector<string>> rows;
    string line;
    while (getline(file, line)) {
        auto fields = splitCSVLine(line);
        if (!fields.empty()) rows.push_back(fields);
    }
    file.close();

    vector<const vector<string>*> favorites;
    auto saveFavoritesToCsv = [&](const vector<const vector<string>*>& favs) {
        ofstream out_fav(favoritesFile);
        for (const auto* fields : favs) {
            for (size_t j = 0; j < fields->size(); ++j) {
                out_fav << (*fields)[j];
                if (j + 1 < fields->size()) out_fav << ",";
            }
            out_fav << "\n";
        }
    };

    // Main interactive loop
    while (true) {
        // Print the greeting and menu each loop
        cout << "\n Howdy, " << currentUser << "! Welcome to the Roblox Games App " << endl;
        cout << "\n=== MENU ===\n";
        cout << "1) Search by name\n";
        cout << "2) Filter by rating\n";
        cout << "3) View favorites\n";
        cout << "4) Add favorite (by search or exact name)\n";
        cout << "5) Remove favorite\n";
        cout << "6) Recommendations (Coming Soon)\n";
        cout << "7) Statistics (Coming Soon)\n";
        cout << "0) Save & Exit\n";
        cout << "Choose: ";

        int choice;
        string choiceStr;
        getline(cin, choiceStr);
        choiceStr = trim(choiceStr);
        try { choice = stoi(choiceStr); } catch(...) { choice = -1; }

        if (choice == 1) {
            cout << "Enter search query: ";
            string query;
            getline(cin, query);
            query = trim(query);
            string queryLower = toLower(query);
            vector<const vector<string>*> matches;
            for (const auto& fields : rows) {
                if (fields.size() > 1) {
                    string name = toLower(fields[1]);
                    string nameTrimmed = name;
                    nameTrimmed.erase(0, nameTrimmed.find_first_not_of(" \t\n\r"));
                    if (nameTrimmed.empty() || nameTrimmed[0] == '#') continue;
                    if (name.find(queryLower) != string::npos) {
                        matches.push_back(&fields);
                    }
                }
            }
            if (matches.empty()) {
                cout << "\nNo matches found.\n" << endl;
            } else {
                cout << "\n" << matches.size() << " matches found:\n" << endl;
                for (const auto* fields : matches) {
                    for (size_t i = 0; i < fields->size(); ++i) {
                        cout << (*fields)[i];
                        if (i + 1 < fields->size()) cout << ",";
                    }
                    cout << "\n";
                }
                cout << endl;
            }
        }
        else if (choice == 2) {
            // Show the min and max rating before prompting
            double minRating = 1e9, maxRating = -1e9;
            for (const auto& fields : rows) {
                if (fields.size() > 7) {
                    string ratingStr = fields[7];
                    ratingStr.erase(0, ratingStr.find_first_not_of(" \t\n\r"));
                    ratingStr.erase(ratingStr.find_last_not_of(" \t\n\r")+1);
                    if (!ratingStr.empty()) {
                        try {
                            double r = stod(ratingStr);
                            if (r < minRating) minRating = r;
                            if (r > maxRating) maxRating = r;
                        } catch (...) {}
                    }
                }
            }
            if (minRating <= maxRating) {
                cout << "Rating range: " << minRating << " to " << maxRating << endl;
            }
            cout << "Enter minimum rating value: ";
            string minRatingStr;
            getline(cin, minRatingStr);
            minRatingStr = trim(minRatingStr);
            double filterRating = 0;
            try { filterRating = stod(minRatingStr); } catch (...) { filterRating = 0; }
            vector<const vector<string>*> matches;
            for (const auto& fields : rows) {
                if (fields.size() > 7) {
                    string ratingStr = fields[7];
                    ratingStr.erase(0, ratingStr.find_first_not_of(" \t\n\r"));
                    ratingStr.erase(ratingStr.find_last_not_of(" \t\n\r")+1);
                    double rating = 0;
                    try { rating = stod(ratingStr); } catch (...) { rating = 0; }
                    string nameTrim = fields[1];
                    nameTrim.erase(0, nameTrim.find_first_not_of(" \t\n\r"));
                    if (nameTrim.empty() || nameTrim[0] == '#') continue;
                    if (rating >= filterRating) {
                        matches.push_back(&fields);
                    }
                }
            }
            if (matches.empty()) {
                cout << "\nNo matches found.\n" << endl;
            } else {
                cout << "\n" << matches.size() << " matches found:\n" << endl;
                for (const auto* fields : matches) {
                    for (size_t i = 0; i < fields->size(); ++i) {
                        cout << (*fields)[i];
                        if (i + 1 < fields->size()) cout << ",";
                    }
                    cout << "\n";
                }
                cout << endl;
            }
        }
        else if (choice == 3) {
            ifstream in_fav(favoritesFile);
            if (!in_fav) {
                cout << "No favorites file found.\n";
            } else {
                string fav_line;
                bool any = false;
                cout << "Favorites loaded from " << favoritesFile << ":\n";
                while (getline(in_fav, fav_line)) {
                    any = true;
                    cout << fav_line << "\n";
                }
                if (!any) cout << "(No favorites yet)\n";
            }
        }
        else if (choice == 4) {
            cout << "Add favorite by searching name." << endl;
            cout << "Enter search query: ";
            string query;
            getline(cin, query);
            query = trim(query);
            string queryLower = toLower(query);
            vector<const vector<string>*> matches;
            for (const auto& fields : rows) {
                if (fields.size() > 1) {
                    string name = toLower(fields[1]);
                    string nameTrimmed = name;
                    nameTrimmed.erase(0, nameTrimmed.find_first_not_of(" \t\n\r"));
                    if (nameTrimmed.empty() || nameTrimmed[0] == '#') continue;
                    if (name.find(queryLower) != string::npos) {
                        matches.push_back(&fields);
                    }
                }
            }
            if (matches.empty()) {
                cout << "No matches found.\n";
            } else {
                cout << matches.size() << " matches found:\n";
                for (size_t i = 0; i < matches.size(); ++i) {
                    cout << i+1 << ") ";
                    const auto* fields = matches[i];
                    for (size_t j = 0; j < fields->size(); ++j) {
                        cout << (*fields)[j];
                        if (j + 1 < fields->size()) cout << ",";
                    }
                    cout << "\n";
                }
                cout << "Pick number to favorite (0 to cancel): ";
                string pickStr;
                getline(cin, pickStr);
                pickStr = trim(pickStr);
                int pick = 0;
                try { pick = stoi(pickStr); } catch (...) { pick = 0; }
                if (pick <= 0 || (size_t)pick > matches.size()) {
                    cout << "Cancelled.\n";
                } else {
                    const auto* selected = matches[pick-1];
                    // Prevent duplicates
                    bool exists = false;
                    for (const auto* f : favorites) {
                        if ((*f)[1] == (*selected)[1]) { exists = true; break; }
                    }
                    if (!exists) {
                        favorites.push_back(selected);
                        cout << "Added to favorites: " << (*selected)[1] << "\n";
                        saveFavoritesToCsv(favorites);
                    } else {
                        cout << "Already in favorites: " << (*selected)[1] << "\n";
                    }
                }
            }
        }
        else if (choice == 5) {
            if (favorites.empty()) {
                cout << "(No favorites in this session—restart and add favorites)\n";
            } else {
                cout << "Favorites in this session:" << endl;
                for (size_t i = 0; i < favorites.size(); ++i) {
                    cout << i+1 << ") ";
                    const auto* fields = favorites[i];
                    for (size_t j = 0; j < fields->size(); ++j) {
                        cout << (*fields)[j];
                        if (j + 1 < fields->size()) cout << ",";
                    }
                    cout << "\n";
                }
                cout << "Enter number to remove (0 to cancel): ";
                string removeStr;
                getline(cin, removeStr);
                removeStr = trim(removeStr);
                int n = 0;
                try { n = stoi(removeStr); } catch (...) { n = 0; }
                if (n <= 0 || (size_t)n > favorites.size()) {
                    cout << "Cancelled.\n";
                } else {
                    string removedName = (*favorites[n-1])[1];
                    favorites.erase(favorites.begin() + (n-1));
                    saveFavoritesToCsv(favorites);
                    cout << "Successfully removed from favorites: " << removedName << "\n";
                }
            }
        }
        else if (choice == 6) {
            cout << "Feature coming soon: Recommendations is in the works and will be released soon!\n";
        }
        else if (choice == 7) {
            cout << "Feature coming soon: Statistics is in the works and will be released soon!\n";
        }
        else if (choice == 0) {
            cout << "Goodbye!" << endl;
            break;
        }
        else {
            cout << "Unknown or unimplemented option." << endl;
        }
    }
    return 0;
}
