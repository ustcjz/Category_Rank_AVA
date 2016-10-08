#include "dataset_construct.h"
#include "DCNN_Feat.h"
/*
construct train and test set based on "init_label_index.txt" file
*/
void constructTrainTest()
{
	const string oriPath = "F:\\Lab\\Aesthetic_Quality\\AVA_dataset\\ImageSet\\";
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(trainPath, "*", false);

	bool flag = false;
	//--- construct train and test based on "init_label_index.txt"
	for (int i = 0; i < dirNames.size(); i++)
	{
		string indexFile = trainPath + dirNames[i] + "\\features_result\\init_label_index.txt";
		ifstream fs(indexFile);
		if (!fs.is_open())
		{
			cout<<"cannot open file: "<<indexFile<<endl;
			return;
		}

		string imgName = "";
		string imgLabel = "";
		string imgBG = "";

		while (!fs.eof())
		{
			fs>>imgName;
			if (fs.fail())
				break;
			fs>>imgLabel;
			if (fs.fail())
				break;

			cout<<imgName<<"\t"<<imgLabel<<endl;

			if (imgLabel == "0")
				imgBG = "B";
			else if (imgLabel == "1")
				imgBG = "G";
			else {
				cout<<"not right format :"<<imgName<<endl;
				continue;
			}

			Mat img;
			img = imread(oriPath + "trainSet\\" + imgName + "G.jpg");
			if ( img.rows != 0)
			{
				//--- save in trainset
				if (flag)
					imwrite(trainPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				else imwrite(testPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				flag = !flag;
				continue;
			}

			img = imread(oriPath + "trainSet\\" + imgName + "B.jpg");
			if ( img.rows != 0)
			{
				//--- save in trainset
				if (flag)
					imwrite(trainPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				else imwrite(testPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				flag = !flag;
				continue;
			}

			img = imread(oriPath + "testSet\\" + imgName + "G.jpg");
			if ( img.rows != 0)
			{
				//--- save in trainset
				if (flag)
					imwrite(trainPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				else imwrite(testPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				flag = !flag;
				continue;
			}

			img = imread(oriPath + "testSet\\" + imgName + "B.jpg");
			if ( img.rows != 0)
			{
				//--- save in trainset
				if (flag)
					imwrite(trainPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				else imwrite(testPath+dirNames[i]+"\\"+imgBG+imgName+".jpg", img);
				flag = !flag;
				continue;
			}
		}
	}
}

/*
	save aesthetic label info to ".aesthLabel" file
*/
void constructAesthLabel()
{
	const string path = "F:\\Lab\\Category_Ranking_AVA\\tmp\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);

	const string labelSuffix = ".aesthLabel";
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path + dirNames[i] + "\\";
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);

		for (int j = 0; j < imgNames.size(); j++)
		{
			ofstream fsLabel(currDir+imgNames[j]+labelSuffix);
			cout<<currDir+imgNames[j]<<endl;
			if (!fsLabel.is_open())
			{
				cout<<"cannot open file"<<currDir + imgNames[j]<<endl;
				return;
			}

			if (imgNames[j].find("G") != -1)
				fsLabel<<1<<endl;
			else fsLabel<<0<<endl;

			fsLabel.close();
		}
	}
}

/*
	construct "dcnn_feat_dat.dat" file of each category and save
	used for baseline and method_1 test
*/
void constructDcnnFeatDatFile()
{
	const string path = "F:\\Lab\\Category_Ranking_AVA\\tmp\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);

	const int featDim = 4096;
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path + dirNames[i] + "\\";
		cout<<currDir<<endl;

		ofstream fsDcnnFeat (currDir + "features_result\\dcnn_feat_dat.dat");
		if (!fsDcnnFeat.is_open())
		{
			cout<<"cannot open file: "<<currDir+"features_result\\dcnn_feat_dat.dat"<<endl;
			return;
		}

		vector<string> dcnnFiles = dir.GetListFiles(currDir,"*.feat",false);
		vector<float> dcnnFeat(4096, -1);
		for (int j = 0; j < dcnnFiles.size(); j++)
		{
			readBinaryFile(currDir+dcnnFiles[j], featDim, dcnnFeat);	

			if (dcnnFiles[j].find("G") != -1)
				fsDcnnFeat<<"2 qid:1 ";
			else fsDcnnFeat<<"1 qid:1 ";
			
			for (int k = 0; k < dcnnFeat.size(); k++)
			{
				fsDcnnFeat<<k+1<<":"<<dcnnFeat[k]<<" ";
			}
			fsDcnnFeat<<endl;
		}
		fsDcnnFeat.close();
	}
}

/*
	construct "ori_label.txt" to save label info or real set, rather than "init_label_index.txt"
*/
void constructOriLabel()
{
	const string path = "F:\\Lab\\Category_Ranking_AVA\\tmp\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);

	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path + dirNames[i] + "\\";
		cout<<currDir<<endl;

		ofstream fsOriLabel(currDir + "features_result\\ori_lable.txt");
		if (!fsOriLabel.is_open())
		{
			cout<<"cannot open file: "<<currDir+"features_result\\ori_lable.txt"<<endl;
			return;
		}
		
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		for (int j = 0; j < imgNames.size(); j++)
		{
			if (imgNames[j].find("G")!=-1)
				fsOriLabel<<1<<endl;
			else fsOriLabel<<0<<endl;
		}
		fsOriLabel.close();
	}
}

/*
	construct "img_label.txt" to save "imgName imgLabel" of real set, rather than "init_label_index.txt"
*/
void constructImgLabel()
{
	const string path = "F:\\Lab\\Category_Ranking_AVA\\tmp\\category_based_test\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);

	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path+dirNames[i]+"\\";
		cout<<currDir<<endl;

		ofstream fsImgLabel(currDir+"features_result\\img_label.txt");
		if (!fsImgLabel.is_open())
		{
			cout<<"cannot open file"<<currDir+"features_result\\img_label.txt"<<endl;
			return;
		}

		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		for (int j = 0; j < imgNames.size(); j++)
		{
			fsImgLabel<<imgNames[j]<<"\t";

			if (imgNames[j].find("G") != -1)
				fsImgLabel<<"1"<<endl;
			else fsImgLabel<<"0"<<endl;
		}
		fsImgLabel.close();
	}
}

/*
	construct "label_xml.xml" to save aesthetic labe info
*/
void constructLabelXML()
{
	const string path = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	Directory dir;
	vector<string> dirNames = dir.GetListFolders(path, "*", false);

	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = path + dirNames[i] + "\\";
		cout<<currDir<<endl;
		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);

		Mat labelMat = Mat::zeros(imgNames.size(), 1, CV_32FC1);
		for (int j = 0; j < imgNames.size(); j++)
		{
			float* dataF = labelMat.ptr<float>(j);
			if (imgNames[j].find("G") != -1)
				dataF[0] = 1;
			else dataF[0] = 0;
		}

		FileStorage fs(currDir+"features_result\\label_xml.xml", FileStorage::WRITE);
		fs<<"labelMat"<<labelMat;
		fs.release();
	}
}

/*
	construct .featIndex file to save the feature index of all trainset
	used for myRankSVM_Learn training process
*/
void constructFeatIndex()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string indexSuffix = ".featIndex";

	Directory dir;
	vector<string> dirNames = dir.GetListFolders(trainPath, "*", false);


	int index = 0;
	int count = 0;
	for (int i = 0; i < dirNames.size(); i++)
	{
		string currDir = trainPath + dirNames[i] + "\\";

		vector<string> imgNames = dir.GetListFiles(currDir, "*.jpg", false);
		count += imgNames.size();
		for (int j = 0; j < imgNames.size(); j++)
		{
			ofstream fsFeatIndex (currDir+imgNames[j]+indexSuffix);
			if (!fsFeatIndex.is_open())
			{
				cout<<"cannot open file: "<<currDir+imgNames[j]+indexSuffix<<endl;
				return;
			}
			cout<<currDir+imgNames[j]+indexSuffix<<endl;

			fsFeatIndex<<index;
			fsFeatIndex.close();
			index ++;
		}
	}
	cout<<"count: "<<count<<endl;
}
