png_set_asm_flags (png_structp png_ptr, png_uint_32 asm_flags)
{
/* Obsolete as of libpng-1.2.20 and will be removed from libpng-1.4.0 */
    if (png_ptr != NULL)
    png_ptr->asm_flags = 0;
    PNG_UNUSED(asm_flags) /* Quiet the compiler */
}
