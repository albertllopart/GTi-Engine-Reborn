#include "Globals.h"
#include "Application.h"
#include "TextureImporter.h"
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib" )
#pragma comment (lib, "Devil/libx86/ILU.lib" ) 
#pragma comment (lib, "Devil/libx86/ILUT.lib" ) 

TextureImporter::TextureImporter(importer_type type) : Importer(type)
{

}

TextureImporter::~TextureImporter()
{

}

bool TextureImporter::Import(const char* source_path, std::string output_file)
{
	bool result = false;

	ILuint texture_name;
	ilGenImages(1, &texture_name);
	ilBindImage(texture_name);

	ILboolean success = ilLoadImage(source_path);

	if (success)
	{
		ilEnable(IL_FILE_OVERWRITE);

		ILuint size;
		ILubyte* data;

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0);// Get the size of the data buffer

		if (size > 0)
		{
			data = new ILubyte[size]; //allocate data buffer

			if (ilSaveL(IL_DDS, data, size) > 0)
			{
				result = App->filesystem->SaveFile(output_file, (char*)data, size, FILE_MATERIAL);
			}

			RELEASE_ARRAY(data);
		}
		else
		{
			App->imgui->AddConsoleLog(("Couldn't import texture: %s", source_path));
		}
	}
	return result;
}