//
// Created by ����� on 2024/1/20.
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
    std::vector<Vec3f> verts_;              /// ÿ��vertex  xyz [��������--- ��Χ-1-1]
    std::vector<std::vector<int> > faces_;  /// ����һ�������ε�index/// verts���±�

public:
    Model(const char* file_path);
    int nverts();
    int nfaces();
    std::vector<int> face(int idex);
    Vec3f vert(int i);                      /// ����verts_ �е� vertex
    ~Model() ;


};


#endif //TINYRENDERER_MODEL_H
