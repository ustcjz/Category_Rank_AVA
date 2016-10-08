#include "score_based_random_based.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void constructScoreRandomDat()
{
	const string saveBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Random_based\\score_";
	vector<float> scoreVec(6, -1);

	for (int i = 0; i < scoreVec.size(); i++)
		scoreVec[i] = 0.5 * (i+1);

	for (int i = 0; i < scoreVec.size(); i++)
	{
	//	string saveFile = saveBase + FormatTransform::int2str(scoreVec[i]*10) + ".txt";
	//	constructOneScoreRandomDat(scoreVec[i], saveFile);
	}

	string saveFile = saveBase + FormatTransform::int2str(5) + ".txt";
	constructOneScoreRandomDat(0.5, saveFile);
}

void constructOneScoreRandomDat(float scoreGap, string saveFile)
{
	ofstream fs(saveFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file: "<<saveFile<<endl;
		return;
	}

	const string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Random_based\\score_";
	ofstream fs2(pathBase + "10.txt");
	ofstream fs3(pathBase + "15.txt");
	ofstream fs4(pathBase + "20.txt");
	ofstream fs5(pathBase + "25.txt");
	ofstream fs6(pathBase + "30.txt");

	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	vector<string> cateNames;
	int qid = 0;
	
	Directory dir;
	cateNames = dir.GetListFolders(trainPath, "*", false);

	//--- read all score into a vector
	int index = 0;
	vector<float> scoreVec(22000, -1);
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);

		for (int j = 0; j < scoreNames.size(); j++)
		{
			float score = -1;
			readScoreFile(currDir+scoreNames[j], score);
			scoreVec[index] = score;
			index++;
		}
	}
	// check if the score num is right

	
	int featIndex = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		vector<string> indexNames = dir.GetListFiles(currDir, "*.featIndex", false);
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);

		if (imgNames.size() != scoreNames.size() || imgNames.size() != indexNames.size())
		{
			cout<<"imgNames and scoreNames not match"<<endl;
			return;
		}

		for (int j = 0; j < imgNames.size(); j++)
		{
			cout<<cateNames[i]+imgNames[j]<<" \t\t"<<featIndex<<endl;
			//---tranverse all the imgs that are after the query image
			qid++;

			float scoreQ = scoreVec[featIndex];
			fs<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs2<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs3<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs4<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs5<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs6<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;

			vector<int> aNames(22000, -1);
			int aIndex = 0;

			vector<int> aNames2(22000, -1);
			int aIndex2 = 0;

			vector<int> aNames3(22000, -1);
			int aIndex3 = 0;

			vector<int> aNames4(22000, -1);
			int aIndex4 = 0;
			
			vector<int> aNames5(22000, -1);
			int aIndex5 = 0;

			vector<int> aNames6(22000, -1);
			int aIndex6 = 0;
			
			//---find the images behind the query image and with bigger score 
			for (int aI = featIndex + 1; aI < index; aI++)
			{
				//---the score of a after image
				float aScore = scoreVec[aI];
				int aIndexVal = aI;

				if (aScore < scoreQ - 0.5) // save the images with smaller score
				{
					aNames[aIndex] = aI;
					aIndex++;
				}else if (aScore > scoreQ + 0.5){ // write the images with bigger score
					fs<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

				if (aScore < scoreQ - 1.0){
					aNames2[aIndex2] = aI;
					aIndex2++;
				}else if (aScore > scoreQ + 1.0){
					fs2<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

				if (aScore < scoreQ - 1.5){
					aNames3[aIndex3] = aI;
					aIndex3++;
				}else if (aScore > scoreQ + 1.5){
					fs3<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

				if (aScore < scoreQ - 2){
					aNames4[aIndex4] = aI;
					aIndex4++;
				}else if (aScore > scoreQ + 2){
					fs4<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

				if (aScore < scoreQ - 2.5){
					aNames5[aIndex5] = aI;
					aIndex5++;
				}else if (aScore > scoreQ + 2.5){
					fs5<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

				if (aScore < scoreQ - 3){
					aNames6[aIndex6] = aI;
					aIndex6++;
				}else if (aScore > scoreQ + 3){
					fs6<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

			}

			//---write the imgs with smaller scores
			qid++;
			
			fs<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex; m++)
			{
				fs<<"1 qid:"<<qid<<" 1:"<<aNames[m]<<endl;
			}

			fs2<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex2; m++)
			{
				fs2<<"1 qid:"<<qid<<" 1:"<<aNames2[m]<<endl;
			}

			fs3<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex3; m++)
			{
				fs3<<"1 qid:"<<qid<<" 1:"<<aNames3[m]<<endl;
			}

			fs4<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex4; m++)
			{
				fs4<<"1 qid:"<<qid<<" 1:"<<aNames4[m]<<endl;
			}

			fs5<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex5; m++)
			{
				fs5<<"1 qid:"<<qid<<" 1:"<<aNames5[m]<<endl;
			}

			fs6<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex6; m++)
			{
				fs6<<"1 qid:"<<qid<<" 1:"<<aNames6[m]<<endl;
			}


			featIndex++;
		}
	}
}

