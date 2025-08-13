#pragma once
#include <string>
//#define FOLDER_PATH

#ifdef FOLDER_PATH
std::string folderPrefix = "./../../";
#else
std::string folderPrefix = "./";
#endif