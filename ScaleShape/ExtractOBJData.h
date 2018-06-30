//
//  ExtractOBJData.h
//  Avatar
//
//  Created by zhoushiwei on 14-5-4.
//  Copyright (c) 2014年 zhoushiwei. All rights reserved.
//

#ifndef Avatar_ExtractOBJData_h
#define Avatar_ExtractOBJData_h

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
using namespace std;


class material
{
public:
    float Ka[4];        //ambient coefficient
    float Kd[4];        //diffuse coefficient
    float Ks[4];        //specular coefficient
    float Ns;            // shiness
    
    float Tr;            //Transpant (or d)
    std::string map_Ka;    //ambient texture
    std::string map_Kd;    //diffuse texture
    std::string map_Ks;    //specular texture
    
    //This mtl loader is still incomplete
    //Please see http://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library
    
    std::string                    matFile;
    
    size_t                        mTotal;    // total material
    std::map<std::string,size_t>matMap;        // matMap[material_name] = material_ID
    std::vector<material>        mList;        // material ID (®C≠” mesh ≥Ã¶h¶≥ 100 ∫ÿ material)
    
    size_t    vTotal, tTotal, nTotal, fTotal; // number of total vertice, faces, texture coord., normal vecters, and faces
    
    material()
    {
        for (size_t i=0;i<4;i++)
            Ka[i] = Kd[i] = Ks[i] = 1.0f;
        Ns = 0.0f;
        Tr = 0.0f;
    }
    
    void LoadMtl(string tex_file)
    {
        std::string    s_file;
        char    token[100], buf[100];
        float    r,g,b;
        FILE        *fp_mtl;
        
        fp_mtl = fopen(tex_file.c_str(),"r");
        
        if (!fp_mtl)
        {
            cout << "Can't open material file \"" << tex_file << "\"!" << endl;
            return;
        }
        
        cout<<tex_file<<endl;
        
        size_t cur_mat;
        
        while(!feof(fp_mtl))
        {
            token[0] = NULL;
            fscanf(fp_mtl,"%s", token);        // ≈™ token
            
            if (!strcmp(token,"newmtl"))
            {
                fscanf(fp_mtl,"%s",buf);
                material newMtl;
                mList.push_back(newMtl);
                cur_mat = mTotal++;                    // ±q mList[1] ∂}©l°AmList[0] ™≈§U®”µπ default material •Œ
                matMap[s_file+string("_")+string(buf)] = cur_mat;     // matMap["material_name"] = material_id;
            }
            
            else if (!strcmp(token,"Ka"))
            {
                fscanf(fp_mtl,"%f %f %f",&r,&g,&b);
                mList[cur_mat].Ka[0] = r;
                mList[cur_mat].Ka[1] = g;
                mList[cur_mat].Ka[2] = b;
                mList[cur_mat].Ka[3] = 1;
            }
            
            else if (!strcmp(token,"Kd"))
            {
                fscanf(fp_mtl,"%f %f %f",&r,&g,&b);
                mList[cur_mat].Kd[0] = r;
                mList[cur_mat].Kd[1] = g;
                mList[cur_mat].Kd[2] = b;
                mList[cur_mat].Kd[3] = 1;
            }
            
            else if (!strcmp(token,"Ks"))
            {
                fscanf(fp_mtl,"%f %f %f",&r,&g,&b);
                mList[cur_mat].Ks[0] = r;
                mList[cur_mat].Ks[1] = g;
                mList[cur_mat].Ks[2] = b;
                mList[cur_mat].Ks[3] = 1;
            }
            
            else if (!strcmp(token,"Ns"))
            {
                fscanf(fp_mtl,"%f",&r);
                mList[cur_mat].Ns = r;
            }
            
            else if (!strcmp(token,"Tr"))
            {
                fscanf(fp_mtl,"%f",&r);
                mList[cur_mat].Tr = r;
            }
            
            else if (!strcmp(token,"d"))
            {
                fscanf(fp_mtl,"%f",&r);
                mList[cur_mat].Tr = r;
            }
            
            if (!strcmp(token,"map_Kd"))
            {
                fscanf(fp_mtl,"%s",buf);
                mList[cur_mat].map_Kd = buf;
            }
            
            if (!strcmp(token,"map_Ks"))
            {
                fscanf(fp_mtl,"%s",buf);
                mList[cur_mat].map_Ks = buf;
            }
            
            if (!strcmp(token,"map_Ka"))
            {
                fscanf(fp_mtl,"%s",buf);
                mList[cur_mat].map_Ka = buf;
            }
            
            else if (!strcmp(token,"#"))      // µ˘∏—
                fgets(buf,100,fp_mtl);
            
            //        printf("[%s]\n",token);
        }
        
        printf("total material:%d\n",matMap.size());
        
        if (fp_mtl) fclose(fp_mtl);
    }
};

