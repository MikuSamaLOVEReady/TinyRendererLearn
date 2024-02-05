#include "tgaimage.h"
#include "Model.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

const int width  = 800;
const int height = 800;

Vec3f  light_dir(0,0,-1);  /// Light_Dir

void line(int x0 , int y0 , int x1 , int y1 , TGAImage& img , TGAColor color)
{
    ///斜率太大、绘制线条会断开
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
            img.set(y, x, color);       ///这比opencv强多了很骚的产生.tga的方法
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
/// 传统做法！ hard！
void triangle(Vec2i& t0 , Vec2i& t1 , Vec2i& t2 , TGAImage& image , TGAColor color)
{
    if (t0.y==t1.y && t0.y==t2.y) return; // I dont care about degenerate triangles
    // sort the vertices, t0, t1, t2 lower?to?upper (bubblesort yay!)
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec2i A =               t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
        }
    }
}

/// 相比-传统更快捷、 利用cross product的结果来验证、记住要归一化 ux uy uz
Vec3f barycentric(Vec2i* pts , Vec2i& P)
{
    Vec3f u = Vec3f(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x)
            ^ Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y);
    /// 经验model而已
    if( std::abs(u.z) < 1 ) return Vec3f(-1, 1, 1);
    /// 归一化 uz = 1
    return Vec3f(1.f - (u.x + u.y)/u.z  , u.y/u.z , u.x/u.z  );
}

void triangle_02(Vec2i* pts , TGAImage& image , TGAColor color) {
    /// 初始化bbox
    Vec2i bboxmin(image.get_width() - 1, image.get_height() - 1);
    Vec2i bboxmax(0, 0);
    Vec2i clam(image.get_width() - 1, image.get_height() - 1);

    /// 遍历所有的三个顶点，缩小bbox的范围
    for (int i = 0; i < 3; i++)
    {
       bboxmin.x = std::max( 0 , std::min(pts[i].x , bboxmin.x));
       bboxmin.y = std::max( 0 , std::min(pts[i].y , bboxmin.y));
       bboxmax.x = std::min( clam.x , std::max(pts[i].x , bboxmax.x));
       bboxmax.y = std::min( clam.y , std::max(pts[i].y , bboxmax.y));
    }
    std::cout << "bbox mix" <<   bboxmin.x << " " <<  bboxmin.y << std::endl;
    std::cout << "bbox max" <<   bboxmax.x << " " <<  bboxmax.y << std::endl;

    ///在Box中绘制
    Vec2i P;
    int count = 0;
    int all_points = 0;
    for( P.x = bboxmin.x ; P.x <= bboxmax.x ; ++P.x){
        for( P.y = bboxmin.y ; P.y <= bboxmax.y ; ++P.y)
        {
           Vec3f res = barycentric( pts , P);
           if(res.x <0 || res.y <0 || res.z < 0 ) {
               count++;
               continue;
           }
            image.set(P.x , P.y , color);
        }
    }
}

void Render_ColorFace( Model& m , TGAImage& img)
{
    for(int i=0 ; i<m.nfaces() ; ++i)
    {
         std::vector<int> face = m.face(i);
         Vec2i screen_coords[3];
         Vec3f world_coords[3];
         for(int j=0 ; j<3 ; j++)
         {
             Vec3f world_pos = m.vert(face[j]); /// world_pos --->【 -1 <---> 1 】
             screen_coords[j] = Vec2i((world_pos.x + 1.) * width / 2., (world_pos.y + 1) * height/2.);
             world_coords[j] = world_pos;
         }
        Vec3f  n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]); /// 这个法线的方向 叉乘与 存储的index顺序有关
        n.normalize();
        float intensity = n * light_dir;
        if(intensity > 0){
            /// 线性光色差值
            triangle_02( screen_coords , img , TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);

    Vec2i pts[3] = {Vec2i(10,10) , Vec2i(100,30) , Vec2i (190 , 160)};
    //triangle_02(pts, image , red);

    Model m(argv[1]);
    Render_ColorFace(m , image);


    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

	return 0;
}

