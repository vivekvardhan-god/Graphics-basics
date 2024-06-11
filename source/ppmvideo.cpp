#include <algorithm>
#include <fstream>
#include <SDL2/SDL.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct Header {
    bool is_ascii;
    int width = -1;
    int height = -1;
    int max_val = -1;
};

struct Pixel {
    int r = -1;
    int g = -1;
    int b = -1;

    // Convert RGB values to BGR for SDL
    int rgb2bgr() {
        return (255 << 24) | (b << 16) | (g << 8) | r;
    }

    void reset() {
        r = -1;
        g = -1;
        b = -1;
    }
};

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

bool Init(int width, int height) {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("ppmviewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

void Close() {
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

unsigned int* ParsePPM(std::ifstream& file, Header& h) {
    char c;

    if (file.is_open()) {
        file.get(c);
        if (c != 'P') {
            printf("File is not in PPM format.\n");
        }
        file.get(c);
        if (c == '3') {
            h.is_ascii = true;
        } else if (c == '6') {
            h.is_ascii = false;
        } else {
            printf("Grayscale formats (P1, P4, P2, P5) are unsupported.\n");
            return NULL;
        }

        char num[10];
        int i = 0;
        int val;
        memset(num, 0, sizeof(num));

        while ((h.width == -1 || h.height == -1 || h.max_val == -1) && file.get(c)) {
            switch (c) {
                case '#':
                    while (file.get(c)) {
                        if (c == '\n') break;
                    }
                    break;
                case '\n':
                case ' ':
                    val = atoi(num);
                    if (val != 0 || (num[0] == '0' && num[1] == '\0')) {
                        if (h.width == -1) {
                            h.width = val;
                        } else if (h.height == -1) {
                            h.height = val;
                        } else if (h.max_val == -1) {
                            h.max_val = val;
                        }

                        memset(num, 0, sizeof(num));
                        i = 0;
                    }
                    break;
                default:
                    if (isdigit(c)) {
                        num[i] = c;
                        i++;
                    }
                    break;
            }
        }

        unsigned int* pixmap = new unsigned int[h.width * h.height];
        int j = 0;
        memset(pixmap, 0, h.width * h.height * sizeof(unsigned int));
        Pixel p;

        while (file.get(c)) {
            if (h.is_ascii) {
                switch (c) {
                    case '#':
                        while (file.get(c)) {
                            if (c == '\n') break;
                        }
                        break;
                    case '\n':
                    case ' ':
                        val = atoi(num);
                        if (val != 0 || (num[0] == '0' && num[1] == '\0')) {
                            if (p.r == -1) {
                                p.r = std::min(val, h.max_val);
                            } else if (p.g == -1) {
                                p.g = std::min(val, h.max_val);
                            } else if (p.b == -1) {
                                p.b = std::min(val, h.max_val);

                                pixmap[j++] = p.rgb2bgr();

                                p.reset();
                            }

                            memset(num, 0, sizeof(num));
                            i = 0;
                        }
                        break;
                    default:
                        if (isdigit(c)) {
                            num[i] = c;
                            i++;
                        }
                        break;
                }
            } else {
                printf("P6 file format is unsupported.\n");
                return NULL;
            }
        }

        return pixmap;
    } else {
        printf("Cannot open specified file!\n");
        return NULL;
    }
}

std::vector<unsigned int*> LoadPPMFrames(const std::string& directory, Header& h) {
    std::vector<std::string> filenames;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".ppm") {
            filenames.push_back(entry.path().string());
        }
    }

    // Convert filenames to C-style strings
    std::vector<const char*> c_filenames;
    for (const auto& filename : filenames) {
        c_filenames.push_back(filename.c_str());
    }

    // Sort C-style strings alphabetically
    std::sort(c_filenames.begin(), c_filenames.end(), [](const char* a, const char* b) {
        if(std::string(a).size() == std::string(b).size()){
            return std::strcmp(a, b) < 0;
        }
        else{
            return std::string(a).size() < std::string(b).size();
        }
    });

    // Print sorted filenames using printf
    for (const auto& filename : c_filenames) {
        printf("Sorted filename: %s\n", filename);
    }

    // Load frames in sorted order
    std::vector<unsigned int*> frames;
    for (const auto& filename : c_filenames) {
        std::ifstream file(filename);
        unsigned int* pixmap = ParsePPM(file, h);
        file.close();
        if (pixmap) {
            frames.push_back(pixmap);
        }
    }

    return frames;
}




void ppmviewer(const std::string& directory, int fps) {
    Header h;
    std::vector<unsigned int*> frames = LoadPPMFrames(directory, h);

    if (frames.empty()) {
        printf("No valid PPM files found in the directory!\n");
        return;
    }

    if (!Init(h.width, h.height)) {
        printf("Failed to initialize!\n");
    } else {
        bool quit = false;
        SDL_Event e;

        Uint32 rmask, gmask, bmask, amask;
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0;

        int depth = 32;
        int pitch = 4 * h.width;
        int frame_delay = 1000 / fps;

        size_t frame_index = 0;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(frames[frame_index], h.width, h.height, depth, pitch,
                                                         rmask, gmask, bmask, amask);

            SDL_BlitSurface(surf, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);

            SDL_FreeSurface(surf);

            frame_index = (frame_index + 1) % frames.size();
            SDL_Delay(frame_delay);
        }
    }

    for (auto pixmap : frames) {
        delete[] pixmap;
    }

    Close();
}

int main(int argc, char* args[]) {
    if (argc < 3) {
        printf("Supply a directory path and frame rate as arguments. Example:\nppmviewer example\\directory 30\n");
        return 0;
    }

    std::string directory = args[1];
    int fps = std::atoi(args[2]);
    ppmviewer(directory, fps);

    return 0;
}
