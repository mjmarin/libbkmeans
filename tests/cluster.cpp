//--------------------------------------
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#include <ctime>

#define BKMEANS_WITH_OCV 0   // Do this before including bkmeans.h

#include <bkmeans.h>


using namespace std;

#define NSAMPLES 2000
#define NDIMS    128

int main(int argc, char ** argv)
{
   Bkmeans bkm(10);
   vector<Bsample> samples;
   int K = 200; //NSAMPLES/3;
   srand(time(NULL));

   
   // Create synthetic data
   for (int ix = 0; ix < NSAMPLES; ix++)
   {
      Bsample stmp;
      for (int i = 0; i < NDIMS; i++)
      {
         stmp.push_back(rand()%2);
         //cout << stmp[i];
      }
      samples.push_back(stmp);      
      //cout << endl;
   }
   /*
   // Create synthetic data
   int data[6][10] = {{1,0,0,0,0,0,0,0,0,0},{0,1,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{1,1,1,1,1,1,1,1,1,0},{1,1,1,1,1,1,1,1,0,1},{1,1,1,1,1,1,1,1,1,1}};
   for (int ix = 0; ix < 6; ix++)
   {
      Bsample stmp;
      for (int i = 0; i < 10; i++)
      {
         stmp.push_back(data[ix][i]);
         cout << stmp[i];
      }
      samples.push_back(stmp);      
      cout << endl;
   }
   K = 2;
   */

   // Run clustering
   bkm.setSamples(samples);

   cout << "Running clustering..." << endl;
   int res = bkm.cluster(K);
   cout << " done ";
   
   cout << "(" << res << ")" << endl;

   cout << "Final energy: " << bkm.getEnergy() << endl;

	return 0;
}
