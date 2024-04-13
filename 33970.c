nsPluginInstance::WriteReady(NPStream* /* stream */ )
{
    if ( _streamfd != -1 ) {
	return 1024;
    } else {
	return 0;
    }
}
