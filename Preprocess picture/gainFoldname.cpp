#include "gainFoldname.h"
//#include <iostream>
std::string gainFoldname()
{
	char* foldname;
	//std::string sfoldname;
	foldname = getcwd(NULL, 0);
	//sfoldname = foldname;
	//for (int i = 0; foldname[i]!=0; i++)std::cout << foldname[i];
	return foldname;
}
