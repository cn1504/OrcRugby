#pragma once
#include <Core.h>

namespace Core
{
	namespace Renderers
	{
		class BufferClearBehavior
		{
		public:
			virtual ~BufferClearBehavior();
			virtual void Clear() = 0;
		};



		class ClearToBlack : public BufferClearBehavior
		{
		public:
			virtual ~ClearToBlack();
			virtual void Clear();
		};

		class ClearDepthAndToBlack : public BufferClearBehavior
		{
		public:
			virtual ~ClearDepthAndToBlack();
			virtual void Clear();
		};

		class ClearDepthAndToWhite : public BufferClearBehavior
		{
		public:
			virtual ~ClearDepthAndToWhite();
			virtual void Clear();
		};

		class ClearDepth : public BufferClearBehavior
		{
		public:
			virtual ~ClearDepth();
			virtual void Clear();
		};
	}
}