class ExtractOBJData{

    // Model Structure
    typedef struct Model
    {
        int vertices;
        int positions;
        int texels;
        int normals;
        int faces;
        int materials;
    }
    Model;
  
    int getMTLinfo(string fp)
    {
        int m = 0;
        
        
        ifstream inMTL;
        inMTL.open(fp);
        if(!inMTL.good())
        {
            cout << "ERROR OPENING MTL FILE" << endl;
            exit(1);
        }
        
        while(!inMTL.eof())
        {
            
            string line;
            getline(inMTL, line);
            string type = line.substr(0,2);
            
            if(type.compare("ne") == 0)
                m++;
        }
        
        inMTL.close();
        
        return m;
    }
    
    Model getOBJinfo(string fp)
    {
        Model model = {0};
        
        // Open OBJ file
        ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            cout << "ERROR OPENING OBJ FILE" << endl;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            string line;
            getline(inOBJ, line);
            //  printf("%s\n",line.c_str());
            string type = line.substr(0,2);
            
            if(type.compare("v ") == 0)
                model.positions++;
            else if(type.compare("vt") == 0)
                model.texels++;
            else if(type.compare("vn") == 0)
                model.normals++;
            else if(type.compare("f ") == 0)
                model.faces++;
        }
        
        model.vertices = model.faces*3;
        
        // Close OBJ file
        inOBJ.close();
        
        return model;
    }
private:
    Model model;
    
    int OBJVertices;
    float OBJPositions;
    float OBJTexels;
    float OBJNormals;
    
    
