//
// Created by 罗瑞笛 on 2024/1/20.
//

#ifndef TINYRENDERER_MODEL_H
#define TINYRENDERER_MODEL_H

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "geometry.h"

class Model {

private:
    std::vector<Vec3f> verts_;              /// 每个vertex  xyz [世界坐标--- 范围-1-1]
    std::vector<std::vector<int> > faces_;  /// 构成一个三角形的index/// verts的下标

public:
    Model(const char* file_path);
    int nverts();
    int nfaces();
    std::vector<int> face(int idex);
    Vec3f vert(int i);                      /// 返回verts_ 中的 vertex
    ~Model() ;


};


#endif //TINYRENDERER_MODEL_H
