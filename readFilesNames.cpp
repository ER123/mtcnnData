#include "readFilesNames.h"
#include <iostream>
bool readFilesNames(const std::string& foldname,std::vector<std::string>& filenames)
{
	intptr_t curfilehandle;
	struct _finddata_t currentfile;
	std::string realfileindex = foldname + "\\" + IMAGE_TYPE;
	//std::cout << "#####################" << realfileindex.c_str() <<std::endl;
	std::string realfilename;
	curfilehandle = _findfirst(realfileindex.c_str(), &currentfile); 
	if (-1 == curfilehandle)return false;
	realfilename =/* foldname + "\\" + */currentfile.name;
	filenames.push_back(realfilename);
	while (-1 != _findnext(curfilehandle, &currentfile))
	{
		realfilename = /*foldname + "\\" +*/ currentfile.name;
		filenames.push_back(realfilename);
	}
	_findclose(curfilehandle);
	//std::cout << "the curfilehandle is: " << curfilehandle << std::endl;
	return true;
}