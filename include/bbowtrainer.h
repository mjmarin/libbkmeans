//--------------------------------------
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#ifndef _BBOWTRAINER_H_
#define _BBOWTRAINER_H_ 1

#include <iostream>

#include <opencv2/core/core.hpp> 
#include <opencv2/features2d/features2d.hpp>

#include <bkmeans.h>

using namespace cv;

/**
   \class BBOWTrainer
   This class implements a BOW trainer for binary data
*/

class BKMEANS_EXPORT BBOWTrainer: public BOWTrainer
{
   public:
   BBOWTrainer(int clusterCount);

   Mat cluster() const;

   /** Do clustering
       \note descriptors are not stored in the object
   */
   Mat cluster( const cv::Mat& descriptors ) const;

   void add( const Mat& descriptors ){std::cerr << "Disabled method" << std::endl;};

   /** Always returns 0
   */
   int descriptorsCount(void) const {return 0;};

   private:
   int _K;
   
};

#endif