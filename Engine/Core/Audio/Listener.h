#pragma once

#include <Core.h>
#include <Components/Entity.h>

namespace Core
{
	namespace Audio
	{
		class AL
		{
		private:
			bool AudioEnabled;
			bool MusicEnabled;

			float Volume;
			float MusicVolume;
			float AmbientVolume;
			float SFXVolume;
			float VoiceVolume;

			ALCdevice *device;
			ALCcontext *ctx;
			std::shared_ptr<Core::Components::Entity> entity;
			glm::vec3 lastPosition;

		public:
			void Init();
			void Update();
			void Close();

			void SetCurrentListener(std::shared_ptr<Core::Components::Entity> e);

			void ToggleAudio();
			bool IsAudioEnabled() const { return AudioEnabled; }
			void ToggleMusic();
			bool IsMusicEnabled() const { return MusicEnabled; }

			void SetVolume(float value) { Volume = value; }
			void SetMusicVolume(float value) { MusicVolume = value; }
			void SetAmbientVolume(float value) { AmbientVolume = value; }
			void SetSFXVolume(float value) { SFXVolume = value; }
			void SetVoiceVolume(float value) { VoiceVolume = value; }

			float GetVolume() const { return (AudioEnabled ? Volume : 0.0f); }
			float GetMusicVolume() const { return (MusicEnabled ? MusicVolume : 0.0f); }
			float GetAmbientVolume() const { return AmbientVolume; }
			float GetSFXVolume() const { return SFXVolume; }
			float GetVoiceVolume() const { return VoiceVolume; }
		};
	}
}