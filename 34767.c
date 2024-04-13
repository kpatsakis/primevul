CatalogueRegisterLocalFpeFunctions (void)
{
    RegisterFPEFunctions(CatalogueNameCheck,
			 CatalogueInitFPE,
			 CatalogueFreeFPE,
			 CatalogueResetFPE,
			 CatalogueOpenFont,
			 CatalogueCloseFont,
			 CatalogueListFonts,
			 CatalogueStartListFontsWithInfo,
			 CatalogueListNextFontWithInfo,
			 NULL,
			 NULL,
			 NULL,
			 CatalogueStartListFontsAndAliases,
			 CatalogueListNextFontOrAlias,
			 FontFileEmptyBitmapSource);
}
