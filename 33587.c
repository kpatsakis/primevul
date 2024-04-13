gpgsm_release (void *engine)
{
  engine_gpgsm_t gpgsm = engine;

  if (!gpgsm)
    return;

  gpgsm_cancel (engine);

  free (gpgsm->colon.attic.line);
  free (gpgsm);
}
