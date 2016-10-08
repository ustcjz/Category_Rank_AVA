#include"Baseline_Regression.h"
#include"DCNN_Feat.h"
#include"basic_function.h"
#include<time.h>
#include"head.h"

//----- contruct train file .xml, normalized .xml and normalization params
void trainXMLConstruct()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const int featDim = 4096;	 

	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);
	
	for (int i = 0; i < cateNames.size(); i++)
	{
		vector<string> dcnnFiles = dir.GetListFiles(trainPath + cateNames[i], "*.feat", false);

		Mat featMat = Mat::zeros(dcnnFiles.size(), featDim, CV_32FC1);
		Mat maxMat = Mat::zeros(1, featDim, CV_32FC1); //--- save the minValue of each dim
		Mat minMat = Mat::zeros(1, featDim, CV_32FC1); //--- maxValue, used for normalization
		float* dataMax = maxMat.ptr<float>(0);
		float* dataMin = minMat.ptr<float>(0);

		string currCateName = trainPath + cateNames[i];
		//--- read each dcnn file and save it to mat
		for (int j = 0; j < dcnnFiles.size(); j++)
		{
			cout<<currCateName + "\\" + dcnnFiles[j]<<endl;

			float* dataFeat = featMat.ptr<float>(j); //--- pointer of featMat

			vector<float>dcnnFeat(featDim, -1);
			readBinaryFile(currCateName+"\\"+dcnnFiles[j], featDim, dcnnFeat);
			
			for (int k = 0; k < dcnnFeat.size(); k++)
			{
				//--- initialization of minMat maxMat
				if (j==0)
				{
					dataMin[k] = dcnnFeat[k];
					dataMax[k] = dcnnFeat[k];
				}
				else {
					dataMin[k] = dataMin[k] < dcnnFeat[k] ? dataMin[k] : dcnnFeat[k];
					dataMax[k] = dataMax[k] > dcnnFeat[k] ? dataMax[k] : dcnnFeat[k];
				}

				dataFeat[k] = dcnnFeat[k];
			}
		}

		string featXML = trainPath + cateNames[i] + "\\features_result\\dcnn_feature_xml.xml";
		FileStorage fs(featXML, FileStorage::WRITE);
		fs<<"feature"<<featMat;
		fs.release();

		string maxXML = trainPath + cateNames[i] + "\\features_result\\dcnn_feature_max_xml.xml";
		FileStorage fsMax(maxXML, FileStorage::WRITE);
		fsMax<<"max"<<maxMat;
		fsMax.release();

		string minXML = trainPath + cateNames[i] + "\\features_result\\dcnn_feature_min_xml.xml";
		FileStorage fsMin(minXML, FileStorage::WRITE);
		fsMin<<"min"<<minMat;
		fsMin.release();

		//----- normalization of feetMat and saved
		for (int j = 0; j < featMat.rows; j++)
		{
			float* dataFeat = featMat.ptr<float>(j);
			for (int k = 0; k < featMat.cols; k++)
			{
				dataFeat[k] = (dataFeat[k] - dataMin[k]) / (dataMax[k] - dataMin[k]);
			}
		}
		string normFeatXML = trainPath + cateNames[i] + "\\features_result\\dcnn_normalized_feature_xml.xml";
		FileStorage fsNorm(normFeatXML, FileStorage::WRITE);
		fsNorm<<"feature"<<featMat;
		fsNorm.release();
	}
}

//----- construct test file .xml, normalize it with params
void testXMLConstruct()
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";
	const int featDim = 4096;

	Directory dir;
	vector<string> cateNames = dir.GetListFolders(testPath, "*", false);

	for (int i = 0; i < cateNames.size(); i++)
	{
		vector<string> dcnnFiles = dir.GetListFiles(testPath+cateNames[i], "*.feat", false);

		//----- read maxMat and minMat of trainset, used for normalization
		Mat maxMat;
		string maxXML = trainPath + cateNames[i] + "\\features_result\\dcnn_feature_max_xml.xml";
		FileStorage fsMax(maxXML, FileStorage::READ);
		fsMax["max"]>>maxMat;
		fsMax.release();		
		float* dataMax = maxMat.ptr<float>(0);

		Mat minMat;
		string minXML = trainPath + cateNames[i] + "\\features_result\\dcnn_feature_min_xml.xml";
		FileStorage fsMin(minXML, FileStorage::READ);
		fsMin["min"]>>minMat;
		fsMin.release();
		float* dataMin = minMat.ptr<float>(0);


		Mat featMat = Mat::zeros(dcnnFiles.size(), featDim, CV_32FC1);
		
		//----- read all dcnnfiles and save to featMat
		vector<float> dcnnFeat(featDim, -1);
		string currCateName = testPath + cateNames[i];
		for (int j = 0; j < dcnnFiles.size(); j++)
		{
			cout<<currCateName + "\\"+ dcnnFiles[j]<<endl;

			readBinaryFile(currCateName+"\\"+dcnnFiles[j], featDim, dcnnFeat);
			float* dataFeat = featMat.ptr<float>(j);
			for (int k = 0; k < dcnnFeat.size(); k++)
				dataFeat[k] = dcnnFeat[k];
		}
		string featXML = testPath + cateNames[i] + "\\features_result\\dcnn_feature_xml.xml";
		FileStorage fsFeat(featXML, FileStorage::WRITE);
		fsFeat<<"feature"<<featMat;
		fsFeat.release();

		//----- normalization of featMat with min and max value
		for (int j = 0; j < featMat.rows; j++)
		{
			float* dataFeat = featMat.ptr<float>(j);

			for (int k = 0; k < featMat.cols; k++)
				dataFeat[k] = (dataFeat[k] - dataMin[k]) / (dataMax[k] - dataMin[k]);
		}
		string normFeatXMl = testPath + cateNames[i] + "\\features_result\\dcnn_normalized_feature_xml.xml";
		FileStorage fsNorm(normFeatXMl, FileStorage::WRITE);
		fsNorm<<"feature"<<featMat;
		fsNorm.release();
	}
}

