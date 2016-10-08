#include "head.h"

/*
construct train and test set based on "init_label_index.txt" file
*/
void constructTrainTest();

/*
	save aesthetic label info to ".aesthLabel" file
*/
void constructAesthLabel();

/*
	construct "dcnn_feat_dat.dat" file of each category and save
	used for baseline and method_1 test
*/
void constructDcnnFeatDatFile();

/*
	construct "ori_label.txt" to save label info or real set, rather than "init_label_index.txt"
*/
void constructOriLabel();

/*
	construct "img_label.txt" to save "imgName imgLabel" of real set, rather than "init_label_index.txt"
*/
void constructImgLabel();

/*
	construct "label_xml.xml" to save aesthetic labe info
*/
void constructLabelXML();


/*
	construct .featIndex file to save the feature index of all trainset
	used for myRankSVM_Learn training process
*/
void constructFeatIndex();