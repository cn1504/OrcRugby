#pragma once

#include <Core.h>
#include <Components/Entity.h>

namespace Core
{
	namespace Audio
	{
		class Track;

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
			std::vector<Track*> tracklist;

		public:
			void Init();
			void Update();
			void Close();

			void SetCurrentListener(std::shared_ptr<Core::Components::Entity> e);
			void PlayTrack(Track* track);

			void EnableAudio();
			void DisableAudio();
			void EnableMusic();
			void DisableMusic();

			void SetVolume(float value) { Volume = value; }
			void SetMusicVolume(float value) { MusicVolume = value; }
			void SetAmbientVolume(float value) { AmbientVolume = value; }
			void SetSFXVolume(float value) { SFXVolume = value; }
			void SetVoiceVolume(float value) { VoiceVolume = value; }

			float GetVolume() { return Volume; }
			float GetMusicVolume() { return (MusicEnabled ? MusicVolume : 0.0f); }
			float GetAmbientVolume() { return AmbientVolume; }
			float GetSFXVolume() { return SFXVolume; }
			float GetVoiceVolume() { return VoiceVolume; }
		};
	}
}