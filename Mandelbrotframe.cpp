#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "Mandelbrot.cpp"

namespace fs = std::filesystem;
int colorchoice = 0;
int fractalchoice = 0;

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
            plot(i,filePath,colorchoice,fractalchoice);
            outFile.close();
        } else {
            std::cerr << "Failed to create file: " << filePath << std::endl;
        }
    }
}

int main() {
    std::cout<<"Enter your colorchoice: "<<"\n";
    std::cout<<"0 - Black and White"<<"\n";
    std::cout<<"1 - Colored with hue"<<"\n";
    std::cin>>colorchoice;
    std::cout<<"Enter your Fractalchoice: "<<"\n";
    std::cout<<"0 - Mandelbrot"<<"\n";
    std::cout<<"1 - Julia Sets"<<"\n";
    std::cin>>fractalchoice;
    std::string directoryPath;
    if(colorchoice==0 && fractalchoice==0){
        directoryPath = "FramesBW";
    }
    if(colorchoice==1 && fractalchoice==0){
        directoryPath = "FramesHSV";
    }
    if(colorchoice==0 && fractalchoice==1){
        directoryPath = "FramesBWJ";
    }
    if(colorchoice==1 && fractalchoice==1){
        directoryPath = "FramesHSVJ";
    }
    int numberOfFiles = 96;
    
    createFilesInDirectory(directoryPath, numberOfFiles);
    
    return 0;
}
