//--------------------------------------
//! \file clustercv.cpp
//! \description This file is part of libbkmeans
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#include <ctime>

#include <bkmeans.h>
#include <bbowtrainer.h>

using namespace std;

using namespace cv;


int main(int argc, char ** argv)
{   
   Bkmeans bkm(10);
   vector<Bsample> samples;
   int K;
   srand(time(NULL));

   if (argc < 2)
   {
      cout << "ERROR: not enough number of arguments" << endl;
      cout << "SYNTAXIS: clustercv <imagepath> [<K>]" << endl;
      exit(-1);
   }

   Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
   if(!img_1.data)
   {
      std::cout<< "ERROR: cannot open image: " << argv[1] << std::endl; 
      return -1; 
   }

  //-- Step 1: Detect the keypoints using SURF Detector  
  std::vector<KeyPoint> keypoints_1;
  FAST(img_1, keypoints_1, 30, false);  

  Mat img_keypoints_1; 

  drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
  
  //-- Show detected (drawn) keypoints
  imshow("Keypoints 1", img_keypoints_1 );

  waitKey(0);

  // Compute descriptors from keypoints
  Ptr<DescriptorExtractor> extractor;
  //extractor = DescriptorExtractor::create("ORB");
  extractor = DescriptorExtractor::create("FREAK");
  //extractor = DescriptorExtractor::create("BRISK");

  Mat descriptors;
  extractor->compute(img_1, keypoints_1, descriptors);
  cout << descriptors.rows << " " << descriptors.cols << endl;
  
/*   
  cout << descriptors << endl;
*/

/*
   // Convert data
   for (int ix = 0; ix < descriptors.rows; ix++)
   {
      Bsample stmp;
      for (int i = 0; i < descriptors.cols; i++)
      {
         stmp.push_back(descriptors.at<unsigned char>(ix,i));
         //cout << stmp[i];
      }
      samples.push_back(stmp);      
      //cout << endl;
   }
*/
   bkm.setSamples(descriptors); // Set data
   
   // Set number of clusters
   if (argc > 2)
      K = atoi(argv[2]);
   else
      K = min(100, (int)samples.size()/3);

   // Run clustering
  

   cout << "Running clustering..." << endl;
   int res = bkm.cluster(K);
   cout << " done ";
   
   cout << "(" << res << ")" << endl;

   cout << "Final energy: " << bkm.getEnergy() << endl;

   /* Alternative */
   cout << "* Starting clustering with BBOWTrainer class " << endl;
   BBOWTrainer bbt(K);
   
   bbt.cluster(descriptors);   

	return 0;
}
