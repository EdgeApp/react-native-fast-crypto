/*
 * Copyright (c) 2015, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "FileIO.hpp"
#include "Debug.hpp"
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mutex>

namespace abcd {

std::string
fileSlashify(const std::string &path)
{
    return path.back() == '/' ? path : path + '/';
}

bool
fileIsJson(const std::string &name)
{
    // Skip hidden files:
    if ('.' == name[0])
        return false;

    return 5 <= name.size() && std::equal(name.end() - 5, name.end(), ".json");
}

Status
fileEnsureDir(const std::string &dir)
{
    if (!fileExists(dir))
    {
        mode_t process_mask = umask(0);
        int e = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
        umask(process_mask);

        if (e)
            return ABC_ERROR(ABC_CC_DirReadError, "Could not create directory");
    }

    return Status();
}

bool
fileExists(const std::string &path)
{
    return 0 == access(path.c_str(), F_OK);
}

Status
fileLoad(DataChunk &result, const std::string &path)
{
    FILE *fp = fopen(path.c_str(), "rb");
    if (!fp)
        return ABC_ERROR(ABC_CC_FileOpenError,
                         "Cannot open " + path + " for reading");

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    result.resize(size);
    if (fread(result.data(), 1, size, fp) != size)
    {
        fclose(fp);
        return ABC_ERROR(ABC_CC_FileReadError,
                         "Cannot read " + path);
    }

    fclose(fp);
    return Status();
}

Status
fileSave(DataSlice data, const std::string &path)
{
    ABC_DebugLog("Writing file %s", path.c_str());

    const auto pathTmp = path + ".tmp";
    FILE *fp = fopen(pathTmp.c_str(), "wb");
    if (!fp)
        return ABC_ERROR(ABC_CC_FileOpenError,
                         "Cannot open " + pathTmp + " for writing");

    if (1 != fwrite(data.data(), data.size(), 1, fp))
    {
        fclose(fp);
        return ABC_ERROR(ABC_CC_FileWriteError, "Cannot write " + pathTmp);
    }
    fclose(fp);

    if (rename(pathTmp.c_str(), path.c_str()))
        return ABC_ERROR(ABC_CC_FileWriteError,
                         "Cannot rename " + pathTmp + " to " + path);

    return Status();
}

static Status
fileDeleteRecursive(const std::string &path)
{
    // It would be better if we could use the POSIX `nftw` function here,
    // but that is not available on all platforms we support.

    // First, be sure the file exists:
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf))
        return Status();

    // If this is a directory, delete the contents:
    if (S_ISDIR(statbuf.st_mode))
    {
        DIR *dir = opendir(path.c_str());
        if (!dir)
            return ABC_ERROR(ABC_CC_SysError, "Cannot open directory for deletion");

        struct dirent *de;
        while (nullptr != (de = readdir(dir)))
        {
            // These two are not real entries:
            if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
                continue;

            Status s = fileDeleteRecursive(fileSlashify(path) + de->d_name);
            if (!s)
            {
                closedir(dir);
                return s.at(ABC_HERE());
            }
        }
        closedir(dir);
    }

    // Actually remove the thing:
    if (remove(path.c_str()))
        return ABC_ERROR(ABC_CC_SysError, "Cannot delete file");

    return Status();
}

Status
fileDelete(const std::string &path)
{
    ABC_DebugLog("Deleting %s", path.c_str());
    return fileDeleteRecursive(path);
}

Status
fileTime(time_t &result, const std::string &path)
{
    struct stat statInfo;
    if (0 != stat(path.c_str(), &statInfo))
        return ABC_ERROR(ABC_CC_Error, "Could not stat file " + path);

    result = statInfo.st_mtime;
    return Status();
}

} // namespace abcd

