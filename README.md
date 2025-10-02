🚇 Delhi Metro Route Finder
<p align="center"> <img src="./images/logo/train.png" width="120" alt="Delhi Metro Logo"> </p> <p align="center"> <b>A C++ based console application to calculate shortest paths, fares, and interchanges in the Delhi Metro Network.</b> </p> <p align="center"> <!-- Badges --> <img src="https://img.shields.io/github/stars/your-username/delhi-metro-route-finder?style=for-the-badge&logo=github" alt="GitHub Stars"> <img src="https://img.shields.io/github/forks/your-username/delhi-metro-route-finder?style=for-the-badge&logo=github" alt="GitHub Forks"> <img src="https://img.shields.io/github/issues/your-username/delhi-metro-route-finder?style=for-the-badge&logo=github" alt="GitHub Issues"> <img src="https://img.shields.io/github/license/your-username/delhi-metro-route-finder?style=for-the-badge" alt="License"> </p>
✨ Features

    ✅ Parse Delhi Metro CSV data into graph
    
    ✅ Build connections between stations including interchanges
    
    ✅ Compute routes using Dijkstra’s Algorithm
    
    ✅ Three routing modes:
    
            📏 Shortest Distance
      
            💰 Minimum Fare
      
            🔄 Minimum Interchange
      
    ✅ Displays distance, fare, and path with line details


📂 Project Structure

    ├── Delhi-Metro-Network.csv   # Metro data
    
    ├── delhi.cpp                  # Source code
    
    ├── README.md                  # Documentation
    
    └── images/                    # Logos / screenshots

    
🛠️ Tech Stack

    Language: C++ (C++11+)
    
    Core Concepts: Graphs, Priority Queue, Dijkstra’s Algorithm
    
    Data: CSV-based station dataset
    
🚀 Getting Started

1️⃣ Clone repository

    git clone https://github.com/your-username/delhi-metro-route-finder.git
    cd delhi-metro-route-finder

2️⃣ Build

    g++ delhi.cpp -o delhi

3️⃣ Run

    ./delhi


📖 Usage Example

    Choose routing option:
    1. Shortest distance route
    2. Minimum fare route
    3. Minimum line interchange route
    Enter choice (1/2/3): 1
    Enter source station ID: 2
    Enter destination station ID: 25
    
    Shortest distance: 18.3 km
    Fare: Rs 40/-
    Path: Kashmere Gate (Yellow Line) -> Rajiv Chowk (Blue Line) -> Mandi House (Violet Line) -> Lajpat Nagar (Pink Line)


📊 Fare Rules
    
    | Distance (km) | Fare (₹) |
    | ------------- | -------- |
    | 0 - 2         | 10       |
    | 2 - 5         | 20       |
    | 5 - 12        | 30       |
    | 12 - 21       | 40       |
    | 21 - 32       | 50       |
    | > 32          | 60       |


🤝 Contributing

Contributions are always welcome!

    1.🍴 Fork the repo
    
    2.🌿 Create a new branch (feature/your-feature)
    
    3.💾 Commit changes
    
    4.🚀 Push to branch
    
    5.🔁 Open a Pull Request


📜 License

  Licensed under the MIT License.

👨‍💻 Author

  RISHI SRIVASTAWA


✨ Delhi Metro made smarter with Graphs & C++! ✨
