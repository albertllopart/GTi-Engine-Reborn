#ifndef __ENGINE_TEXT_EDITOR__
#define __ENGINE_TEXT_EDITOR__

#include "E_Windows.h"
#include "ImGuiColorTextEdit/TextEditor.h"
#include "Glew/include/glew.h"

class E_TextEditor : public E_Windows
{
public:

	E_TextEditor(ModuleEngineWindows* my_editor);
	~E_TextEditor();

	bool Draw();
	void CleanUp();
	void SetEditFile(GLchar* code);

private:
	TextEditor text_editor;

public:
	bool code_edit = false;
};

#endif __ENGINE_TEXT_EDITOR__