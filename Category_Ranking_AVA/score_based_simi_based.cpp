#include "score_based_simi_based.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void consScoreSimiDat()
{
	vector<float> scoreVec(6, -1);

	for (int i = 0; i < scoreVec.size(); i++)
	{
		scoreVec[i] = 0.5*(i + 1);
		consOneScoreSimiaDat(scoreVec[i]);
	}
}


void consOneScoreSimiaDat(float scoreGap)
{
	string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Similarity_based\\score_" + 
		FormatTransform::int2str(int(scoreGap*10)) + "\\";

	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string retrTblPath = "F:\\Lab\\Category_Ranking_AVA\\DCNN_test\\category_based_train\\";

	vector<string> cateNames;
	Directory dir;
	cateNames = dir.GetListFolders(trainPath, "*", false);

	//--- construct the Map<string(imgName), int(featIndex)>
	//--- read all score into a vector
	map<string, int> imgToIndex;
	vector<float> scoreVec(22000, -1);
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
	// check if the score num is right


	//--- 15 ofstream and correspond info
	vector<ofstream> fs(15);
	vector<int> numBound(15, 0);

	for (int i = 0; i < fs.size(); i++)
	{
		fs[i].open(pathBase + "k_" + FormatTransform::int2str(100*(i+1)) + ".txt");
		if (!fs[i].is_open())
		{
			cout<<"cannot open file: "<<pathBase + "k_" + FormatTransform::int2str(100*(i+1)) 
				+ ".txt"<<endl;
			return;
		}

		numBound[i] = 100 * (i + 1);
	}

	//--- traverse for each query image
	int featIndexQ = -1;
	int qid = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		
		for (int j = 0; j < imgNames.size(); j++)
		{
			cout<<cateNames[i] + "\\" + imgNames[j] + "	\t"<<scoreGap<<" \t"<<featIndexQ<<endl;

			featIndexQ++;
			//--- write query img info to 15 files
			qid ++;

			float scoreQ = scoreVec[featIndexQ];

			for (int k = 0; k < 15; k++)
			{
				fs[k]<<"1 qid:"<<qid<<" 1:"<<featIndexQ<<endl;
			}

			//--- the vector to save the featIndex smaller than query image
			vector<int> aRow(1500, -1);
			vector<vector<int>> fIndexVec(15, aRow);

			vector<int> indexB(15, 0); //index  of image that with bigger score
			vector<int> indexS(15, 0); //index  of image that with smaller score


			//--- traverse for each neighbor0
			ifstream fsNei(retrTblPath + cateNames[i] + "\\" + imgNames[j] + ".txt");
			if (!fsNei.is_open())
			{
				cout<<"cannot open file: "<<retrTblPath + cateNames[i] + "\\" + imgNames[j]<<endl;
				return;
			}

			string tmp1 = "";
			int times = 0;
			bool flag15 = false;// k_1500 not full

			while(!fsNei.eof() && times < 21136) // find all the bigger images
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
					for (int k = 0; k < 15; k++)
					{
						if (indexB[k] + indexS[k] >= numBound[k])
						{
							if (k==15) flag15 = true; //k_1500 is full
							continue;
						}
						
						fIndexVec[k][indexS[k]] = featIndexN;
						indexS[k]++;
					}
				}
				else if (scoreN > scoreQ + scoreGap){
					for (int k = 0; k < 15; k++)
					{
						if (indexB[k] + indexS[k] >= numBound[k])
						{
							if (k==15) flag15 = true; //k_1500 is full
							continue;
						}

						fs[k]<<"2 qid:"<<qid<<" 1:"<<featIndexN<<endl;
						indexB[k]++;
					}
				}

				if (flag15) break;
			}
			fsNei.close();

			//--- write all the smaller images
			qid ++;
			for (int k = 0; k < 15; k++)
			{
				fs[k]<<"2 qid:"<<qid<<" 1:"<<featIndexQ<<endl;

				for (int m = 0; m < indexS[k]; m++)
				{
					fs[k]<<"1 qid:"<<qid<<" 1:"<<fIndexVec[k][m]<<endl;
				}
			}
		}
	}

	for (int i = 0; i < 15; i++)
	{
		fs[i].close();
	}
}

void scoreSimiaPredVec()
{
	vector<int> scoreVec(6, -1);
	for (int i = 0; i < scoreVec.size(); i++)
	{
		scoreVec[i] = (i+1) * 5;
		scoreSimiPred(scoreVec[i]);
	}
}

void scoreSimiPred(int score)
{
	string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Similarity_based\\score_" + 
		FormatTransform::int2str(score) + "\\";
	string modelPath = pathBase + "model.dat";
	//string params = " -c 0.01 ";
	string params = " -c 420 ";

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";
	string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	for (int i = 0; i < 15; i++)
	{
		int k = (i+1) * 100;
		string trainFile = pathBase + "k_" + FormatTransform::int2str(k) + ".txt";
		cout<<"score simi based: \tscore: "<<score<<"\tk:"<<k<<endl;

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

