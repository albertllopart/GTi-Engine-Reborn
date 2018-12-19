
#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleImporter.h"


ResourceMesh::ResourceMesh(uint uid) : Resource (uid, RESOURCE_MESH)
{
}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::LoadInMemory()
{
	mesh.CreateVBO();
	mesh.CreateVAO();
	mesh.CreateIBO();
	loaded++;
}

void ResourceMesh::UnLoadFromMemory()
{

}

//////////////////////STRUCT MESH //////////////////////////////

Mesh::Mesh()
{

}

void Mesh::CreateVBO()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size_of_VBO, vertex_info, GL_STATIC_DRAW);//SIZE OF VBO 0!!!!!!!!!!!
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::CreateIBO()
{
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_index, index, GL_STATIC_DRAW); //num index = 0!!!;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


