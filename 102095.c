void MPEG4Extractor::parseID3v2MetaData(off64_t offset) {
    ID3 id3(mDataSource, true /* ignorev1 */, offset);

 if (id3.isValid()) {
 struct Map {
 int key;
 const char *tag1;
 const char *tag2;
 };
 static const Map kMap[] = {
 { kKeyAlbum, "TALB", "TAL" },
 { kKeyArtist, "TPE1", "TP1" },
 { kKeyAlbumArtist, "TPE2", "TP2" },
 { kKeyComposer, "TCOM", "TCM" },
 { kKeyGenre, "TCON", "TCO" },
 { kKeyTitle, "TIT2", "TT2" },
 { kKeyYear, "TYE", "TYER" },
 { kKeyAuthor, "TXT", "TEXT" },
 { kKeyCDTrackNumber, "TRK", "TRCK" },
 { kKeyDiscNumber, "TPA", "TPOS" },
 { kKeyCompilation, "TCP", "TCMP" },
 };
 static const size_t kNumMapEntries = sizeof(kMap) / sizeof(kMap[0]);

 for (size_t i = 0; i < kNumMapEntries; ++i) {
 if (!mFileMetaData->hasData(kMap[i].key)) {
                ID3::Iterator *it = new ID3::Iterator(id3, kMap[i].tag1);
 if (it->done()) {
 delete it;
                    it = new ID3::Iterator(id3, kMap[i].tag2);
 }

 if (it->done()) {
 delete it;
 continue;
 }

 String8 s;
                it->getString(&s);
 delete it;

                mFileMetaData->setCString(kMap[i].key, s);
 }
 }

 size_t dataSize;
 String8 mime;
 const void *data = id3.getAlbumArt(&dataSize, &mime);

 if (data) {
            mFileMetaData->setData(kKeyAlbumArt, MetaData::TYPE_NONE, data, dataSize);
            mFileMetaData->setCString(kKeyAlbumArtMIME, mime.string());
 }
 }
}
