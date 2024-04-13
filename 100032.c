GF_Err gf_sm_load_suspend(GF_SceneLoader *load, Bool suspend)
{
	if (load->suspend) return load->suspend(load, suspend);
	return GF_OK;
}
