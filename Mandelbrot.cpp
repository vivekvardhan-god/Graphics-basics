#include <iostream>
#include <string>
#include "color.h"
#include "vec3.h"
#include "HSV2RGB.h"

int MaxIter = 20;
const int R = 2;
int mandelbrot(const vec3& c,const int maxIter){
    int iter = 0;
    auto z = vec3(0,0,0);
    while(z.length_squared()<4 && iter<maxIter){
        double x = z[0]*z[0] - z[1]*z[1] + c[0];
        double y =  2*z[0]*z[1] + c[1];
        z = vec3(x,y,0);
        iter++;
    }
    return iter;
}

int julia(const vec3& c,vec3& z,const int maxIter){
    int iter = 0;
    while(z.length_squared()<R*R && iter<maxIter){
        double x = z[0]*z[0] - z[1]*z[1] + c[0];
        double y =  2*z[0]*z[1] + c[1];
        z = vec3(x,y,0);
        iter++;
    }
    return iter;
}

void Color(int m,int colorchoice){
    if(colorchoice==0){
        auto pixel_color = color(1.0 - (double(m)/MaxIter), 1.0 - (double(m)/MaxIter), 1.0 - (double(m)/MaxIter));
        write_color(std::cout, pixel_color);
    }
    if(colorchoice==1){
        float hue = 360*(double(m))/MaxIter;
        float saturation = 100;
        float value = (m<MaxIter)?100:0;
        auto RGBvec = HSVtoRGB(hue,saturation,value);
        write_color(std::cout, RGBvec);
    }
}

void plot(int Maxiter,std::string s,int colorchoice,int Fractalchoice){
    const char* str = s.c_str(); 
    freopen(str,"w",stdout);
    int MaxIter = Maxiter;
    int maxwidth = 600;
    int maxheight = 400;
    int RE_START = -2;
    int RE_END = 1;
    int IM_START = -1;
    int IM_END = 1;
    std::cout<<"P3"<<"\n";
    std::cout<<maxwidth<<" "<<maxheight<<"\n";
    std::cout<<255<<"\n";
    for(int j=0;j<maxheight;++j){
        std::clog<<"\rScanlines remaining: "<<(maxheight-j)<<' '<<std::flush;
        for(int i=0;i<maxwidth;i++){
            if(Fractalchoice==0){
                auto c = vec3(RE_START+(double(i)/maxwidth)*(RE_END-RE_START),IM_START+(double(j)/maxheight)*(IM_END-IM_START),0);
                int m = mandelbrot(c,MaxIter);
                Color(m,colorchoice);
            }
            if(Fractalchoice==1){
                auto c = vec3(0.285,0.01,0);
                auto z = vec3(RE_START+(double(i)/maxwidth)*(RE_END-RE_START),IM_START+(double(j)/maxheight)*(IM_END-IM_START),0);
                int m = julia(c,z,MaxIter);
                Color(m,colorchoice);
            }
        }
    }
    std::clog<<"\rDone.                      \n"<<std::flush;
}

/*int main(){
    plot(20,"mandelbrotBW.ppm",0); Run this for a black and white mandelbrot image
    plot(20,"mandelbrotHSV.ppm",1); Run this for a colored hue mandelbrot image
    return 0;
}*/