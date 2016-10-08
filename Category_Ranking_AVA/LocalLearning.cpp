#include"LocalLearning.h"
#include"DCNN_Feat.h"
#include"basic_function.h"
#include"head.h"


/*
	a full test of local learning
*/
void LocalLearningTest()
{
	int K = 50;
	string saveFile = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\DCNN_tmp\\K_50_option_0.txt";
	CUHKPQLocalLearning(K, saveFile);
}

/*
	local learning for CUHKPQ 
	input: 
	K : select the top K relevant imgs
	saveFile: save the result of 7 categories
*/
void CUHKPQLocalLearning(int K, string saveFile)
{
	//------ constant/inconstant variavle 
	const string testPath = "F:\\Lab\\My_CUHK_Dataset\\category_based_test\\";

	//------ enter a category and do local leaning for each possibles pairs
	Directory dir;
	vector<string> foldNames = dir.GetListFolders(testPath, "*", false);
	vector<float> accuracyVec(foldNames.size(), -1);

	int option = 0;
	clock_t st, end;
	for (int i=0; i<foldNames.size(); i++)
	{

		vector<string> imgNames = dir.GetListFiles(testPath+foldNames[i], "*.jpg", false);

		int pairNum = 0;
		int rightNum = 0;
		for (int j=0; j<imgNames.size()-1; j++)
		{
			for (int k=j+1; k<imgNames.size(); k++)
			{
				if (!isPair(imgNames[j], imgNames[k], option))	
					continue;
				pairNum ++;

				//--- predict for each pair
				st = clock();
				if (LocalLearningPredict(foldNames[i]+"\\"+imgNames[j], foldNames[i]+"\\"+imgNames[k], K))
					rightNum++;

				end = clock();
				cout<<"time consuming of category "<<foldNames[i]<<" is: "<<(double)(end-st)/CLOCKS_PER_SEC<<"s"<<endl;
			}
		}
		
		//--- calc the accuracy
		accuracyVec[i] = rightNum / pairNum;
	}

	//------ save result  
	ofstream fs(saveFile);
	if (!fs.is_open())
	{
		cout<<"cannot open file: "<<saveFile<<endl;
		return;
	}

	fs<<"local learning for Top "<<K<<endl<<endl;;	
	for (int i=0; i<foldNames.size(); i++)
	{
		fs<<foldNames[i]<<"\t"<<accuracyVec[i]<<endl;
	}
	fs.close();
}

