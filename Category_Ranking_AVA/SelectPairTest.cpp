#include"SelectPairTest.h"
#include"LocalLearning.h"
#include"DCNN_Feat.h"
#include"basic_function.h"
#include"head.h"

string int2str(int a)
{
	char temp[64];
	string str;
	_itoa(a, temp, 10);
	str = temp;	
	return str;
}

/*
	input:
		str1:  animal 
		str2:  animal\\B10315.jpg
	output:
		whether belongs to the same category
*/
bool isSameCate(string str1, string str2)
{
	if (str2.find(str1)!=-1)
		return true;
	return false;
}

void selectTest()
{
	int Kmin = 50;
	int Kmax = 800;
	int option = 0;

	for (int topK=Kmin; topK<=Kmax; topK+=50)
	{
		selectPairTestForMyRankSVM_balanced(option, topK);
		selectPairTestForMyRankSVM(option, topK);
	}
//	for (int topK=Kmin; topK<=Kmax; topK+=50)
//	{
//		selectPairTestForMyRankSVM_1_1(option, topK);
//	}
	//	selectPairTest(option, topK);
}
/*
	option:
			0
			1

	topK: top k nearest neighbors

	feature used: aesthetic feature, not DCNN feature
*/
void selectPairTest(int option, int topK)
{
	//--- variables 
	const string srcFold = "F:\\Lab\\My_CUHK_Dataset\\category_based_train\\";
	const string savePath = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\SelectPair\\option_0\\";

	const string trainPathBase = "F:\\Lab\\My_CUHK_Dataset\\category_based_train\\";
	const string neighTbPath = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\category_based_train\\";

	const string aesthSuffix = ".aesthFeat";
	const string neighSuffix = ".txt";
	const int featDim = 25;


	Directory dir;
	vector<string> foldNames = dir.GetListFolders(trainPathBase, "*", false);

	//--- trainFile to save formed pair of K neighbors
	string trainFile = savePath+"k_"+int2str(topK)+".txt";

	//--- form train pairs to trainFile
	ofstream fs(trainFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file "<<trainFile<<endl;
		return ;
	}
	
	int qid = 0;
	for (int i=0; i<foldNames.size(); i++)
	{
		vector<string> imgNames = dir.GetListFiles(trainPathBase+foldNames[i]+"\\",
			"*.jpg", false);

		for (int j=0; j<imgNames.size(); j++)
		{
			cout<<trainPathBase+foldNames[i]+"\\"+imgNames[j]<<" "<<topK<<endl;
			qid ++;
			vector<float> aesthFeat(featDim, -1);

			//--- write easthfeat of query image
			readAesthFeatFile(trainPathBase+foldNames[i]+"\\"+imgNames[j]+aesthSuffix, 
				featDim, aesthFeat);
			if (isGood(foldNames[i]+"\\"+imgNames[j]))
				fs<<"2 qid:"<<qid<<" ";
			else fs<<"1 qid:"<<qid<<" ";

			for (int m=0; m<aesthFeat.size(); m++)
			{
				fs<<m+1<<":"<<aesthFeat[m]<<" ";
			}
			fs<<endl;
			
			//--- retrieval the topK neighbors
			vector<string> neighborNames;
			readRetrTable(neighTbPath+foldNames[i]+"\\"+imgNames[j]+neighSuffix,
				topK, neighborNames);


			//--- write the test file with specilized "qid" 
			for (int k=0; k<neighborNames.size(); k++)
			{
				//--- option 1/4, need cate info
				//if (!isSameCate(foldNames[i], neighborNames[k]))
				//	continue;
				
				string aesthFeatFile = srcFold+neighborNames[k]+aesthSuffix;
				readAesthFeatFile(aesthFeatFile, featDim, aesthFeat);
				
				if (isGood(neighborNames[k]))
					fs<<"2 qid:"<<qid<<" ";
				else fs<<"1 qid:"<<qid<<" ";

				for (int m=0; m<aesthFeat.size(); m++)
				{
					fs<<m+1<<":"<<aesthFeat[m]<<" ";
				}
				fs<<endl;
			}
		}
	}
	fs.close();

	/*
	// TODO train model 
	string modelPath = savePath+"model.dat";
	string exePath	= "F:/Google/torrent/svm_rank_windows_1/";
	string params	= " -c 0.01 ";

	string cmdTrain	= exePath + "svm_rank_learn.exe" + " " + params + " "+ trainFile + " "+ modelPath;
	execCommand(cmdTrain);

	// TODO test on each category
	const string testPath = "F:\\Lab\\My_CUHK_Dataset\\category_based_test\\";
	foldNames = dir.GetListFolders(testPath, "*", false);
	
	for (int i=0; i<foldNames.size(); i++)
	{
		string testFile = testPath+foldNames[i]+"\\features_result\\feature_dat.dat";
		string predFile = testPath+foldNames[i]+"\\features_result\\SelectPair\\option_0\\pred_"
			+"k_"+int2str(topK)+".dat";

		//test and predict
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		execCommand(cmdTest);
	}
	*/
}

