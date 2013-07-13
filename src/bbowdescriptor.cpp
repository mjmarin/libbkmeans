//--------------------------------------
//! \author Manuel J Marin-Jimenez
//! \date July 2013
//--------------------------------------

#include <bbowdescriptor.h>

BBOWImgDescriptorExtractor::BBOWImgDescriptorExtractor()
{_status = 0;}


void BBOWImgDescriptorExtractor::setVocabulary(const Mat & vocab)
{
   _dictionary = vocab;
}

int BBOWImgDescriptorExtractor::descriptorSize() const
{
   return _dictionary.size();
}