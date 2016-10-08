#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<direct.h>

#include<opencv2/opencv.hpp>
#include<opencv2/contrib/contrib.hpp>

using namespace std;
using namespace cv;
/*
	option:
			0
			1

	topK: top k nearest neighbors

*/
void selectPairTest(int option, int topK);


/*
	construct the trainFile that needed in MyRankSVM
*/
void selectPairTestForMyRankSVM(int option, int topK);


/*
	construct balanced trainfile, topK contains half "Good" and "Bad"
*/
void selectPairTestForMyRankSVM_balanced(int option, int topK);


/*
	construct trainfile, query and topK form topK paris
*/
void selectPairTestForMyRankSVM_1_1(int option, int topK);

/*
	construct trainfile, balanced 
*/
void selectPairTestForMyRankSVM_balanced_1_1(int option, int topK);


void selectTest();

/*
	input:
		str1:  animal 
		str2:  animal\\B10315.jpg
	output:
		whether belongs to the same category
*/
bool isSameCate(string str1, string str2);
