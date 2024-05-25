#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "MandelbrotB&W.cpp"

namespace fs = std::filesystem;

void createFilesInDirectory(const std::string& directory, int numberOfFiles) {
    // Ensure the directory exists
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    for (int i = 1; i <= numberOfFiles; ++i) {
        // Construct the file name and path
        std::string filePath = directory + "/file" + std::to_string(i) + ".ppm";
        
        // Create and open the file
        std::ofstream outFile(filePath);
        
        // Check if the file was successfully created
        if (outFile.is_open()) {
            plotBW(i,filePath);
            outFile.close();
        } else {
            std::cerr << "Failed to create file: " << filePath << std::endl;
        }
    }
}

int main() {
    std::string directoryPath = "FramesB&W";
    int numberOfFiles = 120;
    
    createFilesInDirectory(directoryPath, numberOfFiles);
    
    return 0;
}