//----- construct needed format feature file .xml
//----- label file .xml already exists
void datasetConstruct()
{

}


//----- train svm and predict , cross-validation using accuracy rather than AP value
void svmTrainPred() 
{
	const string trainPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_train\\";
	const string testPath = "F:\\Lab\\Category_Ranking_AVA\\category_based_test\\";

	Directory dir;
	vector<string> cateNames = dir.GetListFolders(trainPath, "*", false);
	
	//--- img count in trainSet
	int imgCount = 0;
	for (int i = 0; i < cateNames.size(); i++)
	{
		vector<string> imgNames = dir.GetListFiles(trainPath+cateNames[i], "*.jpg", false);
		imgCount += imgNames.size();
	}

	const int featDim = 4096;
	Mat trainMatAll = Mat::zeros(imgCount, featDim, CV_32FC1);
	Mat trainLabelAll = Mat::zeros(imgCount, 1, CV_32FC1);
	int rowIndexAll = 0;
	cout<<"load trainfile .xml ...";
	for (int i = 0; i < cateNames.size(); i++)
	{
		//--- load trainMat and trainLabel, added to trainMatAll
		Mat trainMat;
		string trainMatXML = trainPath + cateNames[i] + "\\features_result\\dcnn_normalized_feature_xml.xml";
		FileStorage fsTrainSet(trainMatXML, FileStorage::READ);
		fsTrainSet["feature"]>>trainMat;
		fsTrainSet.release();

		Mat trainLabel;
		string trainLabelXML = trainPath + cateNames[i] + "\\features_result\\label_xml.xml";
		FileStorage fsTrainLabel(trainLabelXML, FileStorage::READ);
		fsTrainLabel["labelMat"]>>trainLabel;
		fsTrainLabel.release();

		if ( trainMat.rows != trainLabel.rows)
		{
			cout<<"rows not match"<<endl;
			return;
		}

		//--- copy each row to trainMatAll
		for (int j = 0; j < trainMat.rows; j++)
		{
			trainMat.row(j).copyTo(trainMatAll.row(rowIndexAll));
			trainLabel.row(j).copyTo(trainLabelAll.row(rowIndexAll));
			rowIndexAll ++;
		}
	}
	cout<<"done"<<endl;
	//--- the params range for cross validation
	int cMin, cMax, cStep, cTemp;		
	int gMin, gMax, gStep, gTemp;
	int pMin, pMax, pStep, pTemp;
	int cvFolder;

	cMin = 2; cMax = 6; cStep = 2;
	gMin = -3; gMax = 1; gStep = 1;
	pMin = -1; pMax = 1; pStep = 1;
	cvFolder = 3;

	//--- cross validation for linear kernel
//	float cvParams = CrossValiParams::svmParams_Linear(trainMatAll, trainLabelAll, cMin, cMax, 
//		cStep, cvFolder);
//	cTemp = cvParams;

//		//--- cross validation for RBF kernel
//		vector<float> cvParams = CrossValiParams::svmParams_RBF(trainMat, trainLabel, cMin, cMax,
//			pMin, pMax, gMin, gMax, cStep, pStep, gStep, cvFolder);
//		cTemp = cvParams[0];
//		pTemp = cvParams[1];
//		gTemp = cvParams[2];
	CvSVMParams params;
	params.svm_type		= CvSVM::C_SVC;
	params.kernel_type	= CvSVM::LINEAR;
	params.C			= pow(2.0, cTemp);
//		params.p			= pow(2.0, pTemp);
//		params.gamma		= pow(2.0, gTemp);
	params.term_crit	= TermCriteria( CV_TERMCRIT_ITER, (int)1e4, 1e-6);
	
	clock_t st, end;
	st = clock();
	SvmSvrModel svmModel(trainMatAll, trainLabelAll, params);
	end = clock();
	cout<<"time consuming is "<<(double)(end-st)/CLOCKS_PER_SEC<<endl;

	for (int i = 0; i < cateNames.size(); i++)
	{
		bool flag = true; //--- svm prediction will be a float nor a bool if true

		string predFile = testPath + cateNames[i] + "\\features_result\\DCNN_Feat_Rank\\" 
			+ "baseline_regression_c.dat";

		//--- train and test
		Mat testMat;
		string testMatXML = testPath + cateNames[i] + "\\features_result\\dcnn_normalized_feature_xml.xml";
		FileStorage fsTestMat(testMatXML, FileStorage::READ);
		fsTestMat["feature"]>>testMat;
		fsTestMat.release();

		cout<<"test on"<<testMatXML<<endl;

		svmModel.predict(testMat, predFile, flag);
	}
}
