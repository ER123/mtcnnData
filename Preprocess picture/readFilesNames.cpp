#include "readFilesNames.h"
#include <iostream>
using namespace std;

extern vector<string> picnames;
extern vector<string> picaddress;

bool readFilesNames(const std::string& basePath)
{
    DIR *dir;
    struct dirent *ptr;
    std::string base;
    int namelen = 0;

    if ((dir = opendir(basePath.c_str())) == NULL)
    {
        perror("Open dir error...");
        return false;
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8){namelen = strlen(ptr->d_name);if(ptr->d_name[namelen-4] == '.' && ( ptr->d_name[namelen-3] == 'j' || ptr->d_name[namelen-3] == 'J' ) && ( ptr->d_name[namelen-2] == 'p' || ptr->d_name[namelen-2] == 'P' ) && ( ptr->d_name[namelen-1] == 'g' || ptr->d_name[namelen-1] == 'G' ))    ///jpgfile
            {picnames.push_back(ptr->d_name);picaddress.push_back(basePath + "/" + ptr->d_name);cout << basePath + "/" + ptr->d_name<<endl;}}
        //else if(ptr->d_type == 10)    ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
        else if(ptr->d_type == 4)    ///dir
        {
            base.clear();
            base = basePath;
            base = base + "/";
            base = base + ptr->d_name;
            //cout<<base<<endl;//###
            readFilesNames(base);
        }
    }
    closedir(dir);
    return true;
}
