#include "ScreenQuad.h"

namespace Core
{

	ScreenQuad::ScreenQuad()
	{
		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};

		VertexBuffer = 0;
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	}


	ScreenQuad::~ScreenQuad()
	{
		glDeleteBuffers(1, &VertexBuffer);
	}


	void ScreenQuad::Render()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glViewport(0, 0, Settings::Window::Width, Settings::Window::Height);
		glDisable(GL_DEPTH_TEST);

		// Draw quad
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
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
		Debug::GLError("ERROR: Failed to render screen quad.");
	}

}