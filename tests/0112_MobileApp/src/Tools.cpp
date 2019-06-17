#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <iostream>
#include <string.h>
#include "Tools.h"

std::string gTranslationDir(std::string aExePath)
{
    std::string lResult, lTransSubDir("/Translations");
    // We check exe path dir
    struct stat lInfo;
    if(stat(aExePath.c_str(), &lInfo) != 0)
        std::cerr << tr("stat() failed for path: ") << aExePath << tr(" error: ") << strerror(errno);
    else if(S_ISREG(lInfo.st_mode) && (lInfo.st_mode & S_IXUSR))
    {
        lResult = aExePath.substr(0, aExePath.find_last_of('/') + 1) + lTransSubDir;
        if(stat(lResult.c_str(), &lInfo) != 0)
            std::cerr << tr("stat() failed for path: ") << lResult << tr(" error: ") << strerror(errno);
        else if(S_ISDIR(lInfo.st_mode))
            return lResult;
    }
    else if(S_ISDIR(lInfo.st_mode))
    {
        lResult = aExePath + lTransSubDir;
        if(stat(lResult.c_str(), &lInfo) != 0)
            std::cerr << tr("stat() failed for path: ") << lResult << tr(" error: ") << strerror(errno);
        else if(S_ISDIR(lInfo.st_mode))
            return lResult;
    }

    // We check current working directory
    char lCwd[PATH_MAX];
    getcwd(lCwd, PATH_MAX);
    lResult = lCwd;
    lResult += lTransSubDir;
    if(stat(lResult.c_str(), &lInfo) != 0)
        std::cerr << tr("stat() failed for path: ") << lResult << tr(" error: ") << strerror(errno);
    else if(S_ISDIR(lInfo.st_mode))
        return lResult;

    // We check /usr/local/share/PROGRAM_NAME/Translations
    lResult = "/usr/local/share/";
    lResult += aExePath.substr(aExePath.find_last_of('/') + 1) + lTransSubDir;
    if(stat(lResult.c_str(), &lInfo) != 0)
        std::cerr << tr("stat() failed for path: ") << lResult << tr(" error: ") << strerror(errno);
    else if(S_ISDIR(lInfo.st_mode))
        return lResult;
    return "";
}

std::string gGetProgramName(std::string aExePath)
{
    return aExePath.substr(aExePath.find_last_of('/') + 1);
}
