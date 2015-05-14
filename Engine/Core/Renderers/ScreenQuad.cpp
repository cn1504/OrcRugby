#include "ScreenQuad.h"
#include <Assets/Database.h>
#include <Assets/AssetDB.h>

using namespace Core::Renderers;

ScreenQuad::ScreenQuad()
{	
	/*
		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};
		unsigned char* data = new unsigned char[sizeof(g_quad_vertex_buffer_data)];
		memcpy(data, g_quad_vertex_buffer_data, sizeof(g_quad_vertex_buffer_data));

		Core::Database->Sql("INSERT INTO VertexBuffers(key, data) VALUES(?, ?)");
		Core::Database->Bind(1, "SQuad");
		Core::Database->Bind(2, Core::Assets::Blob(data, sizeof(g_quad_vertex_buffer_data)));
		Core::Database->ExecuteAndFree();
	*/
}


ScreenQuad::~ScreenQuad()
{
}


void ScreenQuad::Render(const glm::ivec2& windowSize)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, windowSize.x, windowSize.y);
	glDisable(GL_DEPTH_TEST);

	// Draw quad
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Core::AssetDB->GetVertexBuffer("SQuad")->GetID());
	glVertexAttribPointer(
		0,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	Debug->GLError("ERROR: Failed to render screen quad.");
}
