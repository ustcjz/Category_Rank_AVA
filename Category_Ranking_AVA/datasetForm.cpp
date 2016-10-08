#include"head.h"

bool datasetForm()
{

	// TODO get the category names
	string cateBase = "F:\\Lab\\Category_Ranking_AVA\\";
	Directory dir;
	vector<string> cateNames = dir.GetListFolders( cateBase, "*", false);

	string srcPath = "F:\\Lab\\Aesthetic_Quality\\AVA_dataset\\ImageSet\\";
	// TODO Initial ImgCategory,  and fromSrcToCate()
	for (int i = 0; i < cateNames.size(); i++)
	{
		string catePath = cateBase + cateNames[i] + "\\";

		ImgCategory imgCategory(catePath, srcPath);
		if( !imgCategory.fromSrcToCate())
			return false;
	}
	return true;
}