/*
	form image pair using top relevant K examples, 
	output:
*/
bool LocalLearningPredict( string img1, string img2, int K)
{
	clock_t st, end;
	st = clock();
	//------ constant variable
	const string trainPath = "F:\\Lab\\My_CUHK_Dataset\\category_based_train\\";
	const string testPath = "F:\\Lab\\My_CUHK_Dataset\\category_based_test\\";
	const string retrTblPath = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\category_based_test\\";

	const string tmpTestFile = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\DCNN_tmp\\tmpTest.dat";
	const string tmpPredFile = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\DCNN_tmp\\tmpPred.dat";

	const string tableSuffix = ".txt";

	int featDim = 25;

	//--- form local training set
	vector<string> imgNames1(K, "");
	vector<string> imgNames2(K, "");
	

	readRetrTable(retrTblPath+img1+tableSuffix, K, imgNames1);
	readRetrTable(retrTblPath+img2+tableSuffix, K, imgNames2);
	end = clock();
	cout<<"time consuming of reading relevant imgnames is: "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;

	int trainSetSize = 2*K;
//	int pairNum = 0;
//	int option = 0;	
//	for (int i=0; i<imgNames1.size(); i++)
//	{
//		for (int j=0; j<imgNames2.size(); j++)
//		{
//			// judge whether two images can form a pair or NOT ?
//			if (isPair(imgNames1[i], imgNames2[j], option))
//			{
//				pairNum ++;
//			}
//		}
//	}

//	string localSetPath = retrTblPath + "";
	string localSetPath = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\DCNN_tmp\\localSet.dat";
	string modelPath = "F:\\Lab\\My_CUHK_Dataset\\DCNN_test\\DCNN_tmp\\localModel.dat";
	ofstream fs(localSetPath);
	if (!fs.is_open())
	{
		cout<<"connot open file: "<<localSetPath<<endl;
		return false;
	}

	vector<float> aesthFeatVec(featDim, -1);
	string aesthFile = "";
	const string featSuffix = ".aesthFeat";
	for (int i=0; i<K; i++)
	{
		aesthFile = trainPath + imgNames1[i] + featSuffix;

		readAesthFeatFile(aesthFile, featDim, aesthFeatVec);
			
		// TODO save aesthFeat to localSetFile
		if (isGood(imgNames1[i]))
			fs<<"2 ";
		else fs<<"1 ";
		fs<<"qid:1 ";
		for (int j=0; j<featDim; j++)
		{
			fs<<j+1<<":"<<aesthFeatVec[j]<<" ";
		}
		fs<<endl;
	}

	for (int i=0; i<K; i++)
	{
		aesthFile = trainPath + imgNames2[i] + featSuffix;

		readAesthFeatFile(aesthFile, featDim, aesthFeatVec);
			
		// TODO save aesthFeat to localSetFile
		if (isGood(imgNames2[i])) 
			fs<<"2 ";
		else fs<<"1 ";
		fs<<"qid:1 ";
		for (int j=0; j<featDim; j++)
		{
			fs<<j+1<<":"<<aesthFeatVec[j]<<" ";
		}
		fs<<endl;
	}

	fs.close();
	
	end = clock();
	cout<<"time consuming of writing featVec to file i/o is: "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;

	//------ train model
	string exeTrainPath	= "F:/Google/torrent/svm_rank_windows_1/";
	string params	= " -c 0.1 ";
	string cmdTrain	= exeTrainPath + "svm_rank_learn.exe" + " " + params + " "+ localSetPath + " "+ modelPath;
	exeCommand( cmdTrain);

	end = clock();
	cout<<"time consuming of training is: "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;

	//------ predict and 
	ofstream fsTest(tmpTestFile);
	if (!fsTest.is_open())
	{
		cout<<"cannot open file: "<<tmpTestFile<<endl;
		return false;
	}
	aesthFile = testPath + img1 + featSuffix;
	readAesthFeatFile( aesthFile, featDim, aesthFeatVec);
	
	if (isGood(img1)) 
		fsTest<<"2 ";
	else fsTest<<"1 ";
	fsTest<<"qid:1 ";
	for (int j=0; j<featDim; j++)
	{
		fsTest<<j+1<<":"<<aesthFeatVec[j]<<" ";
	}
	fsTest<<endl;

	aesthFile = testPath + img2 + featSuffix;
	readAesthFeatFile( aesthFile, featDim, aesthFeatVec);
	
	if (isGood(img2)) 
		fsTest<<"2 ";
	else fsTest<<"1 ";
	fsTest<<"qid:1 ";
	for (int j=0; j<featDim; j++)
	{
		fsTest<<j+1<<":"<<aesthFeatVec[j]<<" ";
	}
	fsTest<<endl;
 
	end = clock();
	cout<<"time consuming of writing vec to test file i/o is: "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;
 
	string exeTestPath = "F:/Google/torrent/svm_rank_windows_1/";
	string cmdTest = exeTestPath + "svm_rank_classify.exe" + " "+ tmpTestFile + " "+ modelPath + " "+ tmpPredFile;
	exeCommand( cmdTest);

	end = clock();
	cout<<"time consuming of testing is: "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;

	vector<float> scoreVec(2, -1);
	int rows = 2;
	scoreVec = ReadDataFromFile::readFloatLines(tmpPredFile, 2);

	if (isGood(img1) && scoreVec[0] >= scoreVec[1])
		return true;
	else if (isGood(img2) && scoreVec[1] >= scoreVec[0])
		return true;
	else return false;

	end = clock();
	cout<<"time consuming of return is: "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;
}


/*
	read first K names in retrieval table 
*/
bool readRetrTable(string filepath, int K, vector<string>& imgNames)
{
	ifstream fs(filepath);
	if (!fs.is_open())
	{
		cout<<"cannot open file: "<<filepath<<endl;
		return false;
	}

	imgNames = vector<string>(K, "");
	string tmp = "";
	int pos = -1;
	for (int i=0; i<K; i++)
	{
		fs>>tmp;
		imgNames[i] = tmp; // e.g : animal:\\B15842.jpg
	//	pos = tmp.find("\\");
	//	tmp.insert(pos, "\\");
	}
	fs.close();
}

/*
	judge whether two images can form a pair	
	option:
	0: ignore the category difference
	1: care the category difference
*/
bool isPair(string img1, string img2, int option)
{
	int flag1 = -1;
	int flag2 = -1;

	if (img1.find("G")!=-1)
		flag1 = 1;
	if (img2.find("G")!=-1)
		flag2 = 1;
	if (flag1*flag2 > 0)
		return false;
	else return true;
}

/*
	judge whether a img is labeled as good or not?
	example :  animal\\B13011.jpg
 **/
bool isGood(string img)
{
	int pos = img.find("\\");
	string imgName = img.substr(pos, img.size()-1);
	if (imgName.find("G")!=-1)
		return true;
	else return false;
}

