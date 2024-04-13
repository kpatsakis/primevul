LIBOPENMPT_MODPLUG_API unsigned int ModPlug_NumSamples(ModPlugFile* file)
{
	if(!file) return 0;
	return openmpt_module_get_num_samples(file->mod);
}
