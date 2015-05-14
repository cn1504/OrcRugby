#include "PostProcessingRenderer.h"

using namespace Core::Renderers;

PostProcessingRenderer::PostProcessingRenderer()
{
	CombineBuffers = std::make_unique<Shader>("fspassthrough.vert", "combinebuffers.frag");
	Blur = std::make_unique<Shader>("fspassthrough.vert", "blur.frag");
	NoAA = std::make_unique<Shader>("fspassthrough.vert", "noaa.frag");
	FXAA = std::make_unique<Shader>("fspassthrough.vert", "fxaa.frag");

	SQuad = std::make_unique<ScreenQuad>();
}

PostProcessingRenderer::~PostProcessingRenderer() {}

void PostProcessingRenderer::SetTextures(const Core::Assets::Texture& diffuse, const Core::Assets::Texture& specular)
{
	CombineBuffers->Activate();
	CombineBuffers->SetTexture("DiffuseLightTexture", diffuse, 0);
	CombineBuffers->SetTexture("SpecularLightTexture", specular, 1);
}

void PostProcessingRenderer::Draw(const glm::ivec2& bufferSize)
{	
	CombineBuffers->Activate();
	SQuad->Render(bufferSize);

	/* Render Bloom

	GlowMapHorizontalRB->MakeCurrent();
	GlowMapHorizontalRB->Clear();

	BlurShader->MakeCurrent();
	glUniform2f(BlurShader->GetUL("texelSize"), 1.0f / ((float)Settings::Window::Width), 1.0f / ((float)Settings::Window::Height));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(4));
	glUniform1i(BlurShader->GetUL("textureSample"), 0);

	glUniform1i(BlurShader->GetUL("blurAmount"), 32);
	glUniform1f(BlurShader->GetUL("blurScale"), 1.0f);
	glUniform1f(BlurShader->GetUL("blurStrength"), 0.2f);

	glUniform1i(BlurShader->GetUL("orientation"), 0);

	SQuad.Render();

	GlowMapVerticalRB->MakeCurrent();
	GlowMapVerticalRB->Clear();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GlowMapHorizontalRB->GetOutputTexture(0));
	glUniform1i(BlurShader->GetUL("textureSample"), 0);

	glUniform1i(BlurShader->GetUL("blurAmount"), 32);
	glUniform1f(BlurShader->GetUL("blurScale"), 1.0f);
	glUniform1f(BlurShader->GetUL("blurStrength"), 0.2f);

	glUniform1i(BlurShader->GetUL("orientation"), 1);

	SQuad.Render();


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Reset Blending
	*/

	/* Combine light and geometry

	BufferCombineRB->MakeCurrent();
	BufferCombineRB->Clear();
		
	BufferCombineShader->MakeCurrent();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(0));
	glUniform1i(BufferCombineShader->GetUL("BaseTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(2));
	glUniform1i(BufferCombineShader->GetUL("MSRTexture"), 1);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(3));
	//glUniform1i(BufferCombineShader->GetUL("SpecularHardnessTexture"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, LightRB->GetOutputTexture(0));
	glUniform1i(BufferCombineShader->GetUL("DiffuseLightTexture"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, LightRB->GetOutputTexture(1));
	glUniform1i(BufferCombineShader->GetUL("SpecularLightTexture"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, GlowMapVerticalRB->GetOutputTexture(0));
	glUniform1i(BufferCombineShader->GetUL("GlowTexture"), 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(4));
	glUniform1i(BufferCombineShader->GetUL("EmissiveTexture"), 6);
				
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, GeometryRB->GetOutputTexture(5));
	glUniform1i(BufferCombineShader->GetUL("SkyboxTexture"), 7);

	SQuad.Render();
	*/

	/* Render Post

	// Perform antialiasing pass(es)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (Settings::Video::FXAA > 0)
	{
	FXAAShader->MakeCurrent();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, BufferCombineRB->GetOutputTexture(0));
	glUniform1i(FXAAShader->GetUL("sourceTexture"), 0);
	glUniform2f(FXAAShader->GetUL("frameSize"), (GLfloat)Settings::Window::Width, (GLfloat)Settings::Window::Height);
	SQuad.Render();
	}
	else {
	NOAAShader->MakeCurrent();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, BufferCombineRB->GetOutputTexture(0));
	glUniform1i(NOAAShader->GetUL("sourceTexture"), 0);
	SQuad.Render();
	}

	glUseProgram(0);

	*/
}