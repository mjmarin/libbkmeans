#include <bbowtrainer.h>

BBOWTrainer::BBOWTrainer(int clusterCount)
{
   _K = clusterCount;   
}

Mat BBOWTrainer::cluster() const
{
   Bkmeans _bkm;
   _bkm.cluster(_K);

   return _bkm.getCentroidsMat();
}


Mat BBOWTrainer::cluster( const cv::Mat& descriptors ) const
{
   Bkmeans _bkm;
   _bkm.setSamples(descriptors);
   _bkm.cluster(_K);
   return _bkm.getCentroidsMat();
}