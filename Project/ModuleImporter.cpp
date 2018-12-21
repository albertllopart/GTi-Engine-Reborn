#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Assimp/include/cimport.h" 
#include "Assimp/include/scene.h" 
#include "Assimp/include/postprocess.h" 
#include "Assimp/include/cfileio.h"
#include "Glew/include/glew.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ImGui/imgui.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void AssimpCallback(const char* msg, char* userData)
{
	if (App != nullptr)
	{
		std::string log = msg;
		App->imgui->AddConsoleLog(log);
	}
}

ModuleImporter::ModuleImporter(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init(JSON_Object* data)
{
	importer = new MeshImporter(IMPORTER_MESH);
	shader_importer = new ShaderImporter(IMPORTER_SHADER);

	struct aiLogStream stream;
	stream.callback = AssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::CleanUp(JSON_Object* data)
{
	RELEASE(importer);

	aiDetachAllLogStreams();
	return true;
}

ComponentMesh* ModuleImporter::LoadMesh(const char* fullPath)
{
	ComponentMesh* mesh = new ComponentMesh;
	if (fullPath != nullptr)
	{
		std::string temp = CleanFileName(fullPath);
		importer->Load(temp.c_str(), mesh);
		mesh->source = temp;
	}
	return mesh;
}

void ModuleImporter::ShowImportMeshParameters()
{
	ImGui::SetNextWindowPos(ImVec2(SDL_GetWindowSurface(App->window->window)->w / 2.5f, 500), ImGuiCond_Always);
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowSize(ImVec2(300, 220), ImGuiCond_Always);
	
	ImGui::Begin("Mesh import parameters");
	ImGui::Text("Process Preset:");
	ImGui::SameLine();
	const char* preset[] = { "Left handed", "Target realtime fast", "Target realtime quality", "Target realtime max quality"};
	ImGui::Combo("", &aiPresset, preset, IM_ARRAYSIZE(preset));


	if (ImGui::Button("Import"))
	{
		mesh_import = true;
	}
	if (mesh_import)
	{
		ImportMesh(import_path.c_str());
		mesh_import = false;
		App->imgui->mesh_import = false;
	}
	ImGui::End();

}

bool ModuleImporter::ImportMesh(const char* fullPath)
{
	if (fullPath == nullptr)
		return false;

	const aiScene* scene = nullptr;
	switch (aiPresset)
	{
	case 0:
		scene = aiImportFile(fullPath, aiProcess_ConvertToLeftHanded);
		break;
	case 1:
		scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_Fast);
		break;
	case 2:
		scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_Quality);
		break;
	case 3:
		scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
		break;
	}

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* node = scene->mRootNode;

		if (App->editor->GetSelected() != nullptr && App->editor->GetSelected() != App->editor->GetRoot())
		{
			importer->ImportNodes(scene, node, App->editor->GetSelected(), nullptr);
		}
		else
		{
			GameObject* new_go = new GameObject();
			new_go->SetParent(App->editor->GetRoot());
			importer->ImportNodes(scene, node, new_go, nullptr);
			App->editor->SetSelected(new_go);
		}
		aiReleaseImport(scene);
		return true;
	}
	else
	{
		LOG("Error loading scene %s", fullPath);
		return false;
	}
}

GameObject * ModuleImporter::LoadGameObject(const char * fullPath)
{
	GameObject* newObject = new GameObject();

	uint size = strlen(fullPath);

	std::string temp = CleanFileName(fullPath);
	newObject->name.assign(temp);

	newObject->AddComponent(LoadMesh(fullPath));

	return newObject;
}

ComponentTransform * ModuleImporter::LoadTransform(aiNode * node)
{
	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translation);

	math::float3 pos(translation.x, translation.y, translation.z);
	math::float3 sca(scale.x, scale.y, scale.z);
	math::Quat rot(rotate.x, rotate.y, rotate.z, rotate.w);

	return new ComponentTransform(pos, sca, rot);
}

ComponentMaterial* ModuleImporter::LoadMaterial(aiMaterial* drop)
{
	ComponentMaterial* mat = new ComponentMaterial();

	if (drop != nullptr)
	{
		uint numTextures = drop->GetTextureCount(aiTextureType_DIFFUSE);
		aiString path;
		drop->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string fullPath = "Assets/";
		fullPath.append(path.C_Str());
		mat->SetID(App->textures->ImportImage(fullPath.c_str()));
		mat->SetTextureName(path.C_Str());

		return mat;
	}
	return nullptr;
}

std::string ModuleImporter::CleanFileName(const char* fullPath) const
{
	std::string file_name = fullPath;
	uint start = file_name.find_last_of("\\");
	uint end = file_name.find_last_of(".");
	uint len = end - start;
	file_name = file_name.substr(start + 1, len - 1);

	return file_name;
}





