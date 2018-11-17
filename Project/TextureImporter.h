#ifndef __TextureImporter__
#define __TextureImporter__

#include "Importer.h"
#include "ComponentMaterial.h"
#include "Globals.h"

enum DX_Compression
{
	NONE,
	DXT1,
	DXT2,
	DXT3,
	DXT4,
	DXT5,
	DXT_NO_COMP,
	KEEP_DXTC_DATA,
	DXTC_DATA_FORMAT,
	DX3DC,
	RXGB,
	ATI1N
};

enum Wrap_Mode
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};

class TextureImporter : Importer
{
public:
	TextureImporter(importer_type type);
	~TextureImporter();

	void ShowTextureImportOptions();

	bool Import(const char* source_path, std::string output_file) const;
	uint Load(const char* file_name);
	void SetCompression();
	void SetWrapMode();
	//void SetFileChars(const char * file_dir, std::string file_name);


	//dropped texture
	std::string file_dir;
	std::string file_name;
	bool to_import = false;
	//
	uint last_tex_width = 0;
	uint last_tex_height = 0;

	DX_Compression dx_compression = DX_Compression::NONE;
	int compression = 0;

	bool mip_map = false;

	Wrap_Mode wrap = Wrap_Mode::REPEAT;
	int wrap_mode = 0;
};

#endif