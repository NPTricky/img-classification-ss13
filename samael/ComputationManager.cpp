#include "stdafx.h"
#include "ComputationManager.h"

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

#include <QDebug>
#include <limits>
#include <QVector4d>
#include <vector>

using namespace std;
using namespace cv;

ComputationManager::ComputationManager()
{

}

ComputationManager::~ComputationManager()
{
  
}
ComputationManager* ComputationManager::getInstance()
{
  static ComputationManager instance;
  return &instance;
}



int ComputationManager::SIFT(QString path)
{
   //in MySIFT
	return 0;
}
int ComputationManager::SURF(QString path)
{
/*
  Mat img_1; //image
  Mat img_2; //scene

  //if( !img_1.data || !img_2.data )
  //{ std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<cv::KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Draw keypoints
  Mat img_keypoints_1; Mat img_keypoints_2;

  drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

  //-- Show detected (drawn) keypoints
  imshow("Keypoints 1", img_keypoints_1 );
  imshow("Keypoints 2", img_keypoints_2 );

  waitKey(0);
  */
  return 0;
}
int ComputationManager::MSER(QString path)
{
	/*Mat box = imread("C:\box.png",1);
    MSER ms;
    vector<vector<Point>> regions;
    ms(box, regions, Mat());
    for (int i = 0; i < regions.size(); i++)
    {
        ellipse(box, fitEllipse(regions[i]), Scalar(255));
    }
    imshow("mser", box);
    waitKey(0);*/

  return 0;
}
  