/*
	construct the trainFile that needed in MyRankSVM
*/
void selectPairTestForMyRankSVM(int option, int topK)
{
	//--- variables 
	const string srcFold = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\SelectPair\\option_1\\";

	const string trainPathBase = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	
	//--- the path to save the neighbor table of each image
	const string neighTbPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\category_based_train\\";

	const string aesthSuffix = ".aesthFeat";
	const string neighSuffix = ".txt";
	const string indexSuffix = ".featIndex";

	const int featDim = 25;

	Directory dir;
	vector<string> foldNames = dir.GetListFolders(trainPathBase, "*", false);

	//--- trainFile to save formed pair of K neighbors
	string trainFile = savePath+"k_"+int2str(topK)+".txt";

	//--- form train pairs to trainFile
	ofstream fs(trainFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file "<<trainFile<<endl;
		return ;
	}
	
	int qid = 0;
	for (int i=0; i<foldNames.size(); i++)
	{
		vector<string> imgNames = dir.GetListFiles(trainPathBase+foldNames[i]+"\\",
			"*.jpg", false);

		for (int j=0; j<imgNames.size(); j++)
		{
			cout<<trainPathBase+foldNames[i]+"\\"+imgNames[j]<<" "<<topK<<endl;
			qid ++;
			vector<float> aesthFeat(featDim, -1);
			int featIndex = -1;

			//--- write featIndex of query image
			if (isGood(foldNames[i]+"\\"+imgNames[j]))
				fs<<"2 qid:"<<qid<<" ";
			else fs<<"1 qid:"<<qid<<" ";
			
			readFeatIndexFile(trainPathBase+foldNames[i]+"\\"+imgNames[j]+indexSuffix, featIndex);
			fs<<"1:"<<featIndex<<endl;

			//--- retrieval the topK neighbors
			vector<string> neighborNames;
			readRetrTable(neighTbPath+foldNames[i]+"\\"+imgNames[j]+neighSuffix,
				topK, neighborNames);


			//--- write the test file with specilized "qid" 
			for (int k=0; k<neighborNames.size(); k++)
			{
				//--- option 1/4, need cate info
				if (!isSameCate(foldNames[i], neighborNames[k]))
					continue;

				if (isGood(neighborNames[k]))
					fs<<"2 qid:"<<qid<<" ";
				else fs<<"1 qid:"<<qid<<" ";

				int featIndexN = -1;
				string featIndexFile = srcFold + neighborNames[k]+indexSuffix;
				readFeatIndexFile(featIndexFile, featIndexN);
				fs<<"1:"<<featIndexN<<endl;
				
			}
		}
	}
	fs.close();
	/*
	// TODO train model 
	string trainExe = "F:\\vs2012\\RankSVM_Learn\\Release\\RankSVM_Learn.exe";

	string modelPath = savePath+"model.dat";
	string exePath	= "F:/Google/torrent/svm_rank_windows_1/";
	string params	= " -c 0.01 ";

	string cmdTrain	= trainExe + " " + params + " "+ trainFile + " "+ modelPath;
	exeCommand(cmdTrain);

	// TODO test on each category
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	foldNames = dir.GetListFolders(testPath, "*", false);
	
	for (int i=0; i<foldNames.size(); i++)
	{
		string testFile = testPath+foldNames[i]+"\\features_result\\dcnn_feat_dat.dat";
		string predFile = testPath+foldNames[i]+"\\features_result\\DCNN_Feat_Rank\\SelectPair\\option_1\\pred_"
			+"k_"+int2str(topK)+".dat";

		//test and predict
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		exeCommand(cmdTest);
	}
	*/
}

