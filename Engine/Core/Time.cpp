#include "Time.h"

namespace Core
{
	namespace Time
	{
		float Scale;
		float Delta;
		float Elapsed;
		float RealtimeDelta;
		float RealtimeElapsed;
		float FPS;
		int DrawCallsPerFrame;

		namespace
		{
			std::chrono::high_resolution_clock::time_point appStart;
			std::chrono::high_resolution_clock::time_point frameStart;
			std::chrono::high_resolution_clock::time_point frameEnd;

			float elapsedSinceLastSecond;
			int framesSinceLastSecond;
			float frameOffset;
		}

		void Init()
		{
			Scale = 1.0f;
			Delta = 0.0f;
			Elapsed = 0.0f;
			RealtimeDelta = 0.0f;
			RealtimeElapsed = 0.0f;
			FPS = 0.0f;
			
			elapsedSinceLastSecond = 0.0f;
			framesSinceLastSecond = 0;
			frameOffset = 1.5;

			appStart = std::chrono::high_resolution_clock::now();
			frameStart = frameEnd = appStart;
		}

		void NextUpdate(bool HasFocus)
		{			
			// Sleep if necessary
			if (std::chrono::high_resolution_clock::now() < frameEnd) {
				std::this_thread::sleep_until(frameEnd);
			}

			// Get frame start time
			auto prevStart = frameStart;
			frameStart = std::chrono::high_resolution_clock::now();

			// Set public data
			RealtimeDelta = std::chrono::duration_cast<std::chrono::duration<float>>(frameStart - prevStart).count();
			RealtimeElapsed += RealtimeDelta;
			Delta = RealtimeDelta * Scale;
			Elapsed += Delta;
			DrawCallsPerFrame = 0;

			// Calculate end of frame
			std::chrono::duration<float> frameDuration(1.0f / ((HasFocus) ? Settings::Video::MaxForegroundFPS : Settings::Video::MaxBackgroundFPS));
			frameEnd = frameEnd + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(frameDuration);

			// Calculate Frames per second
			framesSinceLastSecond++;
			elapsedSinceLastSecond += RealtimeDelta;
			if (elapsedSinceLastSecond > 1.0f) {
				FPS = (float)framesSinceLastSecond;
				framesSinceLastSecond = 0;
				elapsedSinceLastSecond = 0.0f;

				// Synchronize the frame timer
				frameEnd = frameStart + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(frameDuration);
			}
		}
	}
}
