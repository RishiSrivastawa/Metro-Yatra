#include <bits/stdc++.h>
using namespace std;

double toDoubleSafe(const std::string &s, const std::string &context = "") {
    if (s.empty()) {
        cerr << "⚠ Empty number field in row: " << context << "\n";
        return 0.0;
    }
    try {
        return stod(s);
    } catch (...) {
        cerr << "⚠ Invalid number in row (" << context << "): '" << s << "' → treated as 0.0\n";
        return 0.0;
    }
}

// ---------- CSV Parsing ----------
vector<string> parseCSVRow(const string &line) {
    vector<string> result;
    string token;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes; // toggle quoted state
        } else if (c == ',' && !inQuotes) {
            result.push_back(token);
            token.clear();
        } else {
            token.push_back(c);
        }
    }
    result.push_back(token);
    return result;
}

// ---------- Graph structures ----------
struct Edge {
    int to;
    double dist;
    int lineId;
};

struct Station {
    int id;
    string name;
    double distFromStart;
    string line;
    string openingDate;
    string layout;
    double latitude;
    double longitude;
};

class MetroGraph {
public:
    unordered_map<int, Station> stations;
    unordered_map<int, vector<Edge> > adj;

    void addStation(const Station &st) {
        stations[st.id] = st;
    }

    void addEdge(int u, int v, double d, int lineId) {
        adj[u].push_back({v, d, lineId});
        adj[v].push_back({u, d, lineId});
    }

