#include "score_based_cate_based_14.h"
#include "DCNN_Feat.h"
#include "basic_function.h"

void consScoreCateDat_14()
{
	//--- read all score into a vector
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_train\\";
	vector<string> cateNames;
	
	Directory dir;
	cateNames = dir.GetListFolders(trainPath, "*", false);

	int index = 0;
	vector<float> scoreVec( getImgNum(trainPath)+10, -1);
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


	//--- scoreGapVec and saveFileVec construction
	const string saveBase = "E:\\Lab\\Category_Ranking_AVA_14\\score_based\\Category_based\\random_select_60\\";
	vector<float> scoreGapVec(8, -1);
	vector<string> saveFileVec(8, "");
	vector<int> biliVec(8, 10);
	for (int i = 0; i < scoreGapVec.size(); i++)
	{
		scoreGapVec[i] = 0.3 + i*0.1;
		biliVec[i] = 60;
		saveFileVec[i] = saveBase + "score_" + FormatTransform::int2str( scoreGapVec[i]*10 ) + ".txt";
	}	

	//consOneScoreCateDat_14(scoreGapVec, saveFileVec, trainPath, scoreVec);
	consOneScoreCateDat_random_14(scoreGapVec, biliVec, saveFileVec, trainPath, scoreVec);
}

/*
	paris only consist of images from same category
*/
void consOneScoreCateDat_14(vector<float> scoreGapVec, vector<string> saveFileVec, string trainPath, vector<float>& scoreVec)
{
	if (scoreGapVec.size() != saveFileVec.size())
	{
		cout<<"size of scoreGap and saveFile not match"<<endl;
		return;
	}
	
	vector<ofstream> fsVec(saveFileVec.size());
	for (int i = 0; i < saveFileVec.size(); i++)
	{
		fsVec[i].open( saveFileVec[i] );
		if ( !fsVec[i].is_open() )
		{
			cout<<"cannot open file: "<<saveFileVec[i]<<endl;
			return;
		}
	}

	int imgNum = 3500; // the most images one category contain

	vector<string> cateNames;
	int qid = 0;
	
	Directory dir;
	cateNames = dir.GetListFolders(trainPath, "*", false);
	
	int featIndex = 0;
	int indexBound = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		vector<string> indexNames = dir.GetListFiles(currDir, "*.featIndex", false);
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);

		if (imgNames.size() != scoreNames.size() || imgNames.size() != indexNames.size()) // check if the nums are right
		{
			cout<<"imgNames and scoreNames not match"<<endl;
			return;
		}

		indexBound += imgNames.size();

		for (int j = 0; j < imgNames.size(); j++)
		{
			cout<<cateNames[i]+"\\"+imgNames[j]<<" \t\t"<<featIndex<<endl; // featIndex++ for next img

			qid++;

			float scoreQ = scoreVec[featIndex];

			for (int fsI = 0; fsI < fsVec.size(); fsI++)
			{
				fsVec[fsI]<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			}


			vector<int> aNames(imgNum+10, -1);
			vector<vector<int>> aNamesVec(fsVec.size(), aNames);
			vector<int> aIndexVec(fsVec.size(), 0);

			//---find the images behind the query image and with bigger score 
			for (int aI = featIndex + 1; aI < indexBound; aI++) // the end condition is the last image of this category
			{
				//---the score of a after image
				float aScore = scoreVec[aI];
				int aIndexVal = aI;

				for (int fsI = 0; fsI < fsVec.size(); fsI++)
				{
					if (aScore < scoreQ - scoreGapVec[fsI]) // save the images with smaller score
					{
						aNamesVec[fsI][aIndexVec[fsI]] = aI;
						aIndexVec[fsI]++;
					}else if (aScore > scoreQ + scoreGapVec[fsI]){ // write the images with bigger score
						fsVec[fsI]<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
					}
				}
			}

			//---write the imgs with smaller scores
			qid++;
			
			for (int fsI = 0; fsI < fsVec.size(); fsI++)
			{
				fsVec[fsI]<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;

				for (int m = 0; m < aIndexVec[fsI]; m++)
				{
					fsVec[fsI]<<"1 qid:"<<qid<<" 1:"<<aNamesVec[fsI][m]<<endl;
				}
			}

			featIndex++;
		}
	}

}


