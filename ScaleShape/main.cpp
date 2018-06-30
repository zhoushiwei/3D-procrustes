////
////  main.cpp
////  ScaleShape
////
////  Created by shiwei zhou on 2018/6/24.
////  Copyright © 2018年 shiwei zhou. All rights reserved.
////
//
//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <opencv2/viz/vizcore.hpp>
//#include <opencv2/viz/viz3d.hpp>
//#include "readObj.hpp"
//#include "ExtractOBJData.h"
//using namespace cv;
//using namespace std;
//
//int main(int argc, const char * argv[]) {
//    std::vector<cv::Point2f> texels; //UV坐标
//    std::vector<cv::Point3f> normals;
//    std::vector<cv::Point3i> faces_vertex;  //每一维表示三个顶点的组成
//    std::vector<cv::Point3f> positions;
//    string filepathOBJ="/Users/shiweizhou/Desktop/MyWorkSpace/Face AR/data/current_mean_normal.obj";
//    int modelposition,modeltexels,modelnormals,modelfaces;
//    ExtractOBJData EOBJ;
//    //  std::vector<cv::Point3i> faces_vertex;  //每一维表示三个顶点的组成
//    std::vector<cv::Point3i> faces_texel;
//    std::vector<cv::Point3i> faces_normal;
//    EOBJ.ExtractOBJDataInit(filepathOBJ, modelposition,modeltexels,modelnormals,modelfaces);
//    EOBJ.extractOBJdata3(filepathOBJ, positions, texels, normals, faces_vertex,faces_texel,faces_normal);
//    
//    
//    ifstream ifs("/Users/shiweizhou/Desktop/Awesome-Project/DeformationTransfer/Face.cons2");
//    std::vector<int> src_index,dst_index;
//    int num;
//    ifs>>num;
//    std::vector<int> AlIndex;
//    for (int i=0; i<66*2; i++) {
//        int index;
//        ifs>>index;
//        if(i%2==1)dst_index.push_back(index);
//        else src_index.push_back(index);
//    }
//    
//    cv::Point3f left_eye,right_eye,mouth_center;
//    for (int i=36; i<=41; i++) {
//        left_eye.x +=positions[src_index[i]].x;
//        left_eye.y +=positions[src_index[i]].y;
//        left_eye.z +=positions[src_index[i]].z;
//    }
//    left_eye.x/=6.0;
//    left_eye.y/=6.0;
//    left_eye.z/=6.0;
//    
//    for (int i=42; i<=47; i++) {
//        right_eye.x +=positions[src_index[i]].x;
//        right_eye.y +=positions[src_index[i]].y;
//        right_eye.z +=positions[src_index[i]].z;
//    }
//    right_eye.x/=6.0;
//    right_eye.y/=6.0;
//    right_eye.z/=6.0;
//    
//    for (int i=48; i<=65; i++) {
//        mouth_center.x +=positions[src_index[i]].x;
//        mouth_center.y +=positions[src_index[i]].y;
//        mouth_center.z +=positions[src_index[i]].z;
//    }
//    
//    mouth_center.x/=18.0;
//    mouth_center.y/=18.0;
//    mouth_center.z/=18.0;
//    
//    cv::Point3f center=positions[src_index[30]];
//    float w_src=sqrt((left_eye.x-right_eye.x)*(left_eye.x-right_eye.x)+(left_eye.y-right_eye.y)*(left_eye.y-right_eye.y)+(left_eye.z-right_eye.z)*(left_eye.z-right_eye.z));
//    float h_src=positions[src_index[33]].y-positions[src_index[27]].y;
//    
//    positions.clear();
//    texels.clear();
//    normals.clear();
//    faces_vertex.clear();
//    faces_texel.clear();
//    faces_normal.clear();
//    filepathOBJ="/Users/shiweizhou/DataBase/FaceWareHouseData/mean.obj";
//    EOBJ.ExtractOBJDataInit(filepathOBJ, modelposition,modeltexels,modelnormals,modelfaces);
//    EOBJ.extractOBJdata3(filepathOBJ, positions, texels, normals, faces_vertex,faces_texel,faces_normal);
//  
//    
//    left_eye=cv::Point3f(0,0,0),right_eye=cv::Point3f(0,0,0),mouth_center=cv::Point3f(0,0,0);
//    for (int i=36; i<=41; i++) {
//        left_eye.x +=positions[dst_index[i]].x;
//        left_eye.y +=positions[dst_index[i]].y;
//        left_eye.z +=positions[dst_index[i]].z;
//    }
//    left_eye.x/=6.0;
//    left_eye.y/=6.0;
//    left_eye.z/=6.0;
//    
//    for (int i=42; i<=47; i++) {
//        right_eye.x +=positions[dst_index[i]].x;
//        right_eye.y +=positions[dst_index[i]].y;
//        right_eye.z +=positions[dst_index[i]].z;
//    }
//    right_eye.x/=6.0;
//    right_eye.y/=6.0;
//    right_eye.z/=6.0;
//    
//    for (int i=48; i<=65; i++) {
//        mouth_center.x +=positions[dst_index[i]].x;
//        mouth_center.y +=positions[dst_index[i]].y;
//        mouth_center.z +=positions[dst_index[i]].z;
//    }
//    mouth_center.x/=18.0;
//    mouth_center.y/=18.0;
//    mouth_center.z/=18.0;
//    
//    cv::Point3f center_dst=positions[dst_index[33]];
//    float w_dst=sqrt((left_eye.x-right_eye.x)*(left_eye.x-right_eye.x)+(left_eye.y-right_eye.y)*(left_eye.y-right_eye.y)+(left_eye.z-right_eye.z)*(left_eye.z-right_eye.z));
//    float h_dst=positions[dst_index[33]].y-positions[dst_index[27]].y;
//    
//    float x_scale=w_dst/w_src;
//    float y_scale=h_dst/h_src;
//    cout<<x_scale<<" "<<y_scale<<endl;
//    x_scale=0.00925107;
//    y_scale=0.00721855;
////    float x_diff=-0.327242-0.002353;
////    float y_diff=-0.654177-0.169083;
////    float z_diff=3.359640-0.63249;
//    
//    string Mainpath="/Users/shiweizhou/DataBase/FaceWareHouseData/Tester_";
//    for (int i=1; i<=150; i++) {
//        stringstream ss;
//        ss<<i;
//        string Tester_path=Mainpath+ss.str()+"/Blendshape/";
//        for (int j=0; j<47; j++) {
//            stringstream sss;
//            sss<<j;
//            string blendshape_path=Tester_path+sss.str()+".obj";
//     
//          //ObjMesh obj=read_obj(blendshape_path);
//            positions.clear();
//            texels.clear();
//            normals.clear();
//            faces_vertex.clear();
//            faces_texel.clear();
//            faces_normal.clear();
//            EOBJ.ExtractOBJDataInit(blendshape_path, modelposition,modeltexels,modelnormals,modelfaces);
//            EOBJ.extractOBJdata3(blendshape_path, positions, texels, normals, faces_vertex,faces_texel,faces_normal);
//            std::vector<Vec3d> points_mesh;
//         
//            for (int l=0; l<positions.size(); l++) {
//                cv::Point3f pt=positions[l];
////                positions[l].x-=x_diff;
////                positions[l].y-=y_diff;
//             //   positions[l].z+=z_diff;
//                positions[l].x/=0.00925107;
//                positions[l].y/=0.00925107;
//                positions[l].z/=0.00925107;
//  
//            }
//            
//            center_dst=positions[dst_index[33]];
//            float x_diff=center_dst.x-center.x;
//            float y_diff=center_dst.y-center.y;
//            float z_diff=center_dst.z-center.z;
//            for (int l=0; l<positions.size(); l++) {
//                cv::Point3f pt=positions[l];
//                positions[l].x-=x_diff;
//                positions[l].y-=y_diff;
//                positions[l].z-=z_diff;
////                positions[l].x/=0.00925107;
////                positions[l].y/=0.00925107;
////                positions[l].z/=0.00925107;
//                
//                
//            }
//            string save_path=Tester_path+sss.str()+"_scale.obj";
//   
//            ofstream  ofsss(save_path);
//            for (int i=0; i<positions.size(); i++) {
//                 cv::Point3f pt=positions[i];
//                ofsss<<"v "<<pt.x<<" "<<pt.y<<" "<<pt.z<<endl;
//            }
//            
//            for (int i=0; i<texels.size(); i++) {
//                cv::Point2f pt=texels[i];
//                ofsss<<"vt "<<pt.x<<" "<<pt.y<<endl;
//            }
//            
//            for (int i=0; i<normals.size(); i++) {
//                ofsss<<"vn "<<normals[i].x<<" "<<normals[i].y<<" "<<normals[i].z<<endl;
//            }
//            
//            for (int i=0; i<faces_vertex.size(); i++) {
//                cv::Point3i pt=faces_vertex[i];
//                cv::Point3i pt_t=faces_texel[i];
//                cv::Point3i pt_n=faces_normal[i];
//                ofsss<<"f "<<pt.x+1<<"/"<<pt_t.x+1<<"/"<<pt_n.x+1<<" "<<pt.y+1<<"/"<<pt_t.y+1<<"/"<<pt_n.y+1<<" "<<pt.z+1<<"/"<<pt_t.z+1<<"/"<<pt_n.z+1<<endl;
//            }
//            ofsss.close();
//            
//            
//        }
//    }
// 
//    
//    return 0;
//}
