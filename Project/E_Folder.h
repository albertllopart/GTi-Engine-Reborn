#ifndef ___E_FOLDER__
#define __ E_FOLDER__

#include "ModuleFileSystem.h"
#include "E_Windows.h"
#include <string>
#include <list>
#include <vector>

//enum FileType
//{
//	FileNone,
//	FileMesh,
//	FileTexture,
//	FileMeta,
//	FileJSON,
//	FileDirectory
//};


struct Path
{
	Path();
	Path(const std::string& path, const std::string& name, const std::string& parent_path, bool directory);
	~Path();
public:
	file_type SetType();
	void RemoveChild(Path* child);
	const std::string GetPath()const;
	void SetParentByPath(std::list<Path*>& paths);
	void SetChild(Path* child);
	void SetParent(Path* parent);
	void ToDelete();

public:
	bool to_delete = false;
	std::string path;
	std::string name;
	std::string parent_path;
	std::list<Path*> child;
	bool directory = true;
	bool operator ==(const Path& value);
	file_type type = FILE_NONE;
	Path* parent;
};

struct PathList
{
	void CleanUp();
	void OrderPath();
	bool PathExist(const std::string& cmp_path)const;
	void FillFiles(std::vector<std::string>& files, file_type especific);
	std::list<Path*>::const_iterator FindFolder(const std::string& show_folder_path)const;
	std::list<Path*> list;
private:

};

class E_Folder : public E_Windows
{
public:
	E_Folder(ModuleEngineWindows* my_editor);
	~E_Folder();

	void CleanUp();
	bool Draw();
	void DeleteFolders();
	bool ShowFolderEmpty() const;
	bool ItemSelectedEmpty() const;

	const char* GetFolderName()const;
	void SetUpFolders();
	void UpdateFiles();
	void FillFiles(std::vector<std::string>& files, file_type especific = FILE_NONE);
	void DrawFolders(Path* draw);
	void DrawFolderInfo();
	bool LoadWindow(char**buffer);

private:


	std::string show_folder;
	std::string item_selected;
	bool delete_folder = false;
	PathList path;
};

#endif //__E_FOLDER__