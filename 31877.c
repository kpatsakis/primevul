  tt_check_trickyness_family( FT_String*  name )
  {

#define TRICK_NAMES_MAX_CHARACTERS  19
#define TRICK_NAMES_COUNT            9

    static const char trick_names[TRICK_NAMES_COUNT]
                                 [TRICK_NAMES_MAX_CHARACTERS + 1] =
    {
      "DFKaiSho-SB",        /* dfkaisb.ttf */
      "DFKaiShu",
      "DFKai-SB",           /* kaiu.ttf */
      "HuaTianKaiTi?",      /* htkt2.ttf */
      "HuaTianSongTi?",     /* htst3.ttf */
      "Ming(for ISO10646)", /* hkscsiic.ttf & iicore.ttf */
      "MingLiU",            /* mingliu.ttf & mingliu.ttc */
      "PMingLiU",           /* mingliu.ttc */
      "MingLi43",           /* mingli.ttf */
    };

    int  nn;


    for ( nn = 0; nn < TRICK_NAMES_COUNT; nn++ )
      if ( ft_strstr( name, trick_names[nn] ) )
        return TRUE;

    return FALSE;
  }
