static int pmcraid_expose_resource(u16 fw_version,
				   struct pmcraid_config_table_entry *cfgte)
{
	int retval = 0;

	if (cfgte->resource_type == RES_TYPE_VSET) {
		if (fw_version <= PMCRAID_FW_VERSION_1)
			retval = ((cfgte->unique_flags1 & 0x80) == 0);
		else
			retval = ((cfgte->unique_flags0 & 0x80) == 0 &&
				  (cfgte->unique_flags1 & 0x80) == 0);

	} else if (cfgte->resource_type == RES_TYPE_GSCSI)
		retval = (RES_BUS(cfgte->resource_address) !=
				PMCRAID_VIRTUAL_ENCL_BUS_ID);
	return retval;
}
