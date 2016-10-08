#include "result_compare_14.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void resCompLabelPred_14()
{
	const string pathBase = "E:\\Lab\\Category_Ranking_AVA_14\\result_compare\\label_based\\";
	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";

	string testPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);


	/*-----------------train and test with random(baseline) strategy--------------------*/
	string trainFile = pathBase + "feature_baseline.txt"; 
	string params = " -c 0.01 ";
	string modelPath = pathBase + "model_random.dat";
	string cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;
	exeCommand(cmdTrain);

	for (int j = 0; j < cateNames.size(); j++)
	{
		string currDir = testPath + cateNames[j] + "\\features_result\\";
		string testFile = currDir + "dcnn_feat_dat.dat";
		string predFile = currDir + "DCNN_Feat_Rank\\Result_compare\\Label_based\\pred_label_baseline.dat";
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		exeCommand(cmdTest);
	}


	/*----------------- train and test with cate_based(method_2) strategy-----------------------*/
	trainFile = pathBase + "feature_cate.txt";
	params = " -c 0.14 ";
	modelPath = pathBase + "model_cate.dat";
	cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;
	exeCommand(cmdTrain);

	for (int j = 0; j < cateNames.size(); j++)
	{
		string currDir = testPath + cateNames[j] + "\\features_result\\";
		string testFile = currDir + "dcnn_feat_dat.dat";
		string predFile = currDir + "DCNN_Feat_Rank\\Result_compare\\Label_based\\pred_label_method_2.dat";
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		exeCommand(cmdTest);
	}
}
