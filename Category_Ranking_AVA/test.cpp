#include"head.h"
#include"DCNN_Feat.h"
#include"basic_function.h"
#include"LocalLearning.h"
#include"SelectPairTest.h"

#include<unordered_set>
#include<unordered_map>
#include<queue>
#include<set>
#include<stack>
#include<limits>

void consSimi();
void consSimiOne(float );

void predSimi();
void predSimiOne(float score);

void consSimi()
{
	vector<float> scoreVec(4, -1);

	for (int i = 0; i < scoreVec.size(); i++)
	{
		scoreVec[i] = 0.6 + 0.1*i;
		consSimiOne(scoreVec[i]);
	}
}

void consSimiOne(float scoreGap)
{

	string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Similarity_based\\score_" + 
		FormatTransform::int2str(int(scoreGap*10)) + "\\";

	cout<<pathBase<<endl;
	
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
	int num = 20;
	int step = 5;
	vector<ofstream> fs(num);
	vector<int> numBound(num, 0);

	for (int i = 0; i < fs.size(); i++)
	{
		fs[i].open(pathBase + "k_" + FormatTransform::int2str(step*(i+1)) + ".txt");
		if (!fs[i].is_open())
		{
			cout<<"cannot open file: "<<pathBase + "k_" + FormatTransform::int2str(step*(i+1)) 
				+ ".txt"<<endl;
			return;
		}

		numBound[i] = step * (i + 1);
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
			featIndexQ++;
			
			cout<<cateNames[i] + "\\" + imgNames[j] + "	\t"<<scoreGap<<" \t"<<featIndexQ<<endl;
			//--- write query img info to 15 files
			qid ++;

			float scoreQ = scoreVec[featIndexQ];

			for (int k = 0; k < fs.size(); k++)
			{
				fs[k]<<"1 qid:"<<qid<<" 1:"<<featIndexQ<<endl;
			}

			//--- the vector to save the featIndex smaller than query image
			vector<int> aRow(100, -1);
			vector<vector<int>> fIndexVec(fs.size(), aRow);

			vector<int> indexB(fs.size(), 0); //index  of image that with bigger score
			vector<int> indexS(fs.size(), 0); //index  of image that with smaller score


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
					for (int k = 0; k < fs.size(); k++)
					{
						if (indexB[k] + indexS[k] >= numBound[k])
						{
							if (k==fs.size() - 1) flag15 = true; //k_1500 is full
							continue;
						}
						
						fIndexVec[k][indexS[k]] = featIndexN;
						indexS[k]++;
					}
				}
				else if (scoreN > scoreQ + scoreGap){
					for (int k = 0; k < fs.size(); k++)
					{
						if (indexB[k] + indexS[k] >= numBound[k])
						{
							if (k==fs.size()-1) flag15 = true; //k_1500 is full
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
			for (int k = 0; k < fs.size(); k++)
			{
				fs[k]<<"2 qid:"<<qid<<" 1:"<<featIndexQ<<endl;

				for (int m = 0; m < indexS[k]; m++)
				{
					fs[k]<<"1 qid:"<<qid<<" 1:"<<fIndexVec[k][m]<<endl;
				}
			}
		}
	}

	for (int i = 0; i < fs.size(); i++)
	{
		fs[i].close();
	}
}

void predSimi()
{
	vector<int> scoreVec(5, -1);

	scoreVec[0] = 4;
	for (int i = 1; i < scoreVec.size(); i++)
	{
		scoreVec[i] = 5 + i;
	}
	
	for (int i = 0; i < scoreVec.size(); i++)
	{
		predSimiOne(scoreVec[i]);
	}
}

void predSimiOne(float score)
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

	int num = 16;
	int step = 5;
	for (int i = 0; i < num; i++)
	{
		int k = (i+1) * step;
		string trainFile = pathBase + "k_" + FormatTransform::int2str(k) + ".txt";
		cout<<"score simi based----------score: "<<score<<"\tk:"<<k<<endl;

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

void Merge(vector<int>& nums, int start, int middle, int end)
{
	vector<int> vec(end-start+1);
	int iLeft = start;
	int iRight = middle+1;

	int curr = 0;	
	while (iLeft<=middle && iRight<=end)
	{
		vec[curr++] = min(nums[iLeft], nums[iRight]);
		if (nums[iLeft]<=nums[iRight])
			iLeft++;
		else iRight++;
	}

	while (iLeft<=middle)
		vec[curr++] = nums[iLeft++];
	while (iRight<=end)
		vec[curr++] = nums[iRight++];
	for (int i=0; i<vec.size(); i++)
		nums[start+i] = vec[i];
}

void MergeSort(vector<int>& nums, int start, int end)
{
	if (start >= end)
		return;

	MergeSort(nums, start, (start+end)/2);
	MergeSort(nums, (start+end)/2+1, end);
	// merge and copy, additional O(n) sapce complexity	
	Merge(nums, start, (start+end)/2, end);
}

void QuickSort(vector<int>& nums, int start, int end)
{
	if (start >= end)
		return;

	int curr = start;
	int median = nums[start];
	for (int i=start; i<=end; i++)
	{
		if ( nums[i]<median )
		{
			swap(nums[curr], nums[i]);
			curr++;
			swap(nums[curr], nums[i]);
		}
	}
	QuickSort(nums, start, curr-1);
	QuickSort(nums, curr+1, end);
}

void ImageScore()
{
	string path = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_test\\";

	Directory dir;
	vector<string> fileNames = dir.GetListFolders(path, "*", false);
	for (int i=0; i<fileNames.size(); i++)
	{
		string currPath = path + fileNames[i] + "\\";
		vector<string> imgFiles = dir.GetListFiles(currPath, "*.jpg", false);

		ofstream fs(currPath + "features_result\\ori_image_scores.txt");
		for (int j=0; j<imgFiles.size(); j++)
		{
			float score = -1;
			readScoreFile( currPath + imgFiles[j] + ".score", score);
			fs<<imgFiles[j]<<"\t"<<score<<endl;
		}
	}
}

bool contrain(vector<int> nums)
{
	unordered_set<int> numSet;
        for (int i=0; i<nums.size(); i++)
        {
            if (numSet.find(nums[i])==numSet.end())
                numSet.insert(nums[i]);
            return true;
        }
        return false;
}


void test()
{
	cout << "test" << endl;
	vector<int> nums(1, 0);
	//contrain(nums);


//	vector<int> nums(4, 1);
//	nums[0] = 0; 
//	for (auto num:nums)
//		cout<<num<<endl;

	return;
	/*
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA_14\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);

	int index = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		string currPath = trainPath + cateNames[i] + "\\features_result\\";

		string DCNN_Feat_Rank = currPath + "DCNN_Feat_Rank\\";
		makeDir(DCNN_Feat_Rank);

		string Result_compare = DCNN_Feat_Rank + "Result_compare\\";
		string Score_based = DCNN_Feat_Rank + "Score_based\\";
		makeDir(Result_compare);
		makeDir(Score_based);

		string Label_based = Result_compare + "Label_based\\";
		string R_Score_based = Result_compare + "Score_based\\";
		makeDir(Label_based);
		makeDir(R_Score_based);

		string Category_based = Score_based + "Category_based\\";
		string Random_based = Score_based + "Random_based\\";
		string Similarity_based = Score_based + "Similarity_based\\";
		makeDir(Category_based);
		makeDir(Random_based);
		makeDir(Similarity_based);


		string Random_600 = Random_based + "random_select_600\\";
		string Cate_60 = Category_based + "random_select_60\\";
		makeDir(Random_600);
		makeDir(Cate_60);

		for (int j=3; j<=10; j++)
		{
			string path = Similarity_based + "score_" + FormatTransform::int2str(j) + "\\";
			makeDir(path);
		}
	}
	*/

	/*------ svm-rank train and predict
	int score = 5;
	string pathBase = "E:\\Lab\\Category_Ranking_AVA\\score_based\\Similarity_based\\score_" + 
		FormatTransform::int2str(score) + "\\";
	string modelPath = pathBase + "model.dat";
	string params = " -c 42 ";

	string trainExe = "F:\\vs2012\\RankSVM_Learn\\x64\\Release\\RankSVM_Learn.exe";
	string exePath = "F:/Google/torrent/svm_rank_windows_1/";
	string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	int num = 20;

	for (int i = 0; i < num; i++)
	{
		int k = (i+1) * 5;
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
	*/
}



