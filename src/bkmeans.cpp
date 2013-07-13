//--------------------------------------
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <bkmeans.h>

using namespace std;

Bkmeans::~Bkmeans()
{
   _centroids.clear();
   _samples.clear();
}

void Bkmeans::setSamples(const std::vector<Bsample> & samples)
{
	if (!_samples.empty())
		_samples.clear();

	_samples = samples;
};

#if BKMEANS_WITH_OCV > 0
/** Accept mat data
*/
void Bkmeans::setSamples(const cv::Mat & samplesMat)
{
   if (!_samples.empty())
      _samples.clear();

   // Convert data
   for (int ix = 0; ix < samplesMat.rows; ix++)
   {
      Bsample stmp;
      for (int i = 0; i < samplesMat.cols; i++)
      {
         stmp.push_back(samplesMat.at<unsigned char>(ix,i));
         //cout << stmp[i];
      }
      _samples.push_back(stmp);      
      //cout << endl;
   }   
}
#endif

int Bkmeans::quantize(Bsample & s)
{
	int id = 0, mindist = 9999999, dist;
	if (_centroids.empty() || _samples.empty())
	{
		cerr << "ERROR: centroids or samples are empty" << endl;
		exit(-1);
	}

	// Compute distance with all centroids
	for (int i = 0; i < _centroids.size(); i++)
	{
       dist = hamdist(_samples[_centroids[i]], s);
	   if (dist < mindist)
	   {
		   mindist = dist;
		   id = i;
	   }
	}

	return id;
}

int Bkmeans::initRandCentroids(int K)
{
   int nsamples = _samples.size();
   srand(time(NULL));
   if (!isempty())
      _centroids.clear();

   if (K > nsamples)
   {
      std::cerr << "ERROR: K must be lower or equal than number of samples." << std::endl;
      return -1;
   }

   _centroids.reserve(K);
   std::vector<int> tmp(nsamples);
   for (int i = 0; i < nsamples; i++)
   {
      tmp[i] = i;
   }
   // Shuffle   
   std::random_shuffle ( tmp.begin(), tmp.end() );
//   cout << "Shuffled" << endl;

   // Take K firsts
   for (int i = 0; i < K; i++)
   {
//	   cout << tmp[i] << " ";
	   _centroids.push_back(tmp[i]);
	   
   }
   tmp.clear();

   // OK
   return 0;
}

std::vector<int> getIdsCluster(std::vector<unsigned int> ids, int idx)
{
   std::vector<int> membix;
   int N = ids.size();

   for (int i = 0; i < N; i++)
      if (ids[i] == idx)
         membix.push_back(i);

   return membix;
}

int * Bkmeans::allDistances(std::vector<int> & membix)
{
   int N = membix.size();
   int * D = new int[N*N];
   if (D == NULL)
   {
      cerr << "ERROR: out of memory" << endl;
      exit(-1);
   }
   memset(D, 0, N*N*sizeof(int));

   int i, j;

   for (i = 0; i < N - 1; i++)
      for (j = i+1; j < N; j++)
      {
         D[i*N+j] = hamdist(_samples[membix[i]], _samples[membix[j]]);
         D[j*N+i] = D[i*N+j];
		 //cout << "D[" << i << "," << j << "]=" << D[i*N+j] << endl;
      }

   return D;
}

int Bkmeans::computeNewCentroid(std::vector<int> & membix, int & mincost)
{
   int centix = 0;
   int * D = NULL;
   //int mincost = 99999999;
   int i, j, N = membix.size();

   if (N == 0)
	   return -1; // Empty cluster

   D = allDistances(membix);

   // Find the item with lowest distance to all other members
   for (i = 0; i < N; i++)
   {
      int cost = 0;
      for (j = 0; j < N; j++)
      {
         cost += D[i*N+j];
      }
      if (i == 0)
      {
         mincost = cost;
      }
      else
      {
         if (cost < mincost)
         {
            mincost = cost;
            centix = i;
         }
      }

   }

   delete [] D;

   return membix[centix];
}

