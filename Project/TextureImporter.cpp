#include "Globals.h"
#include "Application.h"
#include "TextureImporter.h"
#include "Glew/include/glew.h"
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"
#include "ImGui/imgui.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib" )
#pragma comment (lib, "Devil/libx86/ILU.lib" ) 
#pragma comment (lib, "Devil/libx86/ILUT.lib" ) 

TextureImporter::TextureImporter(importer_type type) : Importer(type)
{

}

TextureImporter::~TextureImporter()
{

}

bool TextureImporter::Import(const char* source_path, std::string output_file) const
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

		switch (dx_compression)
		{
			case NONE: ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); break;
			case DXT1: ilSetInteger(IL_DXTC_FORMAT, IL_DXT1); break;
			case DXT2: ilSetInteger(IL_DXTC_FORMAT, IL_DXT2); break;
			case DXT3: ilSetInteger(IL_DXTC_FORMAT, IL_DXT3); break;
			case DXT4: ilSetInteger(IL_DXTC_FORMAT, IL_DXT4); break;
			case DXT5: ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); break;
			case DXT_NO_COMP: ilSetInteger(IL_DXTC_FORMAT, IL_DXT_NO_COMP); break;
			case KEEP_DXTC_DATA: ilSetInteger(IL_DXTC_FORMAT, IL_KEEP_DXTC_DATA); break;
			case DXTC_DATA_FORMAT: ilSetInteger(IL_DXTC_FORMAT, IL_DXTC_DATA_FORMAT); break;
			case DX3DC: ilSetInteger(IL_DXTC_FORMAT, IL_3DC); break;
			case RXGB: ilSetInteger(IL_DXTC_FORMAT, IL_RXGB); break;
			case ATI1N: ilSetInteger(IL_DXTC_FORMAT, IL_ATI1N); break;
			default: ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); 
				App->imgui->AddConsoleLog("Setting default compression"); break;
		}
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

uint TextureImporter::Load(const char* file_name)
{
	std::string path = "Library/Materials/";
	path += file_name;
	path += ".dds";


	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(path.c_str()); 	// Load the image file

									// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		last_tex_width = ImageInfo.Width;
		last_tex_height = ImageInfo.Height;

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			LOG("Image conversion failed - IL reports error: %s ", iluErrorString(error));
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		switch (wrap)
		{
			case REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case MIRRORED_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			case CLAMP_TO_EDGE:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case CLAMP_TO_BORDER:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				break;
			default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself

	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		LOG("Image load failed - IL reports error: %s", iluErrorString(error));
		return 0;
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	LOG("Texture creation successful.");

	return textureID; // Return the GLuint to the texture so you can use it!
}

void TextureImporter::SetCompression()
{
	switch (compression)
	{
		case 0:	
			dx_compression = DXT1;
			break;
		case 1:
			dx_compression = DXT2; 
			break;
		case 2:
			dx_compression = DXT3; 
			break;
		case 3:
			dx_compression = DXT4; 
			break;
		case 4:
			dx_compression = DXT5; 
			break;
		case 5:
			dx_compression = DXT_NO_COMP;
			break;
		case 6:
			dx_compression = KEEP_DXTC_DATA; 
			break;
		case 7:
			dx_compression = DXTC_DATA_FORMAT; 
			break;
		case 8:
			dx_compression = DX3DC; 
			break;
		case 9:
			dx_compression = RXGB;
			break;
		case 10: 
			dx_compression = ATI1N; 
			break;
		default: 
			dx_compression = DXT_NO_COMP;
			App->imgui->AddConsoleLog("No compression selected");
	}
}

void TextureImporter::SetWrapMode()
{
	switch (wrap_mode)
	{
	case 0:
		wrap = REPEAT;
		break;
	case 1:
		wrap = MIRRORED_REPEAT;
		break;
	case 2:
		wrap = CLAMP_TO_EDGE;
		break;
	case 3:
		wrap = CLAMP_TO_BORDER;
		break;
	default:
		wrap = REPEAT;
		App->imgui->AddConsoleLog("Selected GL_REPEAT as default wrap mode");
	}
}

//void TextureImporter::SetFileChars(const char * file_dir, std::string file_name)
//{
//	file_dir = file_dir;
//	file_name = file_name.c_str();
//}

void TextureImporter::ShowTextureImportOptions()
{
	ImGui::SetNextWindowPos(ImVec2(SDL_GetWindowSurface(App->window->window)->w / 2.5f, 160), ImGuiCond_Always);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowSize(ImVec2(280, 220), ImGuiCond_Always);

	ImGui::Begin("Texture import parameters",NULL,flags);
	if (ImGui::CollapsingHeader("Import Settings"))
	{
		ImGui::Text("Compression:");
		ImGui::SameLine();
		const char* compresion_type[] = { "DXT1", "DXT2", "DXT3", "DXT4", "DXT5", "DXT_NO_COMP", "KEEP_DXTC_DATA", "DXTC_DATA_FORMAT", "THREE_DC", "RXGB", "ATI1N" };
		if (ImGui::Combo("", &App->textures->importer->compression, compresion_type, IM_ARRAYSIZE(compresion_type)))
		{
			App->textures->importer->SetCompression();
		}

		//ImGui::Checkbox("MipMapping", &mip_map); not working

		ImGui::Text("Wrap mode:");
		ImGui::SameLine();
		const char* wrap_type[] = { "REPEAT", "MIRRORED_REPEAT", "CLAMP_TO_EDGE", "CLAMP_TO_BORDER" };
		if (ImGui::Combo(" ", &App->textures->importer->wrap_mode, wrap_type, IM_ARRAYSIZE(wrap_type)))
		{
			App->textures->importer->SetWrapMode();
		}
	}
	if (ImGui::Button("Import"))
	{
		to_import = true;
	}
	if (to_import)
	{
		Import(file_dir.c_str(), file_name);
		if (App->editor->GetSelected() != nullptr)
		{
			ComponentMaterial* new_mat = App->editor->LoadComponentMaterial(file_name.c_str());
			App->editor->GetSelected()->AddComponent(new_mat);
			if (new_mat->GetMyGo()->GetParent()->childs.size() > 1)
			{

			}
		}
		to_import = false;
		App->imgui->text_import = false;
	}
	ImGui::End();
}