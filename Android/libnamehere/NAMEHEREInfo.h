#ifndef NAMEHEREINFO_H
#define NAMEHEREINFO_H

#include <iostream>

#include "rbx/Debug.h"

class NAMEHEREInfo
{
public:

    static NAMEHEREInfo& getNAMEHEREInfo()
    {
        static NAMEHEREInfo namehereInfo;
        return namehereInfo;
    }

    std::string getAssetFolderPath()
    {
        if(assetFolderPath.empty())
            RBXASSERT("Asset Path is not Set");

        return assetFolderPath;
    }

    void setAssetFolderPath(std::string assetPath)
    {
        assetFolderPath = assetPath;
    }

private:
    NAMEHEREInfo() {};
    NAMEHEREInfo(NAMEHEREInfo const&);
    void operator=(NAMEHEREInfo const&);
    std::string assetFolderPath;

};

#endif // NAMEHEREINFO_H