void scoreRandomPred()
{
	string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Random_based\\Random_select\\";
	string modelPath = pathBase + "model.dat";
	string params = " -c 420 ";

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";
	string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	for (int i = 0; i < 6; i++)
	{
		int k = (i+1) * 5; 
		string trainFile = pathBase + "score_" + FormatTransform::int2str(k) + ".txt";
		string cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;

		cout<<"score = "<<k<<endl;
		
		exeCommand(cmdTrain);

		//--- test and predic for each category in testSet 
		for (int j = 0; j < cateNames.size(); j++)
		{
			string currDir = testPath + cateNames[j] + "\\features_result\\";
			string testFile = currDir + "dcnn_feat_dat.dat";
			string predFile = currDir + "DCNN_Feat_Rank\\Score_based\\Random_based\\" + 
				"pred_score_" + FormatTransform::int2str(k) + ".dat";

			string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
			exeCommand(cmdTest);
		}
	}
}

void constructScoreRandomDat_random()
{
	string saveFile = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Random_based\\Random_select\\score_5.txt";
	
	ofstream fs(saveFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file: "<<saveFile<<endl;
		return;
	}

	const string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Random_based\\Random_select\\score_";
	ofstream fs2(pathBase + "10.txt");
	ofstream fs3(pathBase + "15.txt");
	ofstream fs4(pathBase + "20.txt");
	ofstream fs5(pathBase + "25.txt");
	ofstream fs6(pathBase + "30.txt");

	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	vector<string> cateNames;
	int qid = 0;
	
	Directory dir;
	cateNames = dir.GetListFolders(trainPath, "*", false);

	//--- read all score into a vector
	int index = 0;
	vector<float> scoreVec(22000, -1);
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);

		for (int j = 0; j < scoreNames.size(); j++)
		{
			float score = -1;
			readScoreFile(currDir+scoreNames[j], score);
			scoreVec[index] = score;
			index++;
		}
	}
	// check if the score num is right

	
	int featIndex = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		vector<string> indexNames = dir.GetListFiles(currDir, "*.featIndex", false);
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);

		if (imgNames.size() != scoreNames.size() || imgNames.size() != indexNames.size())
		{
			cout<<"imgNames and scoreNames not match"<<endl;
			return;
		}

		for (int j = 0; j < imgNames.size(); j++)
		{
			cout<<cateNames[i]+imgNames[j]<<" \t\t"<<featIndex<<endl;
			//---tranverse all the imgs that are after the query image
			qid++;

			float scoreQ = scoreVec[featIndex];
			fs<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs2<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs3<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs4<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs5<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			fs6<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;

			vector<int> aNames(22000, -1);
			int aIndex = 0;

			vector<int> aNames2(22000, -1);
			int aIndex2 = 0;

			vector<int> aNames3(22000, -1);
			int aIndex3 = 0;

			vector<int> aNames4(22000, -1);
			int aIndex4 = 0;
			
			vector<int> aNames5(22000, -1);
			int aIndex5 = 0;

			vector<int> aNames6(22000, -1);
			int aIndex6 = 0;
			
			vector<int> count(6, 0); // for the random select
			//---find the images behind the query image and with bigger score 
			for (int aI = featIndex + 1; aI < index; aI++)
			{
				//---the score of a after image
				float aScore = scoreVec[aI];
				int aIndexVal = aI;

				if (aScore < scoreQ - 0.5) // save the images with smaller score
				{
					aNames[aIndex] = aI;
					aIndex++;
				}else if (aScore > scoreQ + 0.5){ // write the images with bigger score
					if (count[0] == 3)
					{
						fs<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
						count[0] = 0;
					}else count[0]++;
				}

				if (aScore < scoreQ - 1.0){
					aNames2[aIndex2] = aI;
					aIndex2++;
				}else if (aScore > scoreQ + 1.0){
					if (count[1] == 1)
					{
						fs2<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
						count[1] = 0;
					}else count[1] ++;
				}

				if (aScore < scoreQ - 1.5){
					aNames3[aIndex3] = aI;
					aIndex3++;
				}else if (aScore > scoreQ + 1.5){
					if (count[2] == 1)
					{
						fs3<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
						count[2] = 0;
					}else count[2]++;
				}

				if (aScore < scoreQ - 2){
					aNames4[aIndex4] = aI;
					aIndex4++;
				}else if (aScore > scoreQ + 2){
					if (count[3] == 1)
					{
						fs4<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
						count[3] = 0;
					}else count[3]++;
				}

				if (aScore < scoreQ - 2.5){
					aNames5[aIndex5] = aI;
					aIndex5++;
				}else if (aScore > scoreQ + 2.5){
					fs5<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

				if (aScore < scoreQ - 3){
					aNames6[aIndex6] = aI;
					aIndex6++;
				}else if (aScore > scoreQ + 3){
					fs6<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
				}

			}

			//---write the imgs with smaller scores
			qid++;
			
			fs<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex; m+=4)
			{
				fs<<"1 qid:"<<qid<<" 1:"<<aNames[m]<<endl;
			}

			fs2<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex2; m+=4)
			{
				fs2<<"1 qid:"<<qid<<" 1:"<<aNames2[m]<<endl;
			}

			fs3<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex3; m+=4)
			{
				fs3<<"1 qid:"<<qid<<" 1:"<<aNames3[m]<<endl;
			}

			fs4<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex4; m+=4)
			{
				fs4<<"1 qid:"<<qid<<" 1:"<<aNames4[m]<<endl;
			}

			fs5<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex5; m+=4)
			{
				fs5<<"1 qid:"<<qid<<" 1:"<<aNames5[m]<<endl;
			}

			fs6<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;
			for (int m = 0; m < aIndex6; m+=4)
			{
				fs6<<"1 qid:"<<qid<<" 1:"<<aNames6[m]<<endl;
			}


			featIndex++;
		}
	}
}
