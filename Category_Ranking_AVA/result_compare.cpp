#include "result_compare.h"
#include "basic_function.h"
#include "DCNN_Feat.h"

void resCompLabelPred()
{
	const string pathBase = "E:\\Lab\\Category_Ranking_AVA\\result_compare\\label_based\\";
	const string modelPath = pathBase + "model.dat";
	const string trainFile = pathBase + "feature_method_2.txt"; 
	const string params = " -c 0.09 ";

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";

	string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	string cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;
	exeCommand(cmdTrain);

	//--- test and predic for each category in testSet 
	for (int j = 0; j < cateNames.size(); j++)
	{
		string currDir = testPath + cateNames[j] + "\\features_result\\";
		string testFile = currDir + "dcnn_feat_dat.dat";
		string predFile = currDir + "DCNN_Feat_Rank\\Result_compare\\Label_based\\pred_label_method_2.dat";
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		exeCommand(cmdTest);
	}
}

void resCompScorePred()
{
	/*-------------- dataset construction -----------------*/
	/*
	//--- multi-class 
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\train\\result_compare\\score_based\\";	
	const string saveFile = "mul_cls_method_2_10.txt";
	const float gap = 1.0;

	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);

	ofstream fs (savePath + saveFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file : "<<savePath + saveFile<<endl;
		return;
	}

	int index = 0;
	int qid = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currPath = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currPath, "*.jpg", false);
		vector<string> scoreNames = dir.GetListFiles(currPath, "*.jpg.score", false);

		if (imgNames.size() != scoreNames.size())
		{
			cout<<"size not match"<<endl;
			return;
		}

		qid++;
		for (int j = 0; j < imgNames.size(); j++)
		{
			float score = -1;
			readScoreFile(currPath + scoreNames[j], score);
			int cls = floor(score/gap);

			fs<<cls<<" qid:"<<qid<<" 1:"<<index<<endl;;
			cout<<cls<<"\t\t"<<index<<endl;
			index++;
		}
	}

	fs.close();
	*/
	/*-------------- dataset construction end ---------------*/

	/*-------------- svm-rank train and predict -------------*/
	const string pathBase = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\train\\result_compare\\score_based\\";
	const string params = " -c 0.01 ";

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";

	string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> txtNames = dir.GetListFiles(pathBase, "*.txt", false);

	for (int k = 0; k < txtNames.size(); k++)
	{
		cout<<txtNames[k]<<endl;

		string modelPath = pathBase + "model.dat";
		string trainFile = pathBase + txtNames[k];

		string cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;
		exeCommand(cmdTrain);

		vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

		//--- test and predic for each category in testSet 
		for (int j = 0; j < cateNames.size(); j++)
		{
			string currDir = testPath + cateNames[j] + "\\features_result\\";
			string testFile = currDir + "dcnn_feat_dat.dat";
			string predFile = currDir + "DCNN_Feat_Rank\\Result_compare\\Score_based\\" 
				+ txtNames[k].substr(0, txtNames[k].size()-4) + ".dat";
			string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
			cout<<predFile<<endl;
			exeCommand(cmdTest);
		}
	}
	/*-------------- svm-rank trani and predict end ---------*/
}