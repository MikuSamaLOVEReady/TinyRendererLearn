#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0 , int y0 , int x1 , int y1 , TGAImage& img , TGAColor color)
{
    bool steep = false;
    if( std::abs(x0-x1) < std::abs(y0-y1) ){
        std::swap(x0 ,y0);
        std::swap(x1 ,y1);
        steep = true;
    }

    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

   for(int x = x0 ; x <= x1 ; x++)
   {
        float t = (x-x0) / (float)(x1 - x0);
        int y = y1 * t + (1-t)*y0;
        if(steep)
            img.set(y, x, color);
        else
            img.set(x,y,color);
   }
}

/// int dx = x1-x0;
/// int dy = y1-y0;
/// float derror = std::abs(dy/float(dx));  每次移动一个像素，所产生的y方向的像素改变长度
///                 derror > 0.5 半个像素的时候 ，则表示y需要+1了


int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	//image.set(10, 10, red);
    //line(0,0,100,100,image,red);
    line(13,20,80,40,image,white);
    line(20,13,40,80,image,red);       /// steep 问题
    line(80, 40, 13, 20, image, red);  ///左上 右下颠倒


    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

