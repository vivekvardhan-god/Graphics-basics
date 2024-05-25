#include <iostream>
#include <string>
#include "color.h"
#include "vec3.h"

int MaxIter = 20;
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

void plotBW(int Maxiter,std::string s){
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
            auto c = vec3(RE_START+(double(i)/maxwidth)*(RE_END-RE_START),IM_START+(double(j)/maxheight)*(IM_END-IM_START),0);
            int m = mandelbrot(c,MaxIter);
            auto pixel_color = color(1.0 - (double(m)/MaxIter), 1.0 - (double(m)/MaxIter), 1.0 - (double(m)/MaxIter));
            write_color(std::cout, pixel_color);
        }
    }
    std::clog<<"\rDone.                      \n";
}

/*
int main(){
    plot(20,"mandelbrot.ppm");
    return 0;
}
*/