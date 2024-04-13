void stream_int_report_error(struct stream_interface *si)
{
	if (!si->err_type)
		si->err_type = SI_ET_DATA_ERR;

	si->ob->flags |= CF_WRITE_ERROR;
	si->ib->flags |= CF_READ_ERROR;
}