    // ---------- Dijkstra with custom weight function ----------
    pair<double, vector<int> > dijkstra(int s, int t, function<double(const Edge&)> weightFunc) {
        unordered_map<int, double> dist;
        unordered_map<int, int> prev;
        for (auto &kv : stations) dist[kv.first] = 1e18;

        dist[s] = 0;
        priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;
        pq.push({0, s});

        while (!pq.empty()) {
            auto top = pq.top(); pq.pop();
            double d = top.first;
            int u = top.second;

            if (d > dist[u]) continue;
            if (u == t) break;

            for (auto &e : adj[u]) {
                double w = weightFunc(e);
                if (dist[e.to] > d + w) {
                    dist[e.to] = d + w;
                    prev[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        vector<int> path;
        if (dist[t] == 1e18) return {1e18, path};

        for (int v = t; v != s; v = prev[v]) path.push_back(v);
        path.push_back(s);
        reverse(path.begin(), path.end());

        return {dist[t], path};
    }

    // ---------- Fare calculation (example slabs) ----------
    int calculateFare(double dist) {
        if (dist <= 2) return 10;
        if (dist <= 5) return 20;
        if (dist <= 12) return 30;
        if (dist <= 21) return 40;
        if (dist <= 32) return 50;
        return 60;
    }

    void printPath(const vector<int> &path) {
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << stations[path[i]].name << " (" << stations[path[i]].line << ")";
            if (i + 1 < path.size()) cout << " -> ";
        }
        cout << "\n";
    }
};

// ---------- CSV Loader ----------
void loadCSV(const string &filename, MetroGraph &g) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        vector<string> tokens = parseCSVRow(line);
        if (tokens.size() < 8) continue; // skip invalid rows

        Station st;
        st.id = atoi(tokens[0].c_str());
        st.name = tokens[1];
        st.distFromStart = toDoubleSafe(tokens[2], line);
        st.line = tokens[3];
        st.openingDate = tokens[4];
        st.layout = tokens[5];
        st.latitude = toDoubleSafe(tokens[6], line);
        st.longitude = toDoubleSafe(tokens[7], line);

        g.addStation(st);
    }
    file.close();
}

// ---------- Build Edges between consecutive stations ----------
void buildEdges(MetroGraph &g) {
    // Group stations by line
    unordered_map<string, vector<Station>> lineStations;

    for (auto &kv : g.stations) {
        lineStations[kv.second.line].push_back(kv.second);
    }

    // Sort stations on each line by distance and connect them
    for (auto &kv : lineStations) {
        auto &stationsOnLine = kv.second;
        sort(stationsOnLine.begin(), stationsOnLine.end(),
             [](const Station &a, const Station &b) {
                 return a.distFromStart < b.distFromStart;
             });

        for (size_t i = 1; i < stationsOnLine.size(); i++) {
            int u = stationsOnLine[i-1].id;
            int v = stationsOnLine[i].id;
            double d = fabs(stationsOnLine[i].distFromStart - stationsOnLine[i-1].distFromStart);
            g.addEdge(u, v, d, 0); // lineId = 0 for now
        }
    }

    // Connect interchange stations
    unordered_map<string, vector<int>> nameToIds;
    for (auto &kv : g.stations) {
        string base = kv.second.name;
        size_t pos = base.find('[');
        if (pos != string::npos) base = base.substr(0, pos);
        // trim trailing spaces
        base.erase(base.find_last_not_of(" \t\n\r\f\v") + 1);
        nameToIds[base].push_back(kv.first);
    }
    for (auto &kv : nameToIds) {
        auto &ids = kv.second;
        if (ids.size() > 1) {
            for (size_t i = 0; i < ids.size(); i++) {
                for (size_t j = i+1; j < ids.size(); j++) {
                    g.addEdge(ids[i], ids[j], 0.0, -1);
                }
            }
        }
    }
}

// ---------- Main ----------
int main() {
    MetroGraph g;
    loadCSV("Delhi-Metro-Network.csv", g);

    buildEdges(g); // build graph connections

    cout << "Choose routing option:\n";
    cout << "1. Shortest distance route\n";
    cout << "2. Minimum fare route\n";
    cout << "3. Minimum line interchange route\n";
    cout << "Enter choice (1/2/3): ";
    int choice; cin >> choice;

    cout << "Enter source station ID: ";
    int s; cin >> s;
    cout << "Enter destination station ID: ";
    int t; cin >> t;

    pair<double, vector<int>> result;
    double totalDist = 0;
    int totalInterchanges = 0;

    if (choice == 1 || choice == 2) {
        // For shortest distance and minimum fare, weight = distance
        result = g.dijkstra(s, t, [](const Edge &e) {
            return e.dist;
        });
        totalDist = result.first;
    } else if (choice == 3) {
        // For minimum line interchange, weight = 0 if same line, 1 if interchange (lineId == -1)
        result = g.dijkstra(s, t, [](const Edge &e) {
            return (e.lineId == -1) ? 1.0 : 0.0;
        });
        // Calculate total distance and interchanges for the path
        totalDist = 0;
        totalInterchanges = 0;
        auto &path = result.second;
        for (size_t i = 1; i < path.size(); i++) {
            int u = path[i-1];
            int v = path[i];
            // Find edge between u and v
            for (auto &e : g.adj[u]) {
                if (e.to == v) {
                    totalDist += e.dist;
                    if (e.lineId == -1) totalInterchanges++;
                    break;
                }
            }
        }
    } else {
        cout << "Invalid choice.\n";
        return 1;
    }

    auto &path = result.second;
    if (path.empty()) {
        cout << "No path found.\n";
    } else {
        if (choice == 1) {
            cout << "Shortest distance: " << totalDist << " km\n";
            cout << "Fare: Rs " << g.calculateFare(totalDist) << "/-\n";
        } else if (choice == 2) {
            cout << "Minimum fare route distance: " << totalDist << " km\n";
            cout << "Fare: Rs " << g.calculateFare(totalDist) << "/-\n";
        } else if (choice == 3) {
            cout << "Minimum line interchanges: " << totalInterchanges << "\n";
            cout << "Total distance: " << totalDist << " km\n";
            cout << "Fare: Rs " << g.calculateFare(totalDist) << "/-\n";
        }
        g.printPath(path);
    }

    return 0;
}
