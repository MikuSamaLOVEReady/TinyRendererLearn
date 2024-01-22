#include "tgaimage.h"
#include "Model.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

const int width  = 800;
const int height = 800;

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


void line2(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}
/// int dx = x1-x0;
/// int dy = y1-y0;
/// float derror = std::abs(dy/float(dx));  每次移动一个像素，所产生的y方向的像素改变长度
///                 derror > 0.5 半个像素的时候 ，则表示y需要+1了


int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
	//image.set(10, 10, red);
    //line(0,0,100,100,image,red);
    //line(13,20,80,40,image,white);
    //line(20,13,40,80,image,red);       /// steep 问题
    //line(80, 40, 13, 20, image, red);  ///左上 右下颠倒




    Model m(argv[1]);       /// 读取模型文件
    for(int i=0  ; i<m.nfaces() ; ++i){
        std::vector<int> face = m.face(i);
        for(int j =0 ; j<3 ;j++)
        {
            Vec3f v0 = m.vert(face[j]);
            Vec3f v1 = m.vert(face[(j+1) % 3] );
            int x0 = (v0.x + 1.) * width / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            line2(x0 , y0 ,x1, y1 , image, white);
        }
    }


    std::string stest("235/215/235 363/347/363 467/458/467");
    std::istringstream iss(stest);      /// iss 就可以自动切断
    int index;
    int trash2;
    char trash;
    std::cout << "HWEE" << std::endl;
    while( iss >> index >> trash >> trash2 >> trash >> trash2 )
    {
        std::cout << "index: " << index << std::endl;
    }
    std::cout << "WOC" << std::endl;


    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

	return 0;
}

