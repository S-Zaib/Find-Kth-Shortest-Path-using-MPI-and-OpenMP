### README

This C++ project focuses on finding K shortest paths in a graph using Dijkstra's algorithm, with support for reading graph data from different datasets (`Email-EuAll`, `Email-Enron`, `DoctorWho`) specified by the user. The program utilizes file I/O, graph representation using adjacency lists, and prioritized queue operations to efficiently determine the shortest paths.

#### Dependencies
- **C++ Standard Library**: Utilized for data structures, I/O operations, and algorithmic functionalities.
  
#### Code Overview
The key functionalities of the program include:

1. **Reading Graph Data**
   - The `getEdgesFromFile` function reads graph data from a specified file and constructs an adjacency list representation of the graph. It supports both integer and string-based node identifiers.

2. **Dijkstra's Algorithm for K Shortest Paths**
   - The `findKShortest` function implements Dijkstra's algorithm to compute the K shortest paths between specified node pairs. It uses a priority queue to efficiently explore paths based on path weights.

3. **Main Functionality**
   - The `main` function orchestrates the overall execution of the program.
   - It prompts the user to choose a dataset (`Email-EuAll`, `Email-Enron`, `DoctorWho`) and specifies the number of shortest paths (K) to find.
   - After reading the graph data and computing the shortest paths, it outputs the results to the console.

#### Running the Program
1. **Compilation**
   - Compile the program using a C++ compiler, ensuring support for standard libraries and file I/O functionalities.

2. **Execution**
   - Execute the compiled binary.
   - Follow the prompts to choose a dataset and specify the number of shortest paths (K) to compute.

3. **Input**
   - Choose a dataset by entering the corresponding number (`1`, `2`, `3`).
   - Specify the number of shortest paths (`K`) to find.

4. **Output**
   - The program outputs the computed K shortest path lengths and paths for predefined node pairs based on the chosen dataset.
   - Additionally, it displays the execution time for reading the graph data and computing the shortest paths.

#### Notes
- Ensure that the required datasets (`Email-EuAll.txt`, `Email-Enron.txt`, `DoctorWho.csv`) are accessible in the working directory or provide the full file path.
- Adjust the predefined node pairs (`start_node` and `end_node`) for custom testing scenarios.
- This project is designed for educational purposes to demonstrate graph algorithms and file handling in C++.