public:
    
    void extractMTLdata(string fp, string* materials, float diffuses[][3], float speculars[][3])
    {
        
        int m = 0;
        int d = 0;
        int s = 0;
        
        
        ifstream inMTL;
        inMTL.open(fp);
        if(!inMTL.good())
        {
            cout << "ERROR OPENING MTL FILE" << endl;
            exit(1);
        }
        
        while(!inMTL.eof())
        {
            string line;
            getline(inMTL, line);
            string type = line.substr(0,2);
            
            if(type.compare("ne") == 0)
            {
                
                string l = "newmtl ";
                materials[m] = line.substr(l.size());
                m++;
            }
            
            else if(type.compare("Kd") == 0)
            {
                
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    diffuses[d][i] = atof(strtok(NULL, " "));
                
                delete[] l;
                d++;
            }
            
            else if(type.compare("Ks") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++){
                    speculars[s][i] = atof(strtok(NULL, " "));
                    cout<<"speculars[s][i]  "<<speculars[s][i]<<endl;
                }
                
                delete[] l;
                s++;
            }
        }
        
        inMTL.close();
    }
    
    //初始化
    void ExtractOBJDataInit( string &objpath,int &model_position,int &model_texels,int& model_normals,int &model_faces)
    {
        model=getOBJinfo(objpath);
        model_position=model.positions;
        model_texels=model.texels;
        model_normals=model.normals;
        model_faces=model.faces;
    }
    //解析文件
    void extractOBJdata(string fp,  std::vector<cv::Point3f> &positionscv,
                        std::vector<cv::Point2f> &texelscv,
                        std::vector<cv::Point3f> &normalscv,
                        std::vector<cv::Point3i> &faces_vertex,
                        std::vector<cv::Point3i> &faces_texel,
                        std::vector<cv::Point3i> &faces_normal)
    {
        
        
        float positions[model.positions][3];    // XYZ
        float texels[model.texels][2];          // UV
        float normals[model.normals][3];        // XYZ
        int faces[model.faces][9];              // PTN PTN PTN
        
        // Counters
        int p = 0;
        int t = 0;
        int n = 0;
        int f = 0;
        
        // Open OBJ file
        ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            cout << "ERROR OPENING OBJ FILE" << endl;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            string line;
            getline(inOBJ, line);
            string type = line.substr(0,2);
            
            // Positions
            if(type.compare("v ") == 0)
            {
                // Copy line for parsing
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                // Extract tokens
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    positions[p][i] = atof(strtok(NULL, " "));
                positionscv.push_back(cv::Point3d(positions[p][0],positions[p][1],positions[p][2]));
                //   printf("%f %f %f\n",positions[p][0],positions[p][1],positions[p][2]);
                // Wrap up
                delete[] l;
                p++;
            }
            
            // Texels
            else if(type.compare("vt") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<2; i++)
                    texels[t][i] = atof(strtok(NULL, " "));
                texelscv.push_back(cv::Point2d(texels[t][0],texels[t][1]));
                delete[] l;
                t++;
            }
            
            // Normals
            else if(type.compare("vn") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    normals[n][i] = atof(strtok(NULL, " "));
                normalscv.push_back(cv::Point3d(normals[n][0],normals[n][1],normals[n][2]));
                delete[] l;
                n++;
            }
            
            // Faces
            else if(type.compare("f ") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<6; i++)
                    faces[f][i] = atof(strtok(NULL, " /"));
                faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][2]-1,faces[f][4]-1));
                faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][3]-1,faces[f][6]-1));
                //  faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
                
                delete[] l;
                f++;
            }
        }
        //  printf("%d",p);
        // Close OBJ file
        inOBJ.close();
        
    }
    
    void extractOBJdata2(string fp,  std::vector<cv::Point3d> &positionscv,
                         std::vector<cv::Point2d> &texelscv,
                         std::vector<cv::Point3d> &normalscv,
                         std::vector<cv::Point3i> &faces_vertex,
                         std::vector<cv::Point3i> &faces_texel,
                         std::vector<cv::Point3i> &faces_normal)
    {
        
        
        float positions[model.positions][3];    // XYZ
        float texels[model.texels][2];          // UV
        float normals[model.normals][3];        // XYZ
        int faces[model.faces][9];              // PTN PTN PTN
        
        // Counters
        int p = 0;
        int t = 0;
        int n = 0;
        int f = 0;
        
        // Open OBJ file
        ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            cout << "ERROR OPENING OBJ FILE" << endl;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            string line;
            getline(inOBJ, line);
            string type = line.substr(0,2);
            
            // Positions
            if(type.compare("v ") == 0)
            {
                // Copy line for parsing
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                // Extract tokens
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    positions[p][i] = atof(strtok(NULL, " "));
                positionscv.push_back(cv::Point3d(positions[p][0],positions[p][1],positions[p][2]));
                //   printf("%f %f %f\n",positions[p][0],positions[p][1],positions[p][2]);
                // Wrap up
                delete[] l;
                p++;
            }
            
            // Texels
            else if(type.compare("vt") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<2; i++)
                    texels[t][i] = atof(strtok(NULL, " "));
                texelscv.push_back(cv::Point2d(texels[t][0],texels[t][1]));
                delete[] l;
                t++;
            }
            
            // Normals
            else if(type.compare("vn") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    normals[n][i] = atof(strtok(NULL, " "));
                normalscv.push_back(cv::Point3d(normals[n][0],normals[n][1],normals[n][2]));
                delete[] l;
                n++;
            }
            
            // Faces
            //            else if(type.compare("f ") == 0)
            //            {
            //                char* l = new char[line.size()+1];
            //                memcpy(l, line.c_str(), line.size()+1);
            //
            //                strtok(l, " ");
            ////                for(int i=0; i<9; i++)
            ////                    faces[f][i] = atof(strtok(NULL, " //"));
            ////                faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][3]-1,faces[f][6]-1));
            ////                faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][4]-1,faces[f][7]-1));
            ////                faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
            //
            //                                    for(int i=0; i<6; i++)
            //                                       faces[f][i] = atof(strtok(NULL, " //"));
            //                                 faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][2]-1,faces[f][4]-1));
            //                                  faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][3]-1,faces[f][5]-1));
            //                                  //  faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
            //
            //                delete[] l;
            //                f++;
            //            }
        }
        //  printf("%d",p);
        // Close OBJ file
        inOBJ.close();
        
    }
    
    
    //解析文件
    void extractOBJdata3(string fp,  std::vector<cv::Point3f> &positionscv,
                         std::vector<cv::Point2f> &texelscv,
                         std::vector<cv::Point3f> &normalscv,
                         std::vector<cv::Point3i> &faces_vertex,
                         std::vector<cv::Point3i> &faces_texel,
                         std::vector<cv::Point3i> &faces_normal)
    {
        
        
        float positions[model.positions][3];    // XYZ
        double texels[model.texels][2];          // UV
        float normals[model.normals][3];        // XYZ
        int faces[model.faces][9];              // PTN PTN PTN
        
        // Counters
        int p = 0;
        int t = 0;
        int n = 0;
        int f = 0;
        
        // Open OBJ file
        ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            cout << "ERROR OPENING OBJ FILE" << endl;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            string line;
            getline(inOBJ, line);
            string type = line.substr(0,2);
            
            // Positions
            if(type.compare("v ") == 0)
            {
                // Copy line for parsing
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                // Extract tokens
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    positions[p][i] = atof(strtok(NULL, " "));
                positionscv.push_back(cv::Point3f(positions[p][0],positions[p][1],positions[p][2]));
                // printf("%f %f %f\n",positions[p][0],positions[p][1],positions[p][2]);
                // Wrap up
                delete[] l;
                p++;
            }
            
            // Texels
            else if(type.compare("vt") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
              
                strtok(l, " ");
                for(int i=0; i<2; i++){
                    texels[t][i] = atof(strtok(NULL, " "));

                }
             
                texelscv.push_back(cv::Point2f(texels[t][0],texels[t][1]));
                
                delete[] l;
                t++;
            }
            
            // Normals
            else if(type.compare("vn") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    normals[n][i] = atof(strtok(NULL, " "));
                normalscv.push_back(cv::Point3f(normals[n][0],normals[n][1],normals[n][2]));
                delete[] l;
                n++;
            }
            
            // Faces
            else if(type.compare("f ") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<9; i++)
                    faces[f][i] = atof(strtok(NULL, " /"));
                faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][3]-1,faces[f][6]-1));
                faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][4]-1,faces[f][7]-1));
                faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
                
                //                    for(int i=0; i<6; i++)
                //                        faces[f][i] = atof(strtok(NULL, " //"));
                //                    faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][2]-1,faces[f][4]-1));
                //                    faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][3]-1,faces[f][5]-1));
                //                    //  faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
                
                delete[] l;
                f++;
            }
        }
        //  printf("%d",p);
        // Close OBJ file
        inOBJ.close();
        
    }
    void extractOBJdata4(string fp,  std::vector<cv::Point3f> &positionscv,
                         std::vector<cv::Point2f> &texelscv,
                         std::vector<cv::Point3f> &normalscv,
                         std::vector<cv::Point3i> &faces_vertex,
                         std::vector<cv::Point3i> &faces_texel,
                         std::vector<cv::Point3i> &faces_normal)
    {
        
        
        float positions[model.positions][3];    // XYZ
        float texels[model.texels][2];          // UV
        float normals[model.normals][3];        // XYZ
        int faces[model.faces][9];              // PTN PTN PTN
        
        // Counters
        int p = 0;
        int t = 0;
        int n = 0;
        int f = 0;
        
        // Open OBJ file
        ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            cout << "ERROR OPENING OBJ FILE" << endl;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            string line;
            getline(inOBJ, line);
            string type = line.substr(0,2);
            
            // Positions
            if(type.compare("v ") == 0)
            {
                // Copy line for parsing
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                // Extract tokens
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    positions[p][i] = atof(strtok(NULL, " "));
                positionscv.push_back(cv::Point3d(positions[p][0],positions[p][1],positions[p][2]));
                // printf("%f %f %f\n",positions[p][0],positions[p][1],positions[p][2]);
                // Wrap up
                delete[] l;
                p++;
            }
            
            // Texels
            else if(type.compare("vt") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<2; i++)
                    texels[t][i] = atof(strtok(NULL, " "));
                texelscv.push_back(cv::Point2d(texels[t][0],texels[t][1]));
                delete[] l;
                t++;
            }
            
            // Normals
            else if(type.compare("vn") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    normals[n][i] = atof(strtok(NULL, " "));
                normalscv.push_back(cv::Point3d(normals[n][0],normals[n][1],normals[n][2]));
                delete[] l;
                n++;
            }
            
            // Faces
            else if(type.compare("f ") == 0)
            {
                //                char* l = new char[line.size()+1];
                //                memcpy(l, line.c_str(), line.size()+1);
                //     printf("%s\n",line.c_str());
                replace(line.begin(), line.end(), '/', ' ');
                //     printf("%s\n",line.c_str());
                istringstream ls(line);
                int vi1, vti1, vi2, vti2, vi3, vti3;
                string ss;
                ls>>ss;
                ls >> vi1 >> vti1 >> vi2 >> vti2 >> vi3 >> vti3;
                //    printf("vti1 %d  vti2 %d vit3 %d\n",vi1 ,  vi2 ,vi3);
                faces_texel.push_back(cv::Point3i(vti1-1, vti2-1,vti3-1));
                faces_vertex.push_back(cv::Point3i( vi1-1,vi2-1,vi3-1));
                
                if(ls.peek()==' '){
                    int vi4, vti4;
                    ls >> vi4 >> vti4;
                    faces_vertex.push_back(cv::Point3i(vi1-1, vi3-1,vi4-1));
                    faces_texel.push_back(cv::Point3i( vti1-1,vti3-1,vti4-1));
                }
                
                
                //                strtok(l, " ");
                //                for(int i=0; i<9; i++)
                //                    faces[f][i] = atof(strtok(NULL, " /"));
                //                faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][3]-1,faces[f][6]-1));
                //                faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][4]-1,faces[f][7]-1));
                //                faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
                
                //                    for(int i=0; i<6; i++)
                //                        faces[f][i] = atof(strtok(NULL, " //"));
                //                    faces_vertex.push_back(cv::Point3i( faces[f][0]-1, faces[f][2]-1,faces[f][4]-1));
                //                    faces_texel.push_back(cv::Point3i( faces[f][1]-1, faces[f][3]-1,faces[f][5]-1));
                //                    //  faces_normal.push_back(cv::Point3i( faces[f][2]-1, faces[f][5]-1,faces[f][8]-1));
                
                //   delete[] l;
                f++;
            }
        }
        //  printf("%d",p);
        // Close OBJ file
        inOBJ.close();
        
    }
    
    template <class ContainerType>
    void tokenize(const std::string& str, ContainerType& tokens, const std::string& delimiters = " ",
                  bool trim_empty = false)
    {
        std::string::size_type pos, last_pos = 0;
        const auto length = str.length();
        
        using value_type = typename ContainerType::value_type;
        using size_type = typename ContainerType::size_type;
        
        while (last_pos < length + 1)
        {
            pos = str.find_first_of(delimiters, last_pos);
            if (pos == std::string::npos)
            {
                pos = length;
            }
            
            if (pos != last_pos || !trim_empty)
                tokens.push_back(value_type(str.data() + last_pos, (size_type)pos - last_pos));
            
            last_pos = pos + 1;
        }
    }
    
    // Note: Indices in obj's start at 1!
    // Oh no, these contain 3 entries for triangles, but can be quads (=4 entries), and potentially more.
    // Their triplets of: vertex, texture and normal indices. Some of them can be missing. E.g.:
    //  f 1 2 3
    //  f 3/1 4/2 5/3
    //  f 6/4/1 3/5/3 7/6/5
    //  f 7//1 8//2 9//3
    inline auto parse_face(const std::string& line)
    {
        using std::string;
        using std::vector;
        
        // Obj indices are 1-based.
        vector<int> vertex_indices;  // size() = 3 or 4
        vector<int> texture_indices; // size() = 3 or 4
        vector<int> normal_indices;  // size() = 3 or 4
        
        vector<string> tokens;
        vector<string> tokens_temp;
        tokenize(line, tokens, " ");
        //  assert(tokens.size() == 3 || tokens.size() == 4); // For now we need this to be 3 (triangles) or 4 (quads).
        //        if(tokens_temp.size()>4){
        //            for (int i=0; i<4; i++) {
        //                tokens.push_back(tokens_temp[i]);
        //            }
        //        }
        // Now for each of these tokens, we want to split on "/":
        for (const auto& token : tokens)
        {
            vector<string> subtokens;
            tokenize(token, subtokens, "/"); // do we want trim_empty true or false?
            //   assert(subtokens.size() > 0 && subtokens.size() <= 3); // <= 3 correct or not?
            // Ok, let's make our life easy, for now only support the 1/2/3 syntax of the FaceWarehouse scans. In
            // fact the normal_indices are 0... we should check for that - zero-index = ignore, but note that, it's
            // probably a non-standard obj format extension.
            //  assert(subtokens.size() == 3);                         // FaceWarehouse
            vertex_indices.push_back(std::stoi(subtokens[0]) - 1); // obj indices are 1-based, so we subtract one.
            texture_indices.push_back(std::stoi(subtokens[1]) - 1);
            normal_indices.push_back(std::stoi(subtokens[2]) - 1);
            // subtokens[2] is zero, hence, no normal_indices.
        }
        
        return std::make_tuple(vertex_indices, texture_indices, normal_indices);
    }
    
    void extractOBJdata5(string fp,  std::vector<cv::Point3f> &positionscv,
                         std::vector<cv::Point2f> &texelscv,
                         std::vector<cv::Point3f> &normalscv,
                         std::vector<cv::Point3i> &faces_vertex,
                         std::vector<cv::Point3i> &faces_texel,
                         std::vector<cv::Point3i> &faces_normal)
    {
        
        
        float positions[model.positions][3];    // XYZ
        float texels[model.texels][2];          // UV
        float normals[model.normals][3];        // XYZ
        int faces[model.faces][9];              // PTN PTN PTN
        
        // Counters
        int p = 0;
        int t = 0;
        int n = 0;
        int f = 0;
        
        // Open OBJ file
        ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            cout << "ERROR OPENING OBJ FILE" << endl;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            string line;
            getline(inOBJ, line);
            string type = line.substr(0,2);
            
            // Positions
            if(type.compare("v ") == 0)
            {
                // Copy line for parsing
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                // Extract tokens
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    positions[p][i] = atof(strtok(NULL, " "));
                positionscv.push_back(cv::Point3d(positions[p][0],positions[p][1],positions[p][2]));
                // printf("%f %f %f\n",positions[p][0],positions[p][1],positions[p][2]);
                // Wrap up
                delete[] l;
                p++;
            }
            
            // Texels
            else if(type.compare("vt") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<2; i++)
                    texels[t][i] = atof(strtok(NULL, " "));
                texelscv.push_back(cv::Point2d(texels[t][0],texels[t][1]));
                delete[] l;
                t++;
            }
            
            // Normals
            else if(type.compare("vn") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    normals[n][i] = atof(strtok(NULL, " "));
                normalscv.push_back(cv::Point3d(normals[n][0],normals[n][1],normals[n][2]));
                delete[] l;
                n++;
            }
            
            // Faces
            else if(type.compare("f ") == 0)
            {
                
                auto face_data =parse_face(line.substr(2));
                
                //  cout<<std::get<1>(face_data)[0]<<"  "<<std::get<0>(face_data)[1]<<" "<<std::get<0>(face_data)[2]<<endl;
                if (std::get<0>(face_data).size() == 3) // 3 triangle indices, nothing to do:
                {
                    faces_vertex.push_back(cv::Point3i(std::get<0>(face_data)[0], std::get<0>(face_data)[1], std::get<0>(face_data)[2]));
                    
                    faces_texel.push_back(cv::Point3i(std::get<1>(face_data)[0], std::get<1>(face_data)[1], std::get<1>(face_data)[2]));
                       faces_normal.push_back(cv::Point3i(std::get<2>(face_data)[0], std::get<2>(face_data)[1], std::get<2>(face_data)[2]));
                }
                // If their sizes are 4, we convert the quad to two triangles:
                // Actually I think MeshLab does the same, it shows the FaceWarehouse number of "Faces" as twice the "f" entries in the obj.
                else if (std::get<0>(face_data).size() == 4)
                {
                    // Just create two faces with (quad[0], quad[1], quad[2]) and (quad[0], quad[2], quad[3]).
                    faces_vertex.push_back(cv::Point3i(std::get<0>(face_data)[0], std::get<0>(face_data)[1], std::get<0>(face_data)[2]));
                    faces_texel.push_back(cv::Point3i(std::get<1>(face_data)[0], std::get<1>(face_data)[1], std::get<1>(face_data)[2]));
                      faces_normal.push_back(cv::Point3i(std::get<2>(face_data)[0], std::get<2>(face_data)[1], std::get<2>(face_data)[2]));
                    
                    faces_vertex.push_back(cv::Point3i(std::get<0>(face_data)[0], std::get<0>(face_data)[2], std::get<0>(face_data)[3]));
                    faces_texel.push_back(cv::Point3i(std::get<1>(face_data)[0], std::get<1>(face_data)[2], std::get<1>(face_data)[3]));
                        faces_normal.push_back(cv::Point3i(std::get<2>(face_data)[0], std::get<2>(face_data)[2], std::get<2>(face_data)[3]));
                }
                
                f++;
            }
        }
        //  printf("%d",p);
        // Close OBJ file
        inOBJ.close();
        
    }
    
    
    void writeCvertices(string fp, string name, Model model)
    {
        // Create C file
        ofstream outC;
        outC.open(fp);
        if(!outC.good())
        {
            cout << "ERROR CREATING C FILE" << endl;
            exit(1);
        }
        
        // Write to C file
        outC << "// This is a .c file for the model: " << name << endl;
        outC << endl;
        
        // Header
        outC << "#include " << "\"" << name << ".h" << "\"" << endl;
        outC << endl;
        
        // Vertices
        outC << "const int " << name << "Vertices = " << model.vertices << ";" << endl;
        outC << endl;
        
        // Close C file
        outC.close();
    }
    
    void writeCpositions(string fp, string name, Model model, int faces[][9], float positions[][3])
    {
        // Append C file
        ofstream outC;
        outC.open(fp, ios::app);
        printf("model face %d",model.faces);
        // Positions
        outC << "const float " << name << "Positions[" << model.vertices*3 << "] = " << endl;
        outC << "{" << endl;
        for(int i=0; i<model.faces; i++)
        {
            int vA = faces[i][0] - 1;
            int vB = faces[i][3] - 1;
            int vC = faces[i][6] - 1;
            printf("%d %d %d\n",vA,vB,vC);
            outC << positions[vA][0] << ", " << positions[vA][1] << ", " << positions[vA][2] << ", " << endl;
            outC << positions[vB][0] << ", " << positions[vB][1] << ", " << positions[vB][2] << ", " << endl;
            outC << positions[vC][0] << ", " << positions[vC][1] << ", " << positions[vC][2] << ", " << endl;
        }
        outC << "};" << endl;
        outC << endl;
        
        // Close C file
        outC.close();
    }
    
    void writeCtexels(string fp, string name, Model model, int faces[][9], float texels[][2])
    {
        // Append C file
        ofstream outC;
        outC.open(fp, ios::app);
        
        // Texels
        outC << "const float " << name << "Texels[" << model.vertices*2 << "] = " << endl;
        outC << "{" << endl;
        for(int i=0; i<model.faces; i++)
        {
            int vtA = faces[i][1] - 1;
            int vtB = faces[i][4] - 1;
            int vtC = faces[i][7] - 1;
            
            outC << texels[vtA][0] << ", " << texels[vtA][1] << ", " << endl;
            outC << texels[vtB][0] << ", " << texels[vtB][1] << ", " << endl;
            outC << texels[vtC][0] << ", " << texels[vtC][1] << ", " << endl;
            printf("%f %f,%f %f,%f %f\n", texels[vtA][0],texels[vtA][1], texels[vtB][0] ,texels[vtB][1]
                   ,texels[vtC][0] ,texels[vtC][1]);
            
        }
        outC << "};" << endl;
        outC << endl;
        
        // Close C file
        outC.close();
    }
    
    void writeCnormals(string fp, string name, Model model, int faces[][9], float normals[][3])
    {
        // Append C file
        ofstream outC;
        outC.open(fp, ios::app);
        
        // Normals
        outC << "const float " << name << "Normals[" << model.vertices*3 << "] = " << endl;
        outC << "{" << endl;
        for(int i=0; i<model.faces; i++)
        {
            int vnA = faces[i][2] - 1;
            int vnB = faces[i][5] - 1;
            int vnC = faces[i][8] - 1;
            
            outC << normals[vnA][0] << ", " << normals[vnA][1] << ", " << normals[vnA][2] << ", " << endl;
            outC << normals[vnB][0] << ", " << normals[vnB][1] << ", " << normals[vnB][2] << ", " << endl;
            outC << normals[vnC][0] << ", " << normals[vnC][1] << ", " << normals[vnC][2] << ", " << endl;
        }
        outC << "};" << endl;
        outC << endl;
        
        // Close C file
        outC.close();
    }
    
    //   void outputData();
};

#endif

