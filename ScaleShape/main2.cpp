//
//  main.cpp
//  ScaleShape
//
//  Created by shiwei zhou on 2018/6/24.
//  Copyright © 2018年 shiwei zhou. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/viz/vizcore.hpp>
#include <opencv2/viz/viz3d.hpp>
#include "readObj.hpp"
#include "ExtractOBJData.h"
#include "PointTransfer.hpp"
using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    std::vector<cv::Point2f> texels; //UV坐标
    std::vector<cv::Point3f> normals;
    std::vector<cv::Point3i> faces_vertex;  //每一维表示三个顶点的组成
    std::vector<cv::Point3f> positions;
    string filepathOBJ="/Users/shiweizhou/Desktop/MyWorkSpace/Face AR/data/current_mean_normal.obj";
    int modelposition,modeltexels,modelnormals,modelfaces;
    ExtractOBJData EOBJ;
    //  std::vector<cv::Point3i> faces_vertex;  //每一维表示三个顶点的组成
    std::vector<cv::Point3i> faces_texel;
    std::vector<cv::Point3i> faces_normal;
    EOBJ.ExtractOBJDataInit(filepathOBJ, modelposition,modeltexels,modelnormals,modelfaces);
    EOBJ.extractOBJdata3(filepathOBJ, positions, texels, normals, faces_vertex,faces_texel,faces_normal);
    
    
    ifstream ifs("/Users/shiweizhou/Desktop/Awesome-Project/DeformationTransfer/Face.cons2");
    std::vector<int> src_index,dst_index;
    int num;
    ifs>>num;
    std::vector<int> AlIndex;
    for (int i=0; i<66*2; i++) {
        int index;
        ifs>>index;
        if(i%2==1)dst_index.push_back(index);
        else src_index.push_back(index);
    }
    int ptCount=src_index.size();
    
    
    
    double *landmark = new double[ptCount * 3];
    for (int n = 0, i = 0; i<ptCount; i++, n++)
    {
        landmark[i * 3] = positions[src_index[i]].x;
        landmark[i * 3 + 1] = positions[src_index[i]].y;
        landmark[i * 3 + 2] = positions[src_index[i]].z;
    }
    
   
    
    
    positions.clear();
    texels.clear();
    normals.clear();
    faces_vertex.clear();
    faces_texel.clear();
    faces_normal.clear();
    filepathOBJ="/Users/shiweizhou/DataBase/FaceWareHouseData/mean.obj";
    EOBJ.ExtractOBJDataInit(filepathOBJ, modelposition,modeltexels,modelnormals,modelfaces);
    EOBJ.extractOBJdata3(filepathOBJ, positions, texels, normals, faces_vertex,faces_texel,faces_normal);
  
