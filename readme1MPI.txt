### README

This project involves finding K shortest paths in a graph using parallel programming techniques with MPI and OpenMP. The code provided reads graph data from a file, constructs an adjacency list representation of the graph, and then utilizes Dijkstra's algorithm to find K shortest paths between specified pairs of nodes.

#### Dependencies
- **MPI (Message Passing Interface)**: Used for parallel computing and communication between processes.
- **OpenMP**: Used for multi-threading within each MPI process.
- **C++ Standard Library**: Utilized extensively for data structures and I/O operations.

#### Code Overview
The main functionalities of the code include:

1. **Reading Graph Data**
   - The program reads graph data from a specified file (`Email-EuAll.txt`, `Email-Enron.txt`, or `DoctorWho.csv`). The data can be in the form of node indices or node names (for DoctorWho dataset).
   - The function `getEdgesFromFile` reads the file, parses each line, and constructs an adjacency list representation of the graph.

2. **Finding K Shortest Paths**
   - The program employs Dijkstra's algorithm to find the K shortest paths between given start and end nodes.
   - The `findKShortest` function implements Dijkstra's algorithm using a priority queue and tracks the K shortest paths.
   - Pathfinding is parallelized using OpenMP, enabling multiple threads to explore adjacent nodes concurrently.

3. **MPI Communication**
   - The program utilizes MPI for parallel computing, distributing tasks across multiple processes (nodes).
   - Each process focuses on finding K shortest paths for specific node pairs, leveraging MPI to coordinate and synchronize computations.

4. **Execution**
   - The program is executed with MPI, allowing multiple instances (processes) to run simultaneously on distributed systems.
   - Communication among processes is managed using MPI's communication primitives (`MPI_Init`, `MPI_Comm_size`, `MPI_Comm_rank`, `MPI_Bcast`, `MPI_Reduce`, etc.).

#### Running the Program
1. **Compilation**:
   - Compile the program with an MPI-compatible C++ compiler (e.g., `mpic++`) and ensure OpenMP support (`-fopenmp` flag).

2. **Execution**:
   - Run the compiled executable with `mpirun` (or equivalent) to specify the number of processes:
     ```
     mpirun -n <num_processes> ./executable
     ```

3. **Input**:
   - Choose the dataset (`Email-EuAll`, `Email-Enron`, or `DoctorWho`) and specify the number of shortest paths (K) to find.
   - The program will read the graph data, compute K shortest paths, and output results.

#### Notes
- Ensure that MPI and OpenMP are properly installed and configured on your system.
- Adjust the number of MPI processes (`-n`) based on available resources (cores/nodes) to achieve optimal parallel performance.
- Results will be printed to the console, including the K shortest path lengths and paths for each node pair.


