#include "BufferClearBehavior.h"

using namespace Core::Renderers;

BufferClearBehavior::~BufferClearBehavior() {}



ClearToBlack::~ClearToBlack() {}
void ClearToBlack::Clear()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

ClearDepthAndToBlack::~ClearDepthAndToBlack() {}
void ClearDepthAndToBlack::Clear()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

ClearDepth::~ClearDepth() {}
void ClearDepth::Clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}