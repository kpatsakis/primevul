snd_pcm_hw_param_value_max(const struct snd_pcm_hw_params *params,
			   snd_pcm_hw_param_t var, int *dir)
{
	if (hw_is_mask(var)) {
		if (dir)
			*dir = 0;
		return snd_mask_max(hw_param_mask_c(params, var));
	}
	if (hw_is_interval(var)) {
		const struct snd_interval *i = hw_param_interval_c(params, var);
		if (dir)
			*dir = - (int) i->openmax;
		return snd_interval_max(i);
	}
	return -EINVAL;
}