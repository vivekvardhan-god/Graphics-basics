#include <iostream>
#include "color.h"
#include "vec3.h"

int main(){
    freopen("image.ppm","w",stdout);
    int maxwidth = 256;
    int maxheight = 256;
    std::cout<<"P3"<<"\n";
    std::cout<<maxwidth<<" "<<maxheight<<"\n";
    std::cout<<255<<"\n";
    for(int i=0;i<maxwidth;++i){
        std::clog<<"\rScanlines remaining: "<<(maxheight-i)<<' '<<std::flush;
        for(int j=0;j<maxheight;j++){
            
            auto pixel_color = color(double(j)/(maxwidth-1), double(i)/(maxheight-1), 0);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog<<"\rDone.                      \n";
}