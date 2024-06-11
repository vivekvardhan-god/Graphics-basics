#include <algorithm>
#include <fstream>

#include <SDL2/SDL.h>
#include "ppmviewerFunc.h"

int main(int argc,char* args[]){
    ppmviewer("mandelbrotHSV.ppm");
    return 0;
}