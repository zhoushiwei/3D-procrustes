//
//  PointTransfer.cpp
//  ScaleShape
//
//  Created by shiwei zhou on 2018/6/29.
//  Copyright © 2018年 shiwei zhou. All rights reserved.
//

#include "PointTransfer.hpp"

Mat procrustes(double *meanfacelm3, double *facelm3, int keycount, double *data, int nPtCount, cv::Mat &tranferMat,int dim)
{
    Mat meanface(keycount, 1, CV_64FC3, meanfacelm3);
    meanface = meanface.t();
    Mat face(keycount, 1, CV_64FC3, facelm3);
    face = face.t();
    
    Scalar mu_x = cv::mean(meanface);
    Mat tempmeanface = meanface - Mat(meanface.size(), meanface.type(), mu_x);
    double normX = cv::norm(tempmeanface);
    tempmeanface = tempmeanface / normX;
    tempmeanface = tempmeanface.reshape(1, keycount);
    
    Scalar mu_y = cv::mean(face);
    Mat tempface = face - Mat(face.size(), face.type(), mu_y);
    double normY = cv::norm(tempface);
    tempface = tempface / normY;
    tempface = tempface.reshape(1, keycount);
    
    Mat A = tempmeanface.t()*tempface;
    
    Mat w, u, vt;
    SVDecomp(A, w, u, vt);
    
    Mat T = vt.t()*u.t();
    
    double traceTA = cv::sum(w)[0];
    double b = traceTA * normX / normY;
    double d = 1 - traceTA *traceTA;
    
    Mat muY = Mat(Size(3, 1), CV_64FC3, mu_y);
    muY = muY.reshape(1, 3);
    Mat muX = Mat(Size(3, 1), CV_64FC3, mu_x);
    muX = muX.reshape(1, 3);
    Mat c = muX - b*muY*T;
    
    Mat trans = -1 / b *c*T.t();
    
    Mat transM = Mat::zeros(4, 4, CV_64FC1);
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            transM.at<double>(i, j) = 1.0 / b * T.at<double>(i, j);
        }
    }
    
    transM.at<double>(0, 3) = trans.at<double>(0, 0);
    transM.at<double>(1, 3) = trans.at<double>(0, 1);
    transM.at<double>(2, 3) = trans.at<double>(0, 2);
    transM.at<double>(3, 3) = 1;
    
    Mat transMinv;
    cv::invert(transM, transMinv);
    
    Mat tempss = Mat(nPtCount, 3, CV_64FC1, data);
    
    Mat ss = Mat(4, nPtCount, CV_64FC1);
    
    //tempss.copyTo(ss(Rect(0, 0, 3, nPtCount)));
    for (int i = 0; i < nPtCount; i++)
    {
        ss.at<double>(0, i) = tempss.at<double>(i, 0);
        ss.at<double>(1, i) = tempss.at<double>(i, 1);
        ss.at<double>(2, i) = tempss.at<double>(i, 2);
        ss.at<double>(3, i) = 1;
    }
    tranferMat=transMinv.clone();
    //Mat res = ss*transMinv.t();
    //res=res.t();
    Mat res = transMinv*ss;
    Mat re = res(Rect(0, 0, nPtCount, 3));
    
    return re;
    
}
