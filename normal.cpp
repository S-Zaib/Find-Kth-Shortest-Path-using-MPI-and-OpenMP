#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <bits/stdc++.h>
#include <time.h>

using namespace std;

string getNodeName(int nodeID, unordered_map<string, int> &nodeNameToID)
{
    for (const auto &pair : nodeNameToID)
    {
        if (pair.second == nodeID)
        {
            return pair.first;
        }
    }
    return "";
}
int getNodeID(const string &nodeName, unordered_map<string, int> &nodeNameToID)
{
    auto it = nodeNameToID.find(nodeName);
    if (it == nodeNameToID.end())
    {
        // Assign a new ID based on the current size of the map
        int newID = nodeNameToID.size();
        nodeNameToID[nodeName] = newID;
        return newID;
    }
    return (it->second);
}

// Function to get edges from a text file
void getEdgesFromFile(vector<vector<pair<int, int>>> &adj, int &n, const string &filename, unordered_map<string, int> &nodeNameToID, bool isString = false)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Unable to open file!" << endl;
        return;
    }

    string line;
    int maxNode = -1;

    while (getline(file, line))
    {
        if (line.empty() || line[0] == '#') // Skip empty lines and comments
            continue;

        istringstream iss(line);
        string fromStr, toStr, weightStr, directedStr;
        int from, to, weight = 1; // Default weight is 1

        if (isString)
        {
            getline(iss, fromStr, ',');
            getline(iss, toStr, ',');
            if (!getline(iss, weightStr, ',').fail())
            {
                try
                {
                    weight = stoi(weightStr);
                }
                catch (const std::invalid_argument &e)
                {
                    cerr << "Warning: Invalid weight, using default value: " << weightStr << endl;
                }
            }
            getline(iss, directedStr, ',');
            if (directedStr == "undirected")
            {
                // Add the edge in both directions
                from = getNodeID(fromStr, nodeNameToID);
                to = getNodeID(toStr, nodeNameToID);

                maxNode = max(maxNode, max(from, to));
                // Ensure adj vector has enough capacity
                if (maxNode >= adj.size())
                {
                    adj.resize(maxNode + 1);
                }
                adj[from].push_back({to, weight});

                // Add the edge in the reverse direction
                swap(from, to);
                adj[from].push_back({to, weight});
            }
            else
            {
                from = getNodeID(fromStr, nodeNameToID);
                to = getNodeID(toStr, nodeNameToID);

                maxNode = max(maxNode, max(from, to));
                // Ensure adj vector has enough capacity
                if (maxNode >= adj.size())
                {
                    adj.resize(maxNode + 1);
                }
                adj[from].push_back({to, weight});
            }
        }
        else
        { // Handle text file using Function 2 logic
            iss >> from >> to >> weight;
            maxNode = max(maxNode, max(from, to));
            // Ensure adj vector has enough capacity
            if (maxNode >= adj.size())
            {
                adj.resize(maxNode + 1);
            }
            adj[from].push_back({to, weight});
        }
    }

    // Update the number of nodes based on the maximum node ID
    n = maxNode + 1;
    file.close();
}

// Function to find K shortest path lengths
void findKShortest(vector<vector<pair<int, int>>> &adj, int n, int k, int start, int end, unordered_map<string, int> &nodeNameToID, bool isString = false)
{
    // Priority Queue for Dijkstra's Algorithm
    priority_queue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, greater<pair<int, vector<int>>>> pq;

    // Pushing the start node
    pq.push({0, {start}});

    // Set to store the visited nodes
    set<vector<int>> vis;

    // K shortest path lengths
    vector<int> kShortest;

    // Store the K shortest paths
    vector<vector<int>> kPaths;

    // Dijkstra's Algorithm
    while (!pq.empty())
    {
        // Getting the top element of the priority queue
        vector<int> path = pq.top().second;
        int weight = pq.top().first;

        // Popping the top element
        pq.pop();

        // Getting the last node of the path
        int node = path[path.size() - 1];

        // If the last node is the destination
        if (node == end)
        {
            // Adding the weight to the k shortest path lengths
            kShortest.push_back(weight);

            // Adding the path to the k shortest paths
            kPaths.push_back(path);

            // If we have found k shortest path lengths
            if (kShortest.size() == k)
            {
                break;
            }
        }

        // If the node is not visited
        if (vis.find(path) == vis.end())
        {
            // Marking the node as visited
            vis.insert(path);

            // Traversing the adjacent nodes
            int size_adj = adj[node].size();
            // set threads to 6 because my cpu ryzen 5 5600 has 6 cores
            for (int i = 0; i < size_adj; i++)
            {
                auto it = adj[node][i];
                // Getting the adjacent node and weight
                int adjNode = it.first;
                int edgeWeight = it.second;

                // Pushing the adjacent node to the path
                vector<int> newPath = path;
                newPath.push_back(adjNode);

                // Pushing the new path to the priority queue
                pq.push({weight + edgeWeight, newPath});
            }
        }
    }
    // Printing the K shortest path lengths
    cout << "K shortest path lengths are: ";
    for (int i = 0; i < kShortest.size(); i++)
    {
        cout << kShortest[i] << " ";
    }

    // Printing the K shortest paths
    cout << "\nK shortest paths are:\n";
    for (int i = 0; i < kPaths.size(); i++)
    {
        cout << i + 1 << ". ";
        for (int j = 0; j < kPaths[i].size(); j++)
        {
            if (isString)
            {
                cout << getNodeName(kPaths[i][j], nodeNameToID);
                if (j < kPaths[i].size() - 1)
                {
                    cout << " -> ";
                }
            }
            else
            {
                cout << kPaths[i][j];
                if (j < kPaths[i].size() - 1)
                {
                    cout << " -> ";
                }
            }
        }
        cout << endl;
    }
}

