bool getFileMetadata(const String& path, FileMetadata& metadata)
{
    WIN32_FIND_DATAW findData;
    if (!getFindData(path, findData))
        return false;

    if (!getFileSizeFromFindData(findData, metadata.length))
        return false;

    time_t modificationTime;
    getFileModificationTimeFromFindData(findData, modificationTime);
    metadata.modificationTime = modificationTime;

    metadata.type = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? FileMetadata::TypeDirectory : FileMetadata::TypeFile;

    return true;
}
