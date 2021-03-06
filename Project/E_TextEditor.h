#ifndef __ENGINE_TEXT_EDITOR__
#define __ENGINE_TEXT_EDITOR__

#include "E_Windows.h"
#include "ImGuiColorTextEdit/TextEditor.h"


class E_TextEditor : public E_Windows
{
public:

	E_TextEditor(ModuleEngineWindows* my_editor);
	~E_TextEditor();

	bool Draw();
	void CleanUp();
	void SetEditFile(std::string code);

private:
	TextEditor text_editor;

public:
	bool code_edit = false;
	bool is_vertex_shader = false;
	std::string file_name;
};

#endif //__ENGINE_TEXT_EDITOR__