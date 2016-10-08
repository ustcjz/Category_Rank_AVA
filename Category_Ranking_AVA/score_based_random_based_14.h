#include "score_based_head.h"

/*
	construct the data needed
*/
void constructScoreRandomDat_14();

void constructOneScoreRandomDat_14(vector<float> scoreGap, vector<string> saveFile, string trainPath, vector<float>& scoreVec);

/*
	train the pred with the dataset
*/
void scoreRandomPred_14();

/*
	too much example, random select with bili
*/
void constructScoreRandomDat_random_14(vector<float> scoreGap, vector<int> bili, vector<string> saveFile, string trainPath, vector<float>& scoreVec);