// Driver Code
int main()
{

    // Map to store the node names and their corresponding IDs
    unordered_map<string, int> nodeNameToID;
    // Given Input
    int N, K = 3;
    vector<vector<pair<int, int>>> adj;
    int choice;
    bool isString = false;
    cout << "Choose DataSet:\n 1. Email-EuAll\n 2. Email-Enron\n 3. DoctorWho\n Enter Choice:";
    cin >> choice;
    cout << "Enter number of paths:";
    cin >> K;

    string file = "Email-EuAll.txt";
    int chosen = 0;
    // Enron
    if (choice == 2)
    {
        file = "Email-Enron.txt";
        chosen = 1;
    }
    // DoctorWho
    else if (choice == 3)
    {
        file = "DoctorWho.csv";
        chosen = 2;
        isString = true;
    }

    clock_t t, t2;
    t = clock();

    // getEdgesFromFile(file, adj, N);
    getEdgesFromFile(adj, N, file, nodeNameToID, isString);

    t = clock() - t;
    double exec_time = ((double)t) / CLOCKS_PER_SEC;
    cout << "Time taken by file reading is : " << fixed << exec_time << setprecision(5) << endl;
    // Function Call
    t = clock();

    int start_node[3][10] = {{2, 503, 0, 590, 1001, 64117, 512, 133574, 10, 112380},
                             {2, 503, 0, 590, 17063, 2160, 16618, 17213, 240, 2529},
                             {getNodeID("79B Aickman Road", nodeNameToID), getNodeID("Saibra", nodeNameToID), getNodeID("Grun", nodeNameToID), getNodeID("Saibra", nodeNameToID), getNodeID("Cassie Rice", nodeNameToID), getNodeID("Donna Noble", nodeNameToID), getNodeID("Amy Pond", nodeNameToID), getNodeID("Wirrn", nodeNameToID), getNodeID("George Patrick Barnham", nodeNameToID), getNodeID("Evelyn Smythe", nodeNameToID)}};
    int end_node[3][10] = {{4, 3255, 14247, 4039, 130778, 77014, 27035, 2659, 115474, 366},
                           {4, 3255, 14247, 4039, 30472, 11016, 16470, 28442, 17203, 13696},
                           {getNodeID("The Alliance (The Pandorica Opens)", nodeNameToID), getNodeID("Aggedor (The Curse of Peladon)", nodeNameToID), getNodeID("Weed creature", nodeNameToID), getNodeID("Evelyn Smythe", nodeNameToID), getNodeID("Xoanon", nodeNameToID), getNodeID("Zoe Heriot", nodeNameToID), getNodeID("The Brigadier", nodeNameToID), getNodeID("Zygon Osgood", nodeNameToID), getNodeID("Vicki Pallister", nodeNameToID), getNodeID("George Patrick Barnham", nodeNameToID)}};

    for (int i = 0; i < 10; i++)
    {
        t2 = clock();
        cout << "_______________________________________________________________________________________\n";
        cout << "Start Node: " << start_node[chosen][i] << " End Node: " << end_node[chosen][i] << endl;
        findKShortest(adj, N, K, start_node[chosen][i], end_node[chosen][i], nodeNameToID, isString);
        cout << "_______________________________________________________________________________________\n";
        cout << endl;
        t2 = clock() - t2;
        exec_time = ((double)t2) / CLOCKS_PER_SEC;
        cout << "Time taken by K shortest path is : " << fixed << exec_time << setprecision(5) << endl;
    }

    t = clock() - t;
    exec_time = ((double)t) / CLOCKS_PER_SEC;
    cout << "Total Time Taken is : " << fixed << exec_time << setprecision(5);

    return 0;
}