#include"Method_1.h"
#include"DCNN_Feat.h"
#include"basic_function.h"

//----- construct trainset, testset file already exists
void trainsetConstruct()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	
	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);

	for (int i = 0; i < cateNames.size(); i++)
	{
		string saveFile = trainPath + cateNames[i] + "\\features_result\\dcnn_feature_dat.dat";
		ofstream fs(saveFile);
		if (!fs.is_open())
		{
			cout<<"cannot open file: "<<saveFile<<endl;
			return ;
		}
		//--- read all dcnn files
		const int featDim = 4096;
		vector<string> dcnnFiles = dir.GetListFiles(trainPath + cateNames[i], "*.feat", false);
		vector<float> dcnnFeat(featDim, -1);

		string currCateName = trainPath + cateNames[i] ;
		for (int j = 0; j < dcnnFiles.size(); j++)
		{
			cout<<currCateName + "\\" + dcnnFiles[j]<<endl;
			readBinaryFile(currCateName+"\\"+dcnnFiles[j], featDim, dcnnFeat);
			
			//--- whether it is a good or bad
			if (dcnnFiles[j].find("G")!=-1)
				fs<<"2 ";
			else fs<<"1 ";
			fs<<"qid:1 ";

			for (int k = 0; k < dcnnFeat.size(); k++)
			{
				fs<<k+1<<":"<<dcnnFeat[k]<<" ";
			}
			fs<<endl;
		}
		fs.close();
	}
}

//----- train ranksvm and predict
void method_1_pred()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";

	const string exePath = "F:/Google/torrent/svm_rank_windows_1/";
	const string params = " -c 0.01 ";

	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);

	//----- train and predict for each category
	for (int i = 0; i < cateNames.size(); i++)
	{
		string trainFile = trainPath + cateNames[i] + "\\features_result\\dcnn_feat_dat.dat";
		string modelFile = trainPath + cateNames[i] + "\\features_result\\dcnn_model.dat";

		string cmdTrain = exePath + "svm_rank_learn.exe" + " " + params + " " + trainFile + " " + modelFile;
		exeCommand(cmdTrain);


		string dcnnPath = testPath + cateNames[i] + "\\features_result\\DCNN_Feat_Rank\\";
		string testFile = testPath + cateNames[i] + "\\features_result\\dcnn_feat_dat.dat";
		string predFile = dcnnPath + "pred_method_1.dat";
		
		string cmdTest = exePath + "svm_rank_classify.exe" + " " + testFile + " " + modelFile + " " + predFile;
		exeCommand(cmdTest);
	}

}
