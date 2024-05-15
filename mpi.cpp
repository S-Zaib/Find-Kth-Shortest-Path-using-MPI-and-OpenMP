#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <omp.h>
#include <time.h>
#include <mpi.h>
#include <algorithm>
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
            omp_set_num_threads(6);
            #pragma omp for schedule(dynamic)
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
                #pragma omp critical
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

// Function to convert adjacency list to vector<int> for MPI
void convertAdjListToVector(vector<vector<pair<int, int>>> &adj, vector<int> &adjArray, vector<int> &adjArraySize)
{
    for (int i = 0; i < adj.size(); i++)
    {
        adjArraySize.push_back(adj[i].size());
        for (int j = 0; j < adj[i].size(); j++)
        {
            adjArray.push_back(adj[i][j].first);
            adjArray.push_back(adj[i][j].second);
        }
    }
}

// Function to convert vector<int> to adjacency list for MPI
void convertVectorToAdjList(vector<int> &adjArray, vector<int> &adjArraySize, vector<vector<pair<int, int>>> &adj)
{
    int index = 0;
    for (int i = 0; i < adjArraySize.size(); i++)
    {
        vector<pair<int, int>> temp;
        for (int j = 0; j < adjArraySize[i]; j++)
        {
            temp.push_back({adjArray[index], adjArray[index + 1]});
            index += 2;
        }
        adj.push_back(temp);
    }
}

// Driver Code
int main(int argc, char *argv[])
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // Map to store the node names and their corresponding IDs
    unordered_map<string, int> nodeNameToID;
    // Given Input
    int N, K = 3;
    vector<vector<pair<int, int>>> adj;
    int choice;
    if (world_rank == 0)
    {
        cout << "Choose DataSet:\n 1. Email-EuAll\n 2. Email-Enron\n 3. DoctorWho\n Enter Choice:";
        cin >> choice;
        cout << "Enter number of paths:";
        cin >> K;
    }

    // Broadcast the choice and K
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);

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
    }

    vector<int> adjArray;
    vector<int> adjArraySize;
    vector<char> serializedData;
    if (world_rank == 0)
    {
        // Master process
        clock_t t;
        t = clock();

        getEdgesFromFile(adj, N, file, nodeNameToID, chosen == 2);

        t = clock() - t;
        double exec_time = ((double)t) / CLOCKS_PER_SEC;
        cout << "Time taken by file reading is : " << fixed << exec_time << setprecision(5) << endl;

        // Convert adjacency list to vector<int> for MPI
        convertAdjListToVector(adj, adjArray, adjArraySize);

        if (chosen == 2)
        {
            // Serialize nodeNameToID
            for (const auto &pair : nodeNameToID)
            {
                std::copy(pair.first.begin(), pair.first.end(), std::back_inserter(serializedData));
                serializedData.push_back('\0'); // null character as delimiter
                int value = pair.second;
                serializedData.insert(serializedData.end(), reinterpret_cast<const char *>(&value), reinterpret_cast<const char *>(&value) + sizeof(int));
            }
        }
    }

    if (chosen == 2)
    {
        // Broadcast the size of nodeNameToID
        int dataSize;
        if (world_rank == 0)
        {
            dataSize = serializedData.size();
        }
        MPI_Bcast(&dataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Resize the serializedData vector
        if (world_rank != 0)
        {
            serializedData.resize(dataSize);
        }

        // Broadcast the serializedData
        MPI_Bcast(&serializedData[0], dataSize, MPI_CHAR, 0, MPI_COMM_WORLD);

        // Deserialize nodeNameToID
        int index = 0;
        while (index < dataSize)
        {
            string nodeName;
            while (serializedData[index] != '\0')
            {
                nodeName += serializedData[index];
                index++;
            }
            index++;
            int nodeID = *reinterpret_cast<const int *>(&serializedData[index]);
            index += sizeof(int);
            nodeNameToID[nodeName] = nodeID;
        }
    }

    int start_node[3][10] = {{2, 503, 0, 590, 1001, 64117, 512, 133574, 10, 112380},
                             {2, 503, 0, 590, 17063, 2160, 16618, 17213, 240, 2529},
                             {getNodeID("79B Aickman Road", nodeNameToID), getNodeID("Saibra", nodeNameToID), getNodeID("Grun", nodeNameToID), getNodeID("Saibra", nodeNameToID), getNodeID("Cassie Rice", nodeNameToID), getNodeID("Donna Noble", nodeNameToID), getNodeID("Amy Pond", nodeNameToID), getNodeID("Wirrn", nodeNameToID), getNodeID("George Patrick Barnham", nodeNameToID), getNodeID("Evelyn Smythe", nodeNameToID)}};
    int end_node[3][10] = {{4, 3255, 14247, 4039, 130778, 77014, 27035, 2659, 115474, 366},
                           {4, 3255, 14247, 4039, 30472, 11016, 16470, 28442, 17203, 13696},
                           {getNodeID("The Alliance (The Pandorica Opens)", nodeNameToID), getNodeID("Aggedor (The Curse of Peladon)", nodeNameToID), getNodeID("Weed creature", nodeNameToID), getNodeID("Evelyn Smythe", nodeNameToID), getNodeID("Xoanon", nodeNameToID), getNodeID("Zoe Heriot", nodeNameToID), getNodeID("The Brigadier", nodeNameToID), getNodeID("Zygon Osgood", nodeNameToID), getNodeID("Vicki Pallister", nodeNameToID), getNodeID("George Patrick Barnham", nodeNameToID)}};

    // Broadcast the number of nodes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int size = adjArray.size();
    int sizeSize = adjArraySize.size();

    // Broadcast the size of the vectors
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sizeSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Resize the vectors
    if (world_rank != 0)
    {
        adjArray.resize(size);
        adjArraySize.resize(sizeSize);
    }

    // Broadcast the vectors
    MPI_Bcast(&adjArray[0], size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&adjArraySize[0], sizeSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Convert vector<int> to adjacency list for MPI
    convertVectorToAdjList(adjArray, adjArraySize, adj);

    clock_t t, t2;
    t = clock();
    // Each worker process finds K shortest paths for a pair of nodes
    cout << "Process " << world_rank << " Start:" << start_node[chosen][world_rank] << " End:" << end_node[chosen][world_rank] << endl;
    findKShortest(adj, N, K, start_node[chosen][world_rank], end_node[chosen][world_rank], nodeNameToID, chosen == 2);
    t = clock() - t;
    double exec_time = ((double)t) / CLOCKS_PER_SEC;
    cout << "Time taken by process " << world_rank << " is : " << fixed << exec_time << setprecision(5) << endl;

    // Get total time taken in the real world
    double total_time;
    MPI_Reduce(&exec_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        cout << "Total time taken by the program: " << fixed << total_time << setprecision(5) << endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}