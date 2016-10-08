#include<iostream>
#include<fstream>

#include<opencv2/contrib/contrib.hpp>


using namespace std;
using namespace cv;

/*
	a full test of local learning
*/
void LocalLearningTest();

/*
	local learning for CUHKPQ 
	input: 
	K : select the top K relevant imgs
	saveFile: save the result of 7 categories
*/
void CUHKPQLocalLearning(int K, string saveFile);


/*
	form image pair using top relevant K examples, 
*/
bool LocalLearningPredict( string img1, string img2, int K);

/*
	read first K names in retrieval table 
*/
bool readRetrTable(string filepath, int K, vector<string>& imgNames);

/*
	judge whether two images can form a pair	
	option:
	0: ignore the category difference
	1: care the category difference
*/
bool isPair(string img1, string img2, int option);


/*
	judge whether a img is labeled as good or not?
	example :  animal\\B13011.jpg
 **/
bool isGood(string img1);