//    int PointsCount = positions.size();
//    int dim=3;
//    double *ptdata = new double[PointsCount*dim];
//
//    for (int i = 0; i < PointsCount; i++)
//    {
//        ptdata[i*3+0] =positions[i].x;
//        ptdata[i*3+1] =positions[i].y;
//        ptdata[i*3+2] =positions[i].z;
//    }
//
//    double *meanfacelm3 = new double[ptCount * 3];
//    for (int i = 0; i<ptCount; i++)
//    {
//
//        meanfacelm3[i * 3] = positions[dst_index[i]].x;
//        meanfacelm3[i * 3 + 1] = positions[dst_index[i]].y;
//        meanfacelm3[i * 3 + 2] = positions[dst_index[i]].z;
//    }
//
//    Mat newshape = procrustes(landmark,meanfacelm3,  ptCount, ptdata, PointsCount, 3);
   
    
//    string save_path="/Users/shiweizhou/Desktop/Awesome-Project/DeformationTransfer/ScaleShape/ScaleShape/0_scale.obj";
//
//    ofstream  ofsss(save_path);
//    for (int i=0; i<positions.size(); i++) {
//
//        ofsss<<"v "<<newshape.at<double>(0,i)<<" "<<newshape.at<double>(1,i)<<" "<<newshape.at<double>(2,i)<<endl;
//    }
//
//    for (int i=0; i<texels.size(); i++) {
//        cv::Point2f pt=texels[i];
//        ofsss<<"vt "<<pt.x<<" "<<pt.y<<endl;
//    }
//    for (int i=0; i<normals.size(); i++) {
//        ofsss<<"vn "<<normals[i].x<<" "<<normals[i].y<<" "<<normals[i].z<<endl;
//    }
//
//    for (int i=0; i<faces_vertex.size(); i++) {
//        cv::Point3i pt=faces_vertex[i];
//        cv::Point3i pt_t=faces_texel[i];
//        cv::Point3i pt_n=faces_normal[i];
//        ofsss<<"f "<<pt.x+1<<"/"<<pt_t.x+1<<"/"<<pt_n.x+1<<" "<<pt.y+1<<"/"<<pt_t.y+1<<"/"<<pt_n.y+1<<" "<<pt.z+1<<"/"<<pt_t.z+1<<"/"<<pt_n.z+1<<endl;
//    }
//    ofsss.close();
    
    
    
    string Mainpath="/Users/shiweizhou/DataBase/FaceWareHouseData/Tester_";
    for (int k=1; k<=150; k++) {
        cout<<"number "<<k<<endl;
        cv::Mat tranferMat;
        Mat newshape ;
        stringstream ss;
        ss<<k;
        string Tester_path=Mainpath+ss.str()+"/Blendshape/";
        for (int j=0; j<47; j++) {
            stringstream sss;
            sss<<j;
            string blendshape_path=Tester_path+sss.str()+".obj";
     
            positions.clear();
            texels.clear();
            normals.clear();
            faces_vertex.clear();
            faces_texel.clear();
            faces_normal.clear();
            EOBJ.ExtractOBJDataInit(blendshape_path, modelposition,modeltexels,modelnormals,modelfaces);
            EOBJ.extractOBJdata3(blendshape_path, positions, texels, normals, faces_vertex,faces_texel,faces_normal);
            std::vector<Vec3d> points_mesh;
         
            int PointsCount = (int)positions.size();
            int dim=3;
            
            if(j==0){
            double *ptdata = new double[PointsCount*dim];
            
            for (int i = 0; i < PointsCount; i++)
            {
                ptdata[i*3+0] =positions[i].x;
                ptdata[i*3+1] =positions[i].y;
                ptdata[i*3+2] =positions[i].z;
            }
            
            double *meanfacelm3 = new double[ptCount * 3];
            for (int i = 0; i<ptCount; i++)
            {
                
                meanfacelm3[i * 3] = positions[dst_index[i]].x;
                meanfacelm3[i * 3 + 1] = positions[dst_index[i]].y;
                meanfacelm3[i * 3 + 2] = positions[dst_index[i]].z;
            }
            
             newshape = procrustes(landmark,meanfacelm3,  ptCount, ptdata, PointsCount, tranferMat,3);
            delete []ptdata;
            }else {
                
                Mat ss = Mat(4, PointsCount, CV_64FC1);
                
                //tempss.copyTo(ss(Rect(0, 0, 3, nPtCount)));
                for (int i = 0; i < PointsCount; i++)
                {
                    ss.at<double>(0, i) = positions[i].x;
                    ss.at<double>(1, i) = positions[i].y;
                    ss.at<double>(2, i) = positions[i].z;
                    ss.at<double>(3, i) = 1;
                }
               
                Mat res = tranferMat*ss;
                newshape = res(Rect(0, 0, PointsCount, 3));
                
            }
            
            string save_path=Tester_path+sss.str()+"_scale.obj";
            ofstream  ofsss(save_path);
            for (int i=0; i<positions.size(); i++) {
//                 cv::Point3f pt=positions[i];
//                ofsss<<"v "<<pt.x<<" "<<pt.y<<" "<<pt.z<<endl;
                ofsss<<"v "<<newshape.at<double>(0,i)<<" "<<newshape.at<double>(1,i)<<" "<<newshape.at<double>(2,i)<<endl;
            }
            
            for (int i=0; i<texels.size(); i++) {
                cv::Point2f pt=texels[i];
                ofsss<<"vt "<<pt.x<<" "<<pt.y<<endl;
            }
            
            for (int i=0; i<normals.size(); i++) {
                ofsss<<"vn "<<normals[i].x<<" "<<normals[i].y<<" "<<normals[i].z<<endl;
            }
            
            for (int i=0; i<faces_vertex.size(); i++) {
                cv::Point3i pt=faces_vertex[i];
                cv::Point3i pt_t=faces_texel[i];
                cv::Point3i pt_n=faces_normal[i];
                ofsss<<"f "<<pt.x+1<<"/"<<pt_t.x+1<<"/"<<pt_n.x+1<<" "<<pt.y+1<<"/"<<pt_t.y+1<<"/"<<pt_n.y+1<<" "<<pt.z+1<<"/"<<pt_t.z+1<<"/"<<pt_n.z+1<<endl;
            }
            ofsss.close();
            
            
        }
    }
 
    
    return 0;
}
