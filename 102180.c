int RemoveThumbnail(unsigned char * ExifSection)
{
 if (!DirWithThumbnailPtrs ||
 ImageInfo.ThumbnailOffset == 0 ||
 ImageInfo.ThumbnailSize == 0){
 return 0;
 }
 if (ImageInfo.ThumbnailAtEnd == FALSE){
 ErrNonfatal("Thumbnail is not at end of header, can't chop it off", 0, 0);
 return 0;
 }

 {
 int de;
 int NumDirEntries;
 NumDirEntries = Get16u(DirWithThumbnailPtrs);

 for (de=0;de<NumDirEntries;de++){
 int Tag;
 unsigned char * DirEntry;
 DirEntry = DIR_ENTRY_ADDR(DirWithThumbnailPtrs, de);
 Tag = Get16u(DirEntry);
 if (Tag == TAG_THUMBNAIL_LENGTH){
 if (Get16u(DirEntry+2) != FMT_ULONG){
 ErrNonfatal("Can't remove thumbnail", 0, 0);
 return 0;
 }
 Put32u(DirEntry+8, 0);
 }
 }
 }

 return ImageInfo.ThumbnailOffset+8;

}
