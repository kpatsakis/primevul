void Magick::Image::read(MagickCore::Image *image,
  MagickCore::ExceptionInfo *exceptionInfo)
{
  // Ensure that multiple image frames were not read.
  if (image != (MagickCore::Image *) NULL &&
      image->next != (MagickCore::Image *) NULL)
    {
      MagickCore::Image
        *next;

      // Destroy any extra image frames
      next=image->next;
      image->next=(MagickCore::Image *) NULL;
      next->previous=(MagickCore::Image *) NULL;
      DestroyImageList(next);
    }
  replaceImage(image);
  if (exceptionInfo->severity == MagickCore::UndefinedException &&
      image == (MagickCore::Image *) NULL)
    {
      (void) MagickCore::DestroyExceptionInfo(exceptionInfo);
      if (!quiet())
        throwExceptionExplicit(MagickCore::ImageWarning,
          "No image was loaded.");
    }
  ThrowImageException;
}