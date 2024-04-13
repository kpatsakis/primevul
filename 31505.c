  bdf_get_font_property( bdf_font_t*  font,
                         const char*  name )
  {
    hashnode  hn;


    if ( font == 0 || font->props_size == 0 || name == 0 || *name == 0 )
      return 0;

    hn = hash_lookup( name, (hashtable *)font->internal );

    return hn ? ( font->props + hn->data ) : 0;
  }
