#include"head.h"

bool makedir(string dir)
{
	char cmd[500];

	for( int i = 0; i < dir.size(); i++)
	{
		cmd[i] = dir[i];
	}
	cmd[dir.size()] = '\0';
	mkdir(cmd);
	return true;
}

bool execCommand(string cmdStr)
{
	char cmd[500];

	for (int i = 0; i < cmdStr.size(); i++)
	{
		cmd[i] = cmdStr[i];
	}
	cmd[cmdStr.size()] = '\0';
	system(cmd);
	return true;
}