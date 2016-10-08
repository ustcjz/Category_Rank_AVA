#include "head.h"

void constructFeatScore();


/*
	construct the gap dat that C code ranksvm need
*/
void constructGapDat();


/*
	train with gap dat and predict on each test set
*/
void gapPairPred();