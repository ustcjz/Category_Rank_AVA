#include "score_based_simi_based_14.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void consScoreSimiDat_14()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_train\\";

	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);
	//--- construct the Map<string(imgName), int(featIndex)>
	//--- read all score into a vector
	map<string, int> imgToIndex;
	vector<float> scoreVec(getImgNum(trainPath)+10, -1);
	int index = 0;

	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);

		if (imgNames.size() != scoreNames.size())
		{
			cout<<"size not match"<<endl;
			return;
		}

		for (int j = 0; j < scoreNames.size(); j++)
		{
			float score = -1;
			readScoreFile(currDir+scoreNames[j], score);
			scoreVec[index] = score; // save the score
	
			string insStr = cateNames[i] + "\\" + imgNames[j];
			imgToIndex.insert(pair<string, int>(insStr, index)); // insert a pair to map

			index++;
		}
	}


	vector<float> scoreGapVec(8, -1);
	vector<string> saveFileVec(20, "");
	vector<int> numBoundVec(20, -1);
	const string saveBase = "E:\\Lab\\Category_Ranking_AVA_14\\score_based\\Similarity_based\\";
	int kNum = 0;
	
	for (int i = 0; i < scoreGapVec.size(); i++)
	{
		scoreGapVec[i] = 0.3 + 0.1*i;
		string currSavePath = saveBase + "score_" + FormatTransform::int2str(scoreGapVec[i]*10) + "\\";
		makeDir(currSavePath); // make folder

		for (int j = 0; j < saveFileVec.size(); j++) // save 20 files once
		{
			kNum = 5*(j+1);
			saveFileVec[j] = currSavePath + "k_" + FormatTransform::int2str(kNum) + ".txt";
			numBoundVec[j] = kNum;
		}

		consOneScoreSimiDat_14( scoreGapVec[i], saveFileVec, numBoundVec, scoreVec, imgToIndex);
	}

}

void consOneScoreSimiDat_14(float scoreGap, vector<string> saveFileVec, vector<int> numBoundVec, 
							vector<float> scoreVec, map<string, int>& imgToIndex)
{
	// fixed variables
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_train\\";
	const string retrTblPath = "E:\\Lab\\Category_Ranking_AVA_14\\DCNN_test\\category_based_train\\";
	int numRow = 100;
	int imgNumTotal = getImgNum(trainPath);

	vector<ofstream> fsVec(saveFileVec.size()); // open saveFileVec
	for (int i = 0; i < fsVec.size(); i++)
	{
		fsVec[i].open(saveFileVec[i]);
		if ( !fsVec[i].is_open() )
		{
			cout<<"cannot open file: "<<saveFileVec[i]<<endl;
			return;
		}
	}


	Directory dir;
	vector<string> cateNames = dir.GetListFolders( trainPath, "*", false );

	//--- traverse for each query image
	int featIndexQ = -1;
	int qid = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		
		for (int j = 0; j < imgNames.size(); j++)
		{
			featIndexQ++;
			qid ++;
			cout<<cateNames[i] + "\\" + imgNames[j] + "	\t"<<scoreGap<<" \t"<<featIndexQ<<endl;

			for (int k = 0; k < fsVec.size(); k++) // write query img info to 15 files
			{
				fsVec[k]<<"1 qid:"<<qid<<" 1:"<<featIndexQ<<endl;
			}

			vector<int> aRow(numRow, -1);
			vector<vector<int>> fIndexVec(fsVec.size(), aRow); // the vector to save the featIndex smaller than query image
			vector<int> indexB(fsVec.size(), 0); //index  of image that with bigger score
			vector<int> indexS(fsVec.size(), 0); //index  of image that with smaller score

			ifstream fsNei(retrTblPath + cateNames[i] + "\\" + imgNames[j] + ".txt");// open neighbor name table
			if (!fsNei.is_open())
			{
				cout<<"cannot open file: "<<retrTblPath + cateNames[i] + "\\" + imgNames[j]<<endl;
				return;
			}

			float scoreQ = scoreVec[featIndexQ];
			string tmp1 = "";
			int times = 0;
			bool flagAll = false;// all file are full or not flag

			while(!fsNei.eof() && times < imgNumTotal) // find all the bigger images
			{
				fsNei>>tmp1;
				times++;

				//--- the score and featIndex of a neighbor
				int featIndexN = -1;
				float scoreN = -1;
				featIndexN = imgToIndex[tmp1];
				scoreN = scoreVec[featIndexN];

				if (scoreN < scoreQ - scoreGap) // smaller neighbors
				{
					for (int k = 0; k < fsVec.size(); k++)
					{
						if (indexB[k] + indexS[k] >= numBoundVec[k])
						{
							if ( k==fsVec.size()-1 ) flagAll = true; // all saveFile is full
							continue;
						}
						
						fIndexVec[k][indexS[k]] = featIndexN;
						indexS[k]++;
					}
				}
				else if (scoreN > scoreQ + scoreGap){
					for (int k = 0; k < fsVec.size(); k++)
					{

						if (indexB[k] + indexS[k] >= numBoundVec[k])
						{
							if ( k==fsVec.size()-1 ) flagAll = true; // all saveFile is full
							continue;
						}

						fsVec[k]<<"2 qid:"<<qid<<" 1:"<<featIndexN<<endl;
						indexB[k]++;
					}
				}

				if (flagAll) break;
			}
			fsNei.close();


			qid ++;
			for (int k = 0; k < fsVec.size(); k++) // write all the smaller images
			{
				fsVec[k]<<"2 qid:"<<qid<<" 1:"<<featIndexQ<<endl;

				for (int m = 0; m < indexS[k]; m++)
				{
					fsVec[k]<<"1 qid:"<<qid<<" 1:"<<fIndexVec[k][m]<<endl;
				}
			}
		}
	}

	for (int i = 0; i < fsVec.size(); i++)
	{
		fsVec[i].close();
	}
}

void scoreSimiPredVec_14()
{
	for (int i = 8; i <= 10; i++)
		scoreSimiPred_14(i);

}

void scoreSimiPred_14(int score)
{
	string pathBase = "E:\\Lab\\Category_Ranking_AVA_14\\score_based\\Similarity_based\\score_" + 
		FormatTransform::int2str(score) + "\\";
	string modelPath = pathBase + "model.dat";
	//string params = " -c 0.01 ";
	string params = " -c 520 ";

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";

	string testPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	int num = 16;
	int step = 5;
	for (int i = 0; i < num; i++)
	{
		int k = (i+1) * step;
		string trainFile = pathBase + "k_" + FormatTransform::int2str(k) + ".txt";
		cout<<"score based & similarity based : score = "<<score<<", k = "<<k<<endl;

		string cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;
		
		exeCommand(cmdTrain);

		//--- test and predic for each category in testSet 
		for (int j = 0; j < cateNames.size(); j++)
		{
			string currDir = testPath + cateNames[j] + "\\features_result\\";
			string testFile = currDir + "dcnn_feat_dat.dat";
			string predFile = currDir + "DCNN_Feat_Rank\\Score_based\\Similarity_based\\score_" + 
				FormatTransform::int2str(score) + "\\pred_k_" + FormatTransform::int2str(k) + ".dat";

			string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
			exeCommand(cmdTest);
		}
	}
}