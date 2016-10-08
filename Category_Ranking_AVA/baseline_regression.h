#include"head.h"

#include<opencv2/opencv.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<opencv2/contrib/contrib.hpp>

using namespace cv;


//----- contruct train file .xml, normalized .xml and normalization params
void trainXMLConstruct();

//----- construct test file .xml, normalize it with params
void testXMLConstruct();

//----- construct needed format feature file .xml
//----- label file .xml already exists
void datasetConstruct();

//----- train svm and predict 
void svmTrainPred();