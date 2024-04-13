dt_lite_strftime(int argc, VALUE *argv, VALUE self)
{
    return date_strftime_internal(argc, argv, self,
				  "%Y-%m-%dT%H:%M:%S%:z", set_tmx);
}