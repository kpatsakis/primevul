XvImageFormatValues * XvMCListSubpictureTypes (
  Display * dpy,
  XvPortID port,
  int surface_type_id,
  int *count_return
)
{
    XExtDisplayInfo *info = xvmc_find_display(dpy);
    xvmcListSubpictureTypesReply rep;
    xvmcListSubpictureTypesReq  *req;
    XvImageFormatValues *ret = NULL;


    *count_return = 0;

    XvMCCheckExtension (dpy, info, NULL);


    LockDisplay (dpy);
    XvMCGetReq (ListSubpictureTypes, req);
    req->port = port;
    req->surface_type_id = surface_type_id;
    if (!_XReply (dpy, (xReply *) &rep, 0, xFalse)) {
        UnlockDisplay (dpy);
        SyncHandle ();
        return NULL;
    }

    if(rep.num > 0) {
        if (rep.num < (INT_MAX / sizeof(XvImageFormatValues)))
            ret = Xmalloc(rep.num * sizeof(XvImageFormatValues));

        if(ret) {
            xvImageFormatInfo Info;
            int i;

            *count_return = rep.num;

            for(i = 0; i < rep.num; i++) {
              _XRead(dpy, (char*)(&Info), sz_xvImageFormatInfo);
              ret[i].id = Info.id;
              ret[i].type = Info.type;
              ret[i].byte_order = Info.byte_order;
              memcpy(&(ret[i].guid[0]), &(Info.guid[0]), 16);
              ret[i].bits_per_pixel = Info.bpp;
              ret[i].format = Info.format;
              ret[i].num_planes = Info.num_planes;
              ret[i].depth = Info.depth;
              ret[i].red_mask = Info.red_mask;
              ret[i].green_mask = Info.green_mask;
              ret[i].blue_mask = Info.blue_mask;
              ret[i].y_sample_bits = Info.y_sample_bits;
              ret[i].u_sample_bits = Info.u_sample_bits;
              ret[i].v_sample_bits = Info.v_sample_bits;
              ret[i].horz_y_period = Info.horz_y_period;
              ret[i].horz_u_period = Info.horz_u_period;
              ret[i].horz_v_period = Info.horz_v_period;
              ret[i].vert_y_period = Info.vert_y_period;
              ret[i].vert_u_period = Info.vert_u_period;
              ret[i].vert_v_period = Info.vert_v_period;
              memcpy(&(ret[i].component_order[0]), &(Info.comp_order[0]), 32);
              ret[i].scanline_order = Info.scanline_order;
            }
        } else
	   _XEatDataWords(dpy, rep.length);
    }

    UnlockDisplay (dpy);
    SyncHandle ();
    return ret;
}
