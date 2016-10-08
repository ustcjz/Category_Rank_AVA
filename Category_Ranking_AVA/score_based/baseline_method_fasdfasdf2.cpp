#include "baseline_method_2.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void constructBaselineDat()
{
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\train\\";
	ofstream fsBaselineDat(savePath + "feature_dat_baseline.dat");
	if (!fsBaselineDat.is_open())
	{
		cout<<"cannot open file: "<<savePath + "feature_dat_baseline.dat"<<endl;
		return;
	}

	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(trainPath, "*", false);

	const int featDim = 4096;
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = trainPath + dirNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		vector<string> dcnnFiles = dir.GetListFiles(currDir, "*.feat", false);

		if (imgNames.size() != dcnnFiles.size())
		{
			cout<<"img number and .feat num is not match"<<endl;
			return;
		}

		vector<float> dcnnFeat;
		for (int j = 0; j < dcnnFiles.size(); j++)
		{
			if (imgNames[j].find("G") != -1)
				fsBaselineDat<<"2 qid:1 ";
			else fsBaselineDat<<"1 qid:1 ";

			cout<<currDir+imgNames[j]<<endl;

			readBinaryFile(currDir+dcnnFiles[j], featDim, dcnnFeat);
			for (int k = 0; k < dcnnFeat.size(); k++)
			{
				fsBaselineDat<<k+1<<":"<<dcnnFeat[k]<<" ";
			}
			fsBaselineDat<<endl;
		}
	}
	fsBaselineDat.close();
}

void constructMethod_2Dat()
{
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\train\\";
	ofstream fsBaselineDat(savePath + "feature_dat_method_2.dat");
	if (!fsBaselineDat.is_open())
	{
		cout<<"cannot open file: "<<savePath + "feature_dat_baseline.dat"<<endl;
		return;
	}

	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(trainPath, "*", false);

	const int featDim = 4096;
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = trainPath + dirNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		vector<string> dcnnFiles = dir.GetListFiles(currDir, "*.feat", false);

		if (imgNames.size() != dcnnFiles.size())
		{
			cout<<"img number and .feat num is not match"<<endl;
			return;
		}

		vector<float> dcnnFeat;
		for (int j = 0; j < dcnnFiles.size(); j++)
		{
			if (imgNames[j].find("G") != -1)
				fsBaselineDat<<"2 qid:"<<i+1<<" ";
			else fsBaselineDat<<"1 qid:"<<i+1<<" ";

			cout<<currDir+imgNames[j]<<endl;

			readBinaryFile(currDir+dcnnFiles[j], featDim, dcnnFeat);
			for (int k = 0; k < dcnnFeat.size(); k++)
			{
				fsBaselineDat<<k+1<<":"<<dcnnFeat[k]<<" ";
			}
			fsBaselineDat<<endl;
		}
	}
	fsBaselineDat.close();
}

void baselinePred()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\train\\";
	const string trainFile = trainPath + "feature_dat_baseline.dat";
	const string modelFile = trainPath + "model.dat";

	string exePath = "F:/Google/torrent/svm_rank_windows_1/";
	string params = " -c 0.01 ";
	string cmdTrain = exePath + "svm_rank_learn.exe" + " " + params + " " + trainFile + " " + modelFile;
	exeCommand(cmdTrain);

	const string testSetPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(testSetPath, "*", false);
	for (int i = 0; i < dirNames.size(); i++)
	{
		string dcnnPath = testSetPath + dirNames[i] + "\\features_result\\DCNN_Feat_Rank\\";

		string predFile = dcnnPath + "pred_baseline.dat";
		string testFile = testSetPath + dirNames[i] + "\\features_result\\dcnn_feat_dat.dat";

		string cmdTest = exePath + "svm_rank_classify.exe" + " " + testFile + " " + modelFile + " " + predFile;
		exeCommand(cmdTest);
	}
}

void method_2Pred()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\train\\";
	const string trainFile = trainPath + "feature_dat_method_2.dat";
	const string modelFile = trainPath + "model.dat";

	string exePath = "F:/Google/torrent/svm_rank_windows_1/";
	string params = " -c 0.01 ";
	string cmdTrain = exePath + "svm_rank_learn.exe" + " " + params + " " + trainFile + " " + modelFile;
	exeCommand(cmdTrain);

	const string testSetPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(testSetPath, "*", false);
	for (int i = 0; i < dirNames.size(); i++)
	{
		string dcnnPath = testSetPath + dirNames[i] + "\\features_result\\DCNN_Feat_Rank\\";

		string predFile = dcnnPath + "pred_method_2.dat";
		string testFile = testSetPath + dirNames[i] + "\\features_result\\dcnn_feat_dat.dat";

		string cmdTest = exePath + "svm_rank_classify.exe" + " " + testFile + " " + modelFile + " " + predFile;
		exeCommand(cmdTest);
	}
}