/*
	construct balanced trainfile, topK contains half "Good" and "Bad"
*/
void selectPairTestForMyRankSVM_balanced(int option, int topK)
{
	//--- variables 
	const string srcFold = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\SelectPair\\option_1_balanced\\";

	const string trainPathBase = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	
	//--- the path to save the neighbor table of each image
	const string neighTbPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\category_based_train\\";

	const string aesthSuffix = ".aesthFeat";
	const string neighSuffix = ".txt";
	const string indexSuffix = ".featIndex";

	const int defaultTopK = 8000;
	int topKHalf = topK / 2;

	Directory dir;
	vector<string> foldNames = dir.GetListFolders(trainPathBase, "*", false);

	//--- trainFile to save formed pair of K neighbors
	string trainFile = savePath+"k_"+int2str(topK)+".txt";

	//--- form train pairs to trainFile
	ofstream fs(trainFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file "<<trainFile<<endl;
		return ;
	}

	int qid = 0;
	for (int i=0; i<foldNames.size(); i++)
	{
		vector<string> imgNames = dir.GetListFiles(trainPathBase+foldNames[i]+"\\",
			"*.jpg", false);

		for (int j=0; j<imgNames.size(); j++)
		{
			cout<<trainPathBase+foldNames[i]+"\\"+imgNames[j]<<" "<<topK<<endl;

			qid ++;
			int featIndex = -1;

			//--- write featIndex of query image
			if (isGood(foldNames[i]+"\\"+imgNames[j]))
				fs<<"2 qid:"<<qid<<" ";
			else fs<<"1 qid:"<<qid<<" ";
			
			readFeatIndexFile(trainPathBase+foldNames[i]+"\\"+imgNames[j]+indexSuffix, featIndex);
			fs<<"1:"<<featIndex<<endl;

			//--- retrieval the topK neighbors
			vector<string> neighborNames;
			readRetrTable(neighTbPath+foldNames[i]+"\\"+imgNames[j]+neighSuffix,
				defaultTopK, neighborNames);

			//--- write the test file with specilized "qid" 
			int goodNum = 0;
			int badNum = 0;
			for (int k=0; k<neighborNames.size(); k++)
			{
				//--- option 1/4, need cate info, option_1 with a filter
				if (!isSameCate(foldNames[i], neighborNames[k]))
					continue;
				if (goodNum >= topKHalf && badNum >= topKHalf)
					break;

				if (isGood(neighborNames[k]))
				{
					if (goodNum >= topKHalf)
						continue;

					fs<<"2 qid:"<<qid<<" ";
					goodNum ++;

					int featIndexN = -1;
					string featIndexFile = srcFold + neighborNames[k]+indexSuffix;
					readFeatIndexFile(featIndexFile, featIndexN);
					fs<<"1:"<<featIndexN<<endl;
				}
				else
				{
					if (badNum >= topKHalf)
						continue;

					fs<<"1 qid:"<<qid<<" ";
					badNum ++;	

					int featIndexN = -1;
					string featIndexFile = srcFold + neighborNames[k]+indexSuffix;
					readFeatIndexFile(featIndexFile, featIndexN);
					fs<<"1:"<<featIndexN<<endl;
				}
			}
		}
	}
	fs.close();

	/*
	// TODO train model 
	string trainExe = "F:\\vs2012\\RankSVM_Learn\\Release\\RankSVM_Learn.exe";

	string modelPath = savePath+"model.dat";
	string exePath	= "F:/Google/torrent/svm_rank_windows_1/";
	string params	= " -c 0.01 ";

	string cmdTrain	= trainExe + " " + params + " "+ trainFile + " "+ modelPath;
	exeCommand(cmdTrain);

	// TODO test on each category
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	foldNames = dir.GetListFolders(testPath, "*", false);
	
	for (int i=0; i<foldNames.size(); i++)
	{
		string testFile = testPath+foldNames[i]+"\\features_result\\dcnn_feat_dat.dat";
		string predFile = testPath+foldNames[i]+"\\features_result\\DCNN_Feat_Rank\\SelectPair\\option_1_balanced\\pred_"
			+"k_"+int2str(topK)+".dat";

		//test and predict
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		exeCommand(cmdTest);
	}
	*/
}
/*
	construct trainfile, query and topK form topK paris
*/
void selectPairTestForMyRankSVM_1_1(int option, int topK)
{
	//--- variables 
	const string srcFold = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string savePath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\DCNN_Feat_Rank\\SelectPair_1\\option_0\\";

	const string trainPathBase = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	
	//--- the path to save the neighbor table of each image
	const string neighTbPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\category_based_train\\";

	const string aesthSuffix = ".aesthFeat";
	const string neighSuffix = ".txt";
	const string indexSuffix = ".featIndex";

	const int featDim = 25;

	Directory dir;
	vector<string> foldNames = dir.GetListFolders(trainPathBase, "*", false);

	//--- trainFile to save formed pair of K neighbors
	string trainFile = savePath+"k_"+int2str(topK)+".txt";

	//--- form train pairs to trainFile
	ofstream fs(trainFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file "<<trainFile<<endl;
		return ;
	}
	
	int qid = 1;
	for (int i=0; i<foldNames.size(); i++)
	{
		vector<string> imgNames = dir.GetListFiles(trainPathBase+foldNames[i]+"\\",
			"*.jpg", false);

		for (int j=0; j<imgNames.size(); j++)
		{
			cout<<trainPathBase+foldNames[i]+"\\"+imgNames[j]<<" "<<topK<<endl;

			//--- retrieval the topK neighbors
			vector<string> neighborNames;
			readRetrTable(neighTbPath+foldNames[i]+"\\"+imgNames[j]+neighSuffix,
				topK, neighborNames);

			//--- write the test file with specilized "qid" 
			for (int k=0; k<neighborNames.size(); k++)
			{
				//--- option 1/4, need cate info
				//if (!isSameCate(foldNames[i], neighborNames[k]))
				//	continue;

				int featIndex = -1;

				//--- write featIndex of query image
				if (isGood(foldNames[i]+"\\"+imgNames[j]))
					fs<<"2 qid:"<<qid<<" ";
				else fs<<"1 qid:"<<qid<<" ";
				
				readFeatIndexFile(trainPathBase+foldNames[i]+"\\"+imgNames[j]+indexSuffix, featIndex);
				fs<<"1:"<<featIndex<<endl;



				//--- write featIndex of neighbor names
				if (isGood(neighborNames[k]))
					fs<<"2 qid:"<<qid<<" ";
				else fs<<"1 qid:"<<qid<<" ";

				int featIndexN = -1;
				string featIndexFile = srcFold + neighborNames[k]+indexSuffix;
				readFeatIndexFile(featIndexFile, featIndexN);
				fs<<"1:"<<featIndexN<<endl;
				
				qid ++;
			}
		}
	}
	fs.close();
	/*
	// TODO train model 
	string trainExe = "F:\\vs2012\\RankSVM_Learn\\Release\\RankSVM_Learn.exe";

	string modelPath = savePath+"model.dat";
	string exePath	= "F:/Google/torrent/svm_rank_windows_1/";
	string params	= " -c 0.01 ";

	string cmdTrain	= trainExe + " " + params + " "+ trainFile + " "+ modelPath;
	exeCommand(cmdTrain);

	// TODO test on each category
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	foldNames = dir.GetListFolders(testPath, "*", false);
	
	for (int i=0; i<foldNames.size(); i++)
	{
		string testFile = testPath+foldNames[i]+"\\features_result\\dcnn_feat_dat.dat";
		string predFile = testPath+foldNames[i]+"\\features_result\\DCNN_Feat_Rank\\SelectPair\\option_1\\pred_"
			+"k_"+int2str(topK)+".dat";

		//test and predict
		string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
		exeCommand(cmdTest);
	}
	*/
}

/*
	construct trainfile, balanced 
*/
void selectPairTestForMyRankSVM_balanced_1_1(int option, int topK)
{

}
