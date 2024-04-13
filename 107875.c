static qboolean S_AL_HearingThroughEntity( int entityNum )
{
	float	distanceSq;

	if( lastListenerNumber == entityNum )
	{
		distanceSq = DistanceSquared(
				entityList[ entityNum ].origin,
				lastListenerOrigin );

		if( distanceSq > THIRD_PERSON_THRESHOLD_SQ )
			return qfalse; //we're the player, but third person
		else
			return qtrue;  //we're the player
	}
	else
		return qfalse; //not the player
}
