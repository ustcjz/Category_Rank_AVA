#include"head.h"

void cate_ranksvm()
{
	string cateBase = "F:\\Lab\\Category_Ranking_AVA\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders( cateBase, "*", false);

	string exePath = "F:\\Google\\torrent\\svm_rank_windows_1\\";
	string params  = " -c 0.01 ";
	for (int i=0; i < cateNames.size();i++)
	{
		// TODO path of specific category
		string catePath  = cateBase + cateNames[i] + "\\";
		string trainPath = catePath + "trainSet\\feature\\";
		string testPath  = catePath + "testSet\\feature\\";

		makedir(trainPath+"ranksvm\\");
		makedir(testPath+"ranksvm\\");

		string trainData = trainPath + "feature_not_dat.dat";
		string testData  = testPath + "feature_not_dat.dat";
		string testPred  = testPath + "ranksvm\\rank_pred.dat";
		string model	 = trainPath + "ranksvm\\ranksvm_model.dat";

		// TODO train rank model
		string cmdTrain = exePath + "svm_rank_learn.exe" + " " + params + " " + trainData + " " + model;
		string cmdTest  = exePath + "svm_rank_classify.exe" + " " + testData + " " + model + " " + testPred;

		// TODO test on testSet
		execCommand(cmdTrain);
		execCommand(cmdTest);
//		if (i==2)
//		{
//			execCommand(cmdTrain);
//			execCommand(cmdTest);
//		}
	}
}

void cateall_ranksvm()
{
	string cateBase = "F:\\Lab\\Category_Ranking_AVA\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders( cateBase, "*", false);

	string exePath = "F:\\Google\\torrent\\svm_rank_windows_1\\";
	string params  = " -c 0.1 ";

//	string trainPath = "F:\\Lab\\Category_Ranking_AVA\\cate_all\\trainSet\\feature\\";
//	string trainData = trainPath + "feature_not_dat_6.dat";
	string trainPath = "F:\\Lab\\Category_Ranking_AVA\\cate_fenlei\\trainSet\\feature\\";
	string trainData = trainPath + "feature_nor_dat.dat";

	string model	 = trainPath + "ranksvm\\ranksvm_model_all_6.dat";

	string cmdTrain = exePath + "svm_rank_learn.exe" + " " + params + " " + trainData + " " + model;
	execCommand(cmdTrain);

	for (int i=0; i < cateNames.size();i++)
	{
		// TODO path of specific category
		string catePath  = cateBase + cateNames[i] + "\\";
		string testPath  = catePath + "testSet\\feature\\";

//		makedir(trainPath+"ranksvm\\");
//		makedir(testPath+"ranksvm\\");

		string testData  = testPath + "feature_not_dat.dat";
		string testPred  = testPath + "ranksvm\\rank_pred_all_fenlei.dat";

		// TODO train rank model
		string cmdTest  = exePath + "svm_rank_classify.exe" + " " + testData + " " + model + " " + testPred;

		// TODO test on testSet
		execCommand(cmdTest);
	}
}

