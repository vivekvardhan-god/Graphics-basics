#include <iostream>
#include "vec3.h"


vec3 HSVtoRGB(float H, float S,float V){
    /*
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        std::clog<<"The givem HSV values are not in valid range"<<"\n";
        return;
    }
    */
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-abs((int(H)/60)% 2)-1);
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }

    double R = (r+m);
    double G = (g+m);
    double B = (b+m);

    return vec3(R,G,B); 
}