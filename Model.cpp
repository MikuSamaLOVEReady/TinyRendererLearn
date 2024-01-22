//
// Created by 罗瑞笛 on 2024/1/20.
//

#include "Model.h"

Model::Model(const char *file_path): verts_(), faces_() {
    std::ifstream ifile;
    ifile.open(file_path);

    if(ifile.fail())
    {
        std::cout << " can not open file" << std::endl;
        return;
    }

    std::string  line;

    while( !ifile.eof() )
    {
        std::getline(ifile , line);    /// 读取一行行data标准操作
        std::istringstream iss(line.c_str());      /// iss 就可以自动切断
        char trash;                          ///  吸收标志符号

        if( !line.compare(0,2,"v ") )       ///读入vertex data
        {
            iss >> trash;
            Vec3f v;
            for(int i =0 ; i< 3; ++i)
              iss >>  v.raw[i];
            verts_.push_back(v);
        }
        else if ( !line.compare(0,2,"f "))              /// face 一个三角片的index
        {
            iss>>trash;
            std::vector<int> v;
            int index;
            int trash2;
            while( iss >> index >> trash >> trash2 >> trash >> trash2)
            {
                index--;
                v.push_back(index);
            }
            faces_.push_back(v);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

int Model::nfaces() {
    return faces_.size();
}

int Model::nverts() {
    return verts_.size();
}

std::vector<int> Model::face(int idex) {
    return  faces_[idex];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}


Model::~Model() {

}
