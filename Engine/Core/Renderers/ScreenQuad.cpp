#include "ScreenQuad.h"
#include <Assets/Database.h>
#include <Assets/AssetDB.h>
#include "VertexArray.h"

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
	glViewport(0, 0, windowSize.x, windowSize.y);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	Debug->GLError("ERROR: Failed to render screen quad. Viewport and PolyMode");

	// Draw quad
	auto vao = Core::AssetDB->GetVertexArray("SQuad");
	vao->Bind();

	Debug->GLError("ERROR: Failed to render screen quad. AttribPointer");
	glDrawArrays(GL_TRIANGLES, 0, 6);
	vao->Unbind();
	Debug->GLError("ERROR: Failed to render screen quad. DrawArrays");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
