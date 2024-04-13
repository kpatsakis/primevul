void gf_sm_load_done(GF_SceneLoader *load)
{
	if (load->done) load->done(load);
}
