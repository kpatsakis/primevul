static BROTLI_INLINE BROTLI_NO_ASAN void memmove16(
    uint8_t* dst, uint8_t* src) {
#if BROTLI_SAFE_MEMMOVE
  /* For x86 this compiles to the same binary as signle memcpy.
     On ARM memcpy is not inlined, so it works slower.
     This implementation makes decompression 1% slower than regular one,
     and 2% slower than NEON implementation.
   */
  uint32_t buffer[4];
  memcpy(buffer, src, 16);
  memcpy(dst, buffer, 16);
#elif defined(__ARM_NEON__)
  vst1q_u8(dst, vld1q_u8(src));
#else
  /* memcpy is unsafe for overlapping regions and ASAN detects this.
     But, because of optimizations, it works exactly as memmove:
     copies data to registers first, and then stores them to dst. */
  memcpy(dst, src, 16);
#endif
}
