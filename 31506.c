  bdf_get_property( char*        name,
                    bdf_font_t*  font )
  {
    hashnode  hn;
    size_t    propid;


    if ( name == 0 || *name == 0 )
      return 0;

    if ( ( hn = hash_lookup( name, &(font->proptbl) ) ) == 0 )
      return 0;

    propid = hn->data;
    if ( propid >= _num_bdf_properties )
      return font->user_props + ( propid - _num_bdf_properties );

    return (bdf_property_t*)_bdf_properties + propid;
  }
