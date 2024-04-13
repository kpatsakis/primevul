  TT_MulFix14_arm( FT_Int32  a,
                   FT_Int    b )
  {
    FT_Int32  t, t2;


#if defined( __CC_ARM ) || defined( __ARMCC__ )

    __asm
    {
      smull t2, t,  b,  a           /* (lo=t2,hi=t) = a*b */
      mov   a,  t,  asr #31         /* a   = (hi >> 31) */
      add   a,  a,  #0x2000         /* a  += 0x2000 */
      adds  t2, t2, a               /* t2 += a */
      adc   t,  t,  #0              /* t  += carry */
      mov   a,  t2, lsr #14         /* a   = t2 >> 14 */
      orr   a,  a,  t,  lsl #18     /* a  |= t << 18 */
    }

#elif defined( __GNUC__ )

    __asm__ __volatile__ (
      "smull  %1, %2, %4, %3\n\t"       /* (lo=%1,hi=%2) = a*b */
      "mov    %0, %2, asr #31\n\t"      /* %0  = (hi >> 31) */
#if defined( __clang__ ) && defined( __thumb2__ )
      "add.w  %0, %0, #0x2000\n\t"      /* %0 += 0x2000 */
#else
      "add    %0, %0, #0x2000\n\t"      /* %0 += 0x2000 */
#endif
      "adds   %1, %1, %0\n\t"           /* %1 += %0 */
      "adc    %2, %2, #0\n\t"           /* %2 += carry */
      "mov    %0, %1, lsr #14\n\t"      /* %0  = %1 >> 16 */
      "orr    %0, %0, %2, lsl #18\n\t"  /* %0 |= %2 << 16 */
      : "=r"(a), "=&r"(t2), "=&r"(t)
      : "r"(a), "r"(b)
      : "cc" );

#endif

    return a;
  }
