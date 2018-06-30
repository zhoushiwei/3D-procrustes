//
//  PointTransfer.hpp
//  ScaleShape
//
//  Created by shiwei zhou on 2018/6/29.
//  Copyright © 2018年 shiwei zhou. All rights reserved.
//

#ifndef PointTransfer_hpp
#define PointTransfer_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
Mat procrustes(double *meanfacelm3, double *facelm3, int keycount, double *data, int nPtCount, cv::Mat &tranferMat, int dim = 3);

#endif /* PointTransfer_hpp */
