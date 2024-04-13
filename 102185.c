INLINE UWORD32 impeg2d_bit_stream_nxt( stream_t *ps_stream, WORD32 i4_no_of_bits)
{
    UWORD32     u4_bits,u4_offset,u4_temp;
    UWORD8      u4_bit_ptr;

    ASSERT(i4_no_of_bits > 0);

    u4_offset               = ps_stream->u4_offset;
    u4_bit_ptr              = u4_offset & 0x1F;
    u4_bits                 = ps_stream->u4_buf << u4_bit_ptr;

    u4_bit_ptr              += i4_no_of_bits;
 if(32 < u4_bit_ptr)
 {
 /*  Read bits from the next word if necessary */
        u4_temp             = ps_stream->u4_buf_nxt;
        u4_bit_ptr          &= (BITS_IN_INT - 1);

        u4_temp             = (u4_temp >> (BITS_IN_INT - u4_bit_ptr));

 /* u4_temp consists of bits,if any that had to be read from the next word
           of the buffer.The bits read from both the words are concatenated and
           moved to the least significant positions of 'u4_bits'*/
        u4_bits = (u4_bits >> (32 - i4_no_of_bits)) | u4_temp;
 }
 else
 {
        u4_bits = (u4_bits >> (32 - i4_no_of_bits));
 }

 return (u4_bits);
}