void consOneScoreCateDat_random_14(vector<float> scoreGapVec, vector<int>biliVec, vector<string> saveFileVec, string trainPath, vector<float>& scoreVec)
{
	if (scoreGapVec.size() != saveFileVec.size())
	{
		cout<<"size of scoreGap and saveFile not match"<<endl;
		return;
	}
	
	vector<ofstream> fsVec(saveFileVec.size());
	for (int i = 0; i < saveFileVec.size(); i++)
	{
		fsVec[i].open( saveFileVec[i] );
		if ( !fsVec[i].is_open() )
		{
			cout<<"cannot open file: "<<saveFileVec[i]<<endl;
			return;
		}
	}

	int imgNum = 3500; // the most images one category contain

	vector<string> cateNames;
	int qid = 0;

	Directory dir;
	cateNames = dir.GetListFolders(trainPath, "*", false);
	
	int featIndex = 0;
	int indexBound = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currDir = trainPath + cateNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		vector<string> indexNames = dir.GetListFiles(currDir, "*.featIndex", false);
		vector<string> scoreNames = dir.GetListFiles(currDir, "*.jpg.score", false);

		if (imgNames.size() != scoreNames.size() || imgNames.size() != indexNames.size()) // check if the nums are right
		{
			cout<<"imgNames and scoreNames not match"<<endl;
			return;
		}
		
		indexBound += imgNames.size();

		for (int j = 0; j < imgNames.size(); j++)
		{
			cout<<cateNames[i]+"\\"+imgNames[j]<<" \t\t"<<featIndex<<endl; // featIndex++ for next img

			qid++;

			float scoreQ = scoreVec[featIndex];

			for (int fsI = 0; fsI < fsVec.size(); fsI++)
			{
				fsVec[fsI]<<"1 qid:"<<qid<<" 1:"<<featIndex<<endl;
			}


			vector<int> aNames(imgNum+10, -1);
			vector<vector<int>> aNamesVec(fsVec.size(), aNames);
			vector<int> aIndexVec(fsVec.size(), 0);
			vector<int> count(fsVec.size(), 0);
		
			//---find the images behind the query image and with bigger score 
			for (int aI = featIndex + 1; aI < indexBound; aI++)
			{
				//---the score of a after image
				float aScore = scoreVec[aI];
				int aIndexVal = aI;

				for (int fsI = 0; fsI < fsVec.size(); fsI++)
				{
					if (aScore < scoreQ - scoreGapVec[fsI]) // save the images with smaller score
					{
						aNamesVec[fsI][aIndexVec[fsI]] = aI;
						aIndexVec[fsI]++;
					}else if (aScore > scoreQ + scoreGapVec[fsI]){ // write the images with bigger score
						
						if (count[fsI] == biliVec[fsI]-1 ) // random select
						{
							fsVec[fsI]<<"2 qid:"<<qid<<" 1:"<<aIndexVal<<endl;
							count[fsI] = 0;
						}else count[fsI]++;
					}
				}
			}

			//---write the imgs with smaller scores
			qid++;
			
			vector<int> countS(fsVec.size(), 0);	
			for (int fsI = 0; fsI < fsVec.size(); fsI++)
			{
				fsVec[fsI]<<"2 qid:"<<qid<<" 1:"<<featIndex<<endl;

				for (int m = 0; m < aIndexVec[fsI]; m++)
				{
					if ( countS[fsI] == biliVec[fsI]-1 )
					{
						fsVec[fsI]<<"1 qid:"<<qid<<" 1:"<<aNamesVec[fsI][m]<<endl;
						countS[fsI] = 0;
					}else countS[fsI]++;
				}
			}

			featIndex++;
		}
	}
}

void scoreCatePred_14()
{
	string random_select = "random_select_60\\";
	string pathBase = "E:\\Lab\\Category_Ranking_AVA_14\\score_based\\Category_based\\" + random_select;
	string modelPath = pathBase + "model.dat";
	string params = " -c 520 "; // 26939 * 2 *0.01 ~= 540

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";

	string testPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_test\\";
	
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	//for (int i = 0; i < 6; i++)
	for (int i = 3; i <= 10; i++) // only score5.txt
	{
		int k = i; 
		cout<<"score based & cate based: score = "<<k<<endl;
		string trainFile = pathBase + "score_" + FormatTransform::int2str(k) + ".txt";
		string cmdTrain = trainExe + " " + params + " " + trainFile + " " + modelPath;
		
		exeCommand(cmdTrain);

		//--- test and predic for each category in testSet 
		for (int j = 0; j < cateNames.size(); j++)
		{
			string currDir = testPath + cateNames[j] + "\\features_result\\";
			string testFile = currDir + "dcnn_feat_dat.dat";
			string predFile = currDir + "DCNN_Feat_Rank\\Score_based\\Category_based\\" + random_select + 
				"pred_score_" + FormatTransform::int2str(k) + ".dat";

			string cmdTest = exePath + "svm_rank_classify.exe" + " "+ testFile + " "+ modelPath + " "+ predFile;
			exeCommand(cmdTest);
		}
	}
}