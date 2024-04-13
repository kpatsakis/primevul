void error_exit(j_common_ptr cinfo)
{
    decoder_error_mgr *err = reinterpret_cast_ptr<decoder_error_mgr *>(cinfo->err);
    longjmp(err->setjmp_buffer, -1);
}
