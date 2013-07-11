//--------------------------------------
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#include <custom.h>
/*
#ifndef BKMEANS_WITH_OCV 
  #define BKMEANS_WITH_OCV 0
#endif
  */

#ifndef _BKMEANS_H_
#define _BKMEANS_H_ 1

#include <iostream>
#include <vector>
#include <export.h>

#if BKMEANS_WITH_OCV > 0
 #include <cv.h>
 #include <opencv2/core/core.hpp> 
 #include <opencv2/features2d/features2d.hpp>
 #include <opencv2/highgui/highgui.hpp>
#endif 

#define BKMEANS_RET_CONV  1
#define BKMEANS_RET_MAXIT 2

typedef std::vector<unsigned char> Bsample;

/**
   \class Bkmeans
   This class implements (in a naive way) kmeans for binary data.
*/

class BKMEANS_EXPORT Bkmeans
{
   public:      
      Bkmeans(){_maxIters=1000; _energy=-1;};
      Bkmeans(int maxIters){_maxIters = maxIters; _energy=-1;};
      ~Bkmeans();

      // Set & get

      /** Set samples. This must be done before calling cluster()
      */
      void setSamples(std::vector<Bsample> & samples);

      #if BKMEANS_WITH_OCV > 0
      /** Accept mat data
      */
      void setSamples(cv::Mat & samplesMat);
      #endif

	  /** Returns a copy of the centroids
	  */
      std::vector<Bsample> getCentroids(void);

      unsigned long int getEnergy(void){return _energy;};

	  /** Returns true if there are no centroids (e.g. before cluster())	     
	  */
      bool isempty(void){return _centroids.size() == 0;}

      // Misc
	  /** Assign cluster id 
	      \param s Input sample
	      \return Cluster index
	  */
      int quantize(Bsample & s); 

	  /** Runs clustering on samples
	      \param K Number of clusters
		  \return Exit code
	  */
      int cluster(int K); //! Main procedure: runs kmeans

   private:
      std::vector<Bsample> _samples;
      std::vector<int> _centroids;    // Contains _samples indexes
      int _maxIters;
      unsigned long int _energy; // Current energy
	  
      // Useful methods
      int initRandCentroids(int K);
      unsigned long int intraClusterCost(std::vector<unsigned int> & ids, int idx);
      int * allDistances(std::vector<int> & membix);
      int computeNewCentroid(std::vector<int> & membix, int & mincost);

      /** Computes Hamming distance between two samples
      */
      #if BKMEANS_WITH_OCV > 0
      // Adapted from Wikipedia
      unsigned hamdist(Bsample & x, Bsample & y)
      {
         //std::cout << "Inside" << std::endl;
         unsigned dist = 0, val;
         int size = x.size();

         for (int i = 0; i < size; i++)
         {
            val = x[i] ^ y[i];
            // Count the number of set bits
            while(val)
            {
               ++dist; 
               val &= val - 1;
            }            
         }

         return dist;
      }
     #else
	  unsigned hamdist(Bsample & x, Bsample & y)
	  {
        //std::cout << "Old" << std::endl;
		  unsigned dist = 0;
		  int size = x.size();

		  for (int i = 0; i < size; i++)
           dist += (x[i] != y[i])?1:0;

		  return dist;
	  }
     #endif
};

#endif
