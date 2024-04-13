GC_API int GC_CALL GC_posix_memalign(void **memptr, size_t align, size_t lb)
{
  /* Check alignment properly.  */
  if (((align - 1) & align) != 0 || align < sizeof(void *)) {
#   ifdef MSWINCE
      return ERROR_INVALID_PARAMETER;
#   else
      return EINVAL;
#   endif
  }

  if ((*memptr = GC_memalign(align, lb)) == NULL) {
#   ifdef MSWINCE
      return ERROR_NOT_ENOUGH_MEMORY;
#   else
      return ENOMEM;
#   endif
  }
  return 0;
}
