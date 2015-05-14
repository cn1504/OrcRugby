#pragma once
#include <Core.h>

namespace Core
{
	class Timer
	{
	private:
		std::chrono::high_resolution_clock::time_point appStart;
		std::chrono::high_resolution_clock::time_point frameStart;
		std::chrono::high_resolution_clock::time_point frameEnd;

		float elapsedSinceLastSecond;
		int framesSinceLastSecond;

		float scale;
		float delta;
		float elapsed;
		float realtimeDelta;
		float realtimeElapsed;
		float fps;
		int drawCallsPerFrame;

	public:
		const float& Scale;
		const float& Delta;
		const float& Elapsed;
		const float& RealtimeDelta;
		const float& RealtimeElapsed;
		const float& FPS;
		const int& DrawCallsPerFrame;


		Timer() : Scale(scale), Delta(delta), Elapsed(elapsed), RealtimeDelta(realtimeDelta), 
			RealtimeElapsed(realtimeElapsed), FPS(fps), DrawCallsPerFrame(drawCallsPerFrame)
		{
			scale = 1.0f;
			delta = 0.0f;
			elapsed = 0.0f;
			realtimeDelta = 0.0f;
			realtimeElapsed = 0.0f;
			fps = 0.0f;

			elapsedSinceLastSecond = 0.0f;
			framesSinceLastSecond = 0;


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
			realtimeDelta = std::chrono::duration_cast<std::chrono::duration<float>>(frameStart - prevStart).count();
			realtimeElapsed += RealtimeDelta;
			delta = RealtimeDelta * Scale;
			elapsed += Delta;
			drawCallsPerFrame = 0;

			// Calculate end of frame
			std::chrono::duration<float> frameDuration(1.0f / ((HasFocus) ? 60.0f : 60.0f));
			frameEnd = frameEnd + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(frameDuration);

			// Calculate Frames per second
			framesSinceLastSecond++;
			elapsedSinceLastSecond += RealtimeDelta;
			if (elapsedSinceLastSecond > 1.0f) {
				fps = (float)framesSinceLastSecond;
				framesSinceLastSecond = 0;
				elapsedSinceLastSecond = 0.0f;

				// Synchronize the frame timer
				frameEnd = frameStart + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(frameDuration);
			}
		}
	};
}
