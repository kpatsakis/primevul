gpgsm_reset (void *engine)
{
  engine_gpgsm_t gpgsm = engine;

  /* IF we have an active connection we must send a reset because we
     need to reset the list of signers.  Note that RESET does not
     reset OPTION commands. */
  return (gpgsm->assuan_ctx
          ? gpgsm_assuan_simple_command (gpgsm->assuan_ctx, "RESET",
                                         NULL, NULL)
          : 0);
}
