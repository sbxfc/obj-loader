//
//  RGObjLoader.h
//  obj-loader
//
//  Created by sbx_fc on 14-10-20.
//  Copyright (c) 2014年 rungame. All rights reserved.
//

#ifndef __obj_loader__RGObjLoader__
#define __obj_loader__RGObjLoader__

#include <cstdlib>
#include <cstring>
#include <cassert>

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

namespace rg
{
    typedef struct
    {
        std::vector<unsigned int>   indices;
        std::vector<float>          positions;
        std::vector<float>          normals;
        std::vector<float>          texcoords;
        std::vector<int>            material_ids;
    } mesh_r;
    
    typedef struct
    {
        std::string name;
        
        float ambient[3];
        float diffuse[3];
        float specular[3];
        float transmittance[3];
        float emission[3];
        float shininess;
        float ior;
        float dissolve;
        int illum;
        
        std::string ambient_texname;
        std::string diffuse_texname;
        std::string specular_texname;
        std::string normal_texname;
        std::map<std::string, std::string> unknown_parameter;
    } material_r;
    
    //加载的面的信息
    typedef struct
    {
        std::string name;
        mesh_r       mesh;
    } shape_r;
    
    
    
    class MaterialReader
    {
    public:
        MaterialReader(){}
        virtual ~MaterialReader(){}
        virtual std::string operator() (const std::string& matId,std::vector<material_r>& materials,std::map<std::string, int>& matMap) = 0;
    };
    
    
    class MaterialFileReader: public MaterialReader
    {
    public:
        MaterialFileReader(const std::string& mtl_basepath): m_mtlBasePath(mtl_basepath) {}
        virtual ~MaterialFileReader() {}
        virtual std::string operator() (
                const std::string& matId,
                std::vector<material_r>& materials,
                std::map<std::string,
                int>& matMap);
    private:
        std::string m_mtlBasePath;
    };
    
    std::string LoadObj(
        std::vector<shape_r>& shapes,
        std::vector<material_r>& materials,
        const char* filename,
        const char* mtl_basepath = NULL);
    
    std::string LoadObj(std::vector<shape_r>& shapes,
                        std::vector<material_r>& materials,
                        std::istream& inStream,
                        MaterialReader& readMatFn);
}

#endif /* defined(__obj_loader__RGObjLoader__) */
