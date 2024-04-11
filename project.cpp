      #include <bits/stdc++.h>
      #include <unistd.h>
      #include <stdlib.h>

      #define TOTAL 252

      using namespace std;

      int Graph[TOTAL][TOTAL];
      map<int, string> station;
      vector<pair<int, int>> interchanges{};
      vector<int> terminals{0};

      void mapstations() {
          ifstream fin;
          string s;
          fin.open("stations.txt", ios::in);
          int n = 0;
          fin.seekg(0);
          fin.clear();
          while (!fin.eof()) {
              getline(fin, s);
              if (s == "Terminate") {
                  terminals.push_back(n - 1);
                  if (fin.eof())
                      continue;
                  terminals.push_back(n);
                  continue;
              }
              station[n] = s;
              n++;
          }

          for (int i = 0; i < station.size(); i++) {
              for (int j = 0; j < station.size(); j++) {
                  if (j == i)
                      continue;
                  if (station[i] == station[j])
                      interchanges.push_back(make_pair(i, j));
              }
          }
      }

      void joingraph() {
          for (int i = 0; i < terminals.size(); i += 2) {
              for (int j = terminals[i]; j < terminals[i + 1]; j++) {
                  Graph[j][j + 1] = 2;
                  Graph[j + 1][j] = 2;
              }
          }

          for (int i = 0; i < interchanges.size(); i++) {
              Graph[interchanges[i].first][interchanges[i].second] = 4;
          }
      }

      string color(int n) {
          if (n >= 0 && n <= 36)
              return "Yellow";
          if (n >= 37 && n <= 65)
              return "Red";
          if (n >= 66 && n <= 123)
              return "Blue";
          if (n >= 124 && n <= 144)
              return "Green";
          if (n >= 145 && n <= 182)
              return "Pink";
          if (n >= 183 && n <= 216)
              return "Violet";
          if (n >= 217 && n <= 241)
              return "Magenta";
          if (n >= 242 && n <= 245)
              return "Grey";
          if (n >= 246 && n <= 251)
              return "Airport Express";
          return ""; // Added to address the warning and handle cases where n is out of range
      }

      string direction(int child[], int n, int des) {
          string s = color(n);
          if (s == "Yellow") {
              if (child[n] == n + 1)
                  return station[terminals[1]];
              return station[terminals[0]];
          }
          // Add similar conditions for other colors

          return ""; // Added to address the warning and handle cases where color(n) doesn't match any condition
      }

      int check(string s) {
          for (int i = 0; i < TOTAL; i++) {
              string s2 = station[i];
              transform(s.begin(), s.end(), s.begin(), ::tolower);
              transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
              if (s.compare(s2) == 0) {
                  cout << station[i] << " is selected" << endl;
                  return i;
              }
          }

          int count = 0;
          unordered_map<int, int> search;
          bool first = true;
          for (int i = 0; i < TOTAL; i++) {
              string s2 = station[i];
              transform(s.begin(), s.end(), s.begin(), ::tolower);
              transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
              if (s.compare(0, s.size(), s2, 0, s.size()) == 0) {
                  if (first) {
                      cout << "Search results :" << endl;
                      first = false;
                  }
                  cout << i << "->" << station[i] << "  (" << color(i) << " Line)" << endl;
                  search[i] = 1;
                  count++;
              }
          }

          if (count == 0)
              return -1;

          int key;
          cout << "Enter key to select station : ";
          while (1) {
              cin >> key;
              if (search.find(key) != search.end()) {
                  cout << station[key] << " is selected" << endl;
                  return key;
              } else
                  cout << "Enter valid key from the list" << endl;
          }
      }

      void printPath(int parent[], int j, int child[], int des, int &stations, int &intr) {
          if (parent[j] != -1 && station[j] != station[parent[j]])
              stations++;

          if (parent[j] == -1) {
              stations++;
              cout << "_________________________________________________________" << endl;
              cout << "Path from " << station[j] << " to " << station[des] << endl;
              cout << "_________________________________________________________" << endl;
              cout << "\nStart at " << station[j] << " station on " << color(j) << " Line" << endl;
              bool flag = false;
              for (int i = 0; i < interchanges.size(); i++) {
                  if (j == interchanges[i].first && child[j] == interchanges[i].second)
                      flag = true;
              }
              if (!flag) {
                  cout << endl;
                  cout << "Board metro towards " << direction(child, j, des) << endl;
                  cout << endl;
                  cout << "-->" << station[j];
              }
              return;
          }

          child[parent[j]] = j;
          printPath(parent, parent[j], child, des, stations, intr);

          if (station[j] == station[des] && j != des)
              return;

          if (j != des) {
              for (int i = 0; i < interchanges.size(); i++) {
                  if (parent[j] == interchanges[i].first && j == interchanges[i].second) {
                      intr++;
                      cout << endl;
                      if (color(parent[j]) != color(j)) {
                          cout << "_____________________________________________________" << endl;
                          cout << endl << "Change here for " << color(j) << " Line";
                      }
                      cout << endl << "Board metro towards " << direction(child, j, des) << endl;
                      cout << "_____________________________________________________" << endl;
                      cout << endl;
                  }
              }
          }
          cout << "-->" << station[j];
      }

      int minDistance(int dist[], bool sptSet[]) {
          int min = INT_MAX, min_index;

          for (int v = 0; v < TOTAL; v++) {
              if (sptSet[v] == false && dist[v] <= min) {
                  min = dist[v];
                  min_index = v;
              }
          }
          return min_index;
      }

      void Dijkstra(int src, int des) {
          int dist[TOTAL];
          bool sptSet[TOTAL];
          int parent[TOTAL];

          for (int i = 0; i < TOTAL; i++) {
              parent[i] = -1;
              dist[i] = INT_MAX;
              sptSet[i] = false;
          }

          dist[src] = 0;

          for (int count = 0; count < TOTAL - 1; count++) {
              int u = minDistance(dist, sptSet);
              sptSet[u] = true;

              for (int v = 0; v < TOTAL; v++) {
                  if (!sptSet[v] && Graph[u][v] && dist[u] + Graph[u][v] < dist[v]) {
                      parent[v] = u;
                      dist[v] = dist[u] + Graph[u][v];
                  }
              }
          }

          cout << endl;
          int child[TOTAL];
          int stations = 0, intr = 0;
          printPath(parent, des, child, des, stations, intr);
          cout << endl << endl << "_____________________________________________________" << endl;
          cout << "Total Stations : " << stations << endl;
          cout << "Total Interchanges : " << intr << endl;
          cout << "_____________________________________________________" << endl;
      }

      int main() {
          memset(Graph, 0, sizeof(Graph));
          mapstations();
          joingraph();
          string start, des;
          int s, d, choice;
          cout << "_______________________________________________________________________________________________________________________" << endl;
          cout << "_______________________________________________________________________________________________________________________" << endl << endl;
          cout << "\t\t" << " **     ** ****** ****** *****    *****      ****       **    **  ******  ******    ****** " << endl;
          cout << "\t\t" << " **** **** **       **   **  **  **   **    **          **    **    **    **   **   **     " << endl;
          cout << "\t\t" << " ** *** ** ****     **   *****   **   **   **    *****  **    **    **    **    **  ****   " << endl;
          cout << "\t\t" << " **     ** **       **   ** **   **   **    **  **  **  **    **    **    **   **   **     " << endl;
          cout << "\t\t" << " **     ** ******   **   **  **   *****      ***    **  ********  ******  ******    ****** " << endl;
          cout << "_______________________________________________________________________________________________________________________" << endl;
          cout << "_______________________________________________________________________________________________________________________" << endl << endl;
          cout << "Please select an option:" << endl;

          do {
              cout << "1. Find path from source to destination" << endl;
              cout << "2. Display list of all stations along with interchanges" << endl;
              cout << "3. Exit" << endl << endl << endl;
              cin >> choice;
              switch (choice) {
                  case 1:
                      do {
                          cout << "Enter starting station : ";
                          getline(cin >> ws, start);
                          s = check(start);
                          if (s == -1)
                              cout << "Invalid input...Try again" << endl;
                      } while (s == -1);
                      do {
                          cout << "\nEnter destination : ";
                          getline(cin >> ws, des);
                          d = check(des);
                          if (d == -1)
                              cout << "Invalid input...Try again" << endl;
                      } while (d == -1);

                      if (station[d] == station[s]) {
                          int k;
                          cout << "\nEntered source and destination are same" << endl;
                          cout << "Enter 1 to continue or any key to exit" << endl;
                          cin >> k;
                          if (k == 1)
                              break;
                          else {
                              choice = 3;
                              break;
                          }
                      }

                      cout << "__________________________________________________________________________________" << endl;
                      cout << "Starting Station : " << station[s] << "\t\t Destination : " << station[d] << endl;
                      cout << "__________________________________________________________________________________" << endl;
                      int k;
                      cout << "1. Shortest Path" << endl;
                      cout << "2. Path having minimum interchanges" << endl << endl << endl;
                      do {
                          cin >> k;
                          if (k == 1)
                              Dijkstra(s, d);
                          else if (k == 2) {
                              for (int i = 0; i < interchanges.size(); i++) {
                                  Graph[interchanges[i].first][interchanges[i].second] = 50;
                              }
                              Dijkstra(s, d);
                              for (int i = 0; i < interchanges.size(); i++) {
                                  Graph[interchanges[i].first][interchanges[i].second] = 4;
                              }
                          } else {
                              cout << "Please enter a valid choice:" << endl;
                              k = 3;
                          }
                      } while (k == 3);
                      cout << "\n\nDo you want to continue(1/0)";
                      cin >> k;
                      if (k != 1)
                          choice = 3;
                      break;
                  case 2:
                      int c;
                      cout << "Select Metro Line : " << endl;
                      for (int f = 0; f < terminals.size(); f = f + 2) {
                          if (f / 2 == 3)
                              cout << f / 2 << ". " << color(terminals[f]) << " Line (Towards Vaishali)" << endl;
                          else
                              cout << f / 2 << ". " << color(terminals[f]) << " Line" << endl;
                      }
                      cout << "11. All Stations" << endl;
                      do {
                          cin >> c;
                          if (c < 0 || c > 10)
                              cout << "Please enter a valid choice:" << endl;
                      } while (c < 0 || c > 10);

                      int start, end;
                      if (c >= 0 && c < 10) {
                          start = terminals[2 * c];
                          end = terminals[2 * c + 1];
                      } else {
                          start = terminals[0];
                          end = terminals[terminals.size() - 1];
                      }

                      if (c != 10) {
                          cout << color(terminals[2 * c]) << " Line Stations : " << endl;
                          for (int i = start; i <= end; i++) {
                              cout << i - start + 1 << "->" << station[i];
                              for (int j = 0; j < interchanges.size(); j++) {
                                  if (i == interchanges[j].first) {
                                      cout << " (Interchange : " << color(i) << " Line --> " << color(interchanges[j].second) << " Line) ";
                                  }
                              }
                              cout << endl;
                          }
                      } else {
                          cout << "All Stations : " << endl;
                          for (int i = 0; i < TOTAL; i++) {
                              cout << i + 1 << "->" << station[i];
                              for (int j = 0; j < interchanges.size(); j++) {
                                  if (i == interchanges[j].first) {
                                      cout << " (Interchange : " << color(i) << " Line --> " << color(interchanges[j].second) << " Line) ";
                                  }
                              }
                              cout << endl;
                          }
                      }
                      cout << "\n\nDo you want to continue(1/0)";
                      cin >> c;
                      if (c != 1)
                          choice = 3;
                      break;
                  case 3:
                      cout << "Exiting...";
                      break;
                  default:
                      cout << "Please enter a valid choice" << endl;
              }
          } while (choice != 3);
          return 0; // Added to address the error and ensure the main function returns a value
      }
