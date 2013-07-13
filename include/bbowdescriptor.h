//--------------------------------------
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#ifndef _BBOWDESCRIPTOR_H_
#define _BBOWDESCRIPTOR_H_ 1

#include <iostream>

#include <opencv2/core/core.hpp> 
#include <opencv2/features2d/features2d.hpp>

#include <bkmeans.h>

using namespace cv;

/**
   \class BBOWImgDescriptorExtractor
   This class computes BOW descriptor for binary features
*/

class BKMEANS_EXPORT BBOWImgDescriptorExtractor
{
 public:
   BBOWImgDescriptorExtractor();

   void setVocabulary(const Mat & vocab);

   const Mat& getVocabulary(void) const;

   vector<cv::Mat> compute(Mat & imgDescriptors) const;

   int descriptorSize() const;

 private:
    int _status;
    vector<cv::Mat> _dictionary;
};

#endif
