void PopulateDataInBuilder(BlobDataBuilder* builder,
                           size_t index,
                           base::TaskRunner* file_runner) {
  if (index % 2 != 0) {
    builder->PopulateFutureData(0, "abcde", 0, 5);
    if (index % 3 == 0) {
      builder->PopulateFutureData(1, "z", 0, 1);
    }
  } else if (index % 3 == 0) {
    scoped_refptr<ShareableFileReference> file_ref =
        ShareableFileReference::GetOrCreate(
            base::FilePath::FromUTF8Unsafe(
                base::SizeTToString(index + kTotalRawBlobs)),
            ShareableFileReference::DONT_DELETE_ON_FINAL_RELEASE, file_runner);
    builder->PopulateFutureFile(0, file_ref, base::Time::Max());
  }
}