unsigned long int Bkmeans::intraClusterCost(std::vector<unsigned int> & ids, int idx)
{
   unsigned long int cost = 0;
   int N;  

   std::vector<int> membix = getIdsCluster(ids, idx);
   N = membix.size();

   for (int i = 0; i < N-1; i++)
   {
      for (int j = i+1; j < N; j++)
         cost += hamdist(_samples[membix[i]], _samples[membix[j]]);
   }

   return cost;
}


int Bkmeans::cluster(int K)
{
   bool stop;
   int iter = 0, retcode = 0;
   unsigned long int totalcost = 0;
   int nsamples = _samples.size();
   std::vector<unsigned int> ids(nsamples);   

   // Init: choose initial centroids
   int iret = initRandCentroids(K);
//_centroids[0] = 0;
//_centroids[1] = 3;
   if (iret != 0)
   {
      cerr << "ERROR: cannot init centroids" << endl;
      return -1;
   }
   else
      cout << "Centroids size: " << _centroids.size() << endl;

   // To store cost of each cluster
   std::vector<unsigned long int> cost(K);

   stop = false;
   iter = 0;
//   cout << "To main loop" << endl;
   // Main Loop
   while(!stop)
   {
      cout << endl << "* Iteration: " << iter << endl;
      // Assign samples to centroids (quantize) and compute cost
      for (int i = 0; i < nsamples; i++)
      {
         ids[i] = quantize(_samples[i]);
		 //cout << ids[i] << " " ;
      }

      //cout << endl;
      totalcost = 0;
      //cout << "Cost: " << endl;
      for (int i = 0; i < K; i++)
      {
         cost[i] = intraClusterCost(ids, i);
         //cout << cost[i] << " ";
         totalcost += cost[i];
      }
      cout << "- Total cost: " << totalcost << endl;

      // Find new centroids: the one whose distance to the other members of group is the lowest
      int updates = 0;
      for (int i = 0; i < K; i++)
      {
         std::vector<int> membix = getIdsCluster(ids, i);
         int newcost = 0;
         int newcentroid = computeNewCentroid(membix, newcost);
         if (newcentroid != -1 && newcentroid != _centroids[i])
		 {
           _centroids[i] = newcentroid;
            updates++;
		 }
      }

      // Evaluate new cost and check stop condition
      if (updates == 0)
      {
	      stop = true;
         cout << endl << "INFO: algorithm fully converged" << endl;
		   retcode = BKMEANS_RET_CONV;
      }

      // New iteration finished
      iter++;
      if (iter >= _maxIters)
      {
         stop = true;
         cout << endl << "INFO: max iterations reached" << endl;
		 retcode = BKMEANS_RET_MAXIT;
      }
   }

   _energy = totalcost;

   return retcode;
}

std::vector<Bsample> Bkmeans::getCentroids(void) const
{
	int N = _centroids.size();
   if (N == 0)
   {
      cerr << "ERROR: empty centroids" << endl;
      exit(-1);
   }

	std::vector<Bsample> centroids;
	centroids.reserve(N);

   for (int i = 0; i < N; i++)
   {
      centroids.push_back(_samples[_centroids[i]]);
   }

   return centroids;
}

cv::Mat Bkmeans::getCentroidsMat(void) const
{
	int N = _centroids.size();
   if (N == 0)
   {
      cerr << "ERROR: empty centroids" << endl;
      exit(-1);
   }

   int len = _samples[0].size();
	cv::Mat centroids;
   centroids.create(0,len, CV_8UC1);

   for (int i = 0; i < N; i++)
   {
      //cv::Mat tmp(1,len, CV_8UC1);
      cv::Mat tmp(_samples[_centroids[i]], false); // Data is not copied
      /*
      for (int j = 0; j < len; j++)
      {
         tmp.at<uchar>(1,j) = _samples[_centroids[i]][j];
      }
      */
      //tmp = cv::Mat(_samples[_centroids[i]], false); // Data is not copied
      centroids.push_back(tmp);
   }

   return centroids;
}