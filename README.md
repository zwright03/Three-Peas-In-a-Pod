This is a sorting visualizer and performance analyzer

Features include:
- Visualizing the sorting processes of merge and quick sort step-by-step
- Comparing the performance of the algorithms on a large data set in three scenarios
      - Unsorted
      - Already sorted
      - Reverse sorted

Software requirements include:
- CMake 3.8+
- Git (Required for CMake to fetch SFML)
- C++17 or higher

Hardware requirements include:
- System with at least 8GB of ram
- Modern CPU able to handle visualization and large datasets

The following resource files are included in the repository
- resources/data/2020_taxi_data.csv (Dataset file)
- resources/fonts/Roboto-Bold.ttf (Font file)

The workflow in the following order is:
- Welcome screen with descriptive information
- Merge and quick sort visualization
- Performance testing on the dataset in different scenarios
- Results screen with bar graphs comparing time and memory usage

Build Instructions:
1. Ensure 'git' is installed and accessible from the command line.
2. Ensure CMake version 3.8+ is installed.
3. C++ Compiler: Ensure you have a compiler that supports C++17 or later.
4. Run CMake to configure the project. CMake will automatically fetch the SFML dependency.