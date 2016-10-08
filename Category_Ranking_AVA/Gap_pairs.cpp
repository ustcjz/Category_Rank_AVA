#include "Gap_pairs.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void constructFeatScore()
{
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\Gap_pairs\\train\\";
	const string path = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";

	ofstream fsFeat (savePath + "allFeat.dat");
	if (!fsFeat.is_open())
	{
		cout<<"cannot open file : "<<savePath+"allFeat.dat"<<endl;
		return;
	}

	ofstream fsScore (savePath + "allScore.dat");
	if (!fsScore.is_open())
	{
		cout<<"cannot open file : "<<savePath+"allScore.dat"<<endl;
		return;
	}

	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path+dirNames[i]+"\\";
		cout<<currDir<<endl;
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);

		for (int j = 0; j < imgNames.size(); j++)
		{
			//string imgNum = imgNames[j].substr(0, imgNames[j].size()-4);

			vector<float> feat(4096, -1);
			readBinaryFile(currDir+imgNames[j]+".feat", 4096, feat);
			for (int k = 0; k < feat.size(); k++)
			{
				fsFeat<<feat[k]<<" ";
			}
			fsFeat<<endl;

			float score = -1;
			readScoreFile(currDir+imgNames[j]+".score", score);
			fsScore<<score<<endl;
		}
	}

	fsFeat.close();
	fsScore.close();
}


/*
	construct the gap dat that C code ranksvm need
*/
void constructGapDat()
{
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\Gap_pairs\\train\\";
	const string path = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";

	const float gap = 0.5;
	const int orderM = 100;

	ofstream fsGap (savePath + "feature_dat_gap_05.dat");
	if (!fsGap.is_open())
	{
		cout<<"cannot open file: "<<savePath+"feature_dat_gap_05.dat"<<endl;
		return;
	}

	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path+dirNames[i]+"\\";
		cout<<currDir<<endl;
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);

		for (int j = 0; j < imgNames.size(); j++)
		{
			//string imgNum = imgNames[j].substr(0, imgNames[j].size()-4);

			vector<float> feat(4096, -1);
			readBinaryFile(currDir+imgNames[j]+".feat", 4096, feat);

			float score = -1;
			readScoreFile(currDir+imgNames[j]+".score", score);

			// the order num of one image
			int orderNum = orderM;
			//if (score <= 4)
			//{
			//	orderNum = orderM - ((int)(4-score)/gap+1);
			//}else if (score >= 6)
			//{
			//	orderNum = orderM + ((int)(score-6)/gap+1);
			//}
			orderNum = (int)(score/gap) + 1;
			cout<<currDir+imgNames[j]<<" : "<<orderNum<<endl;

			fsGap<<orderNum<<" "<<"qid:1 ";
			for (int k = 0; k < feat.size(); k++)
			{
				fsGap<<k+1<<":"<<feat[k]<<" ";
			}
			fsGap<<endl;
		}
	}
	fsGap.close();
}

/*
	train with gap dat and predict on each test set
*/
void gapPairPred()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\Gap_pairs\\train\\";
	const string trainFile = trainPath + "feature_dat_gap_05.dat";
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
		string dcnnPath = testSetPath + dirNames[i] + "\\features_result\\DCNN_Feat_Rank\\GapPair";

		string predFile = dcnnPath + "pred_gap_10.dat";
		string testFile = testSetPath + dirNames[i] + "\\features_result\\dcnn_feat_dat.dat";

		string cmdTest = exePath + "svm_rank_classify.exe" + " " + testFile + " " + modelFile + " " + predFile;
		exeCommand(cmdTest);
	}
}
