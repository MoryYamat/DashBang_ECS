#include "Engine/Audio/Internal/Backends/MiniAudioBackend.hpp"

#include "Engine/Audio/Internal/AudioLog.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <utility>
#include <miniaudio.h>
#include <memory>
#include <filesystem>

namespace Engine::Audio
{
	struct MiniAudioBackend::Impl
	{
		ma_engine engine{};
		bool initialized = false;
	};

	MiniAudioBackend::MiniAudioBackend()
		:impl_(std::make_unique<Impl>())
	{

	}

	MiniAudioBackend::~MiniAudioBackend()
	{
		shutdown();
	}

	MiniAudioBackend::MiniAudioBackend(MiniAudioBackend&& other) noexcept
		:impl_(std::move(other.impl_))
	{

	}

	MiniAudioBackend& MiniAudioBackend::operator=(MiniAudioBackend&& other) noexcept
	{
		if (this != &other)
		{
			shutdown();
			impl_.reset();
			impl_ = std::move(other.impl_);
			other.impl_.reset();
		}
		return *this;
	}

	bool MiniAudioBackend::initialize()
	{
		if (!impl_ || impl_->initialized)
		{
			return true;
		}

		const ma_result r = ma_engine_init(nullptr, &impl_->engine);
		if (r != MA_SUCCESS)
		{
			Log::error(Log::kBackend, "MiniAudio::initialize (failed to init MiniAudioEngine)");
			impl_->initialized = false;
			return false;
		}

		impl_->initialized = true;
		return true;
	}

	void MiniAudioBackend::shutdown()noexcept
	{
		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kBackend, "MiniAudio::shutdown (unexpected)");
			return;
		}

		ma_engine_uninit(&impl_->engine);
		impl_->initialized = false;
	}

	void MiniAudioBackend::play_one_shot(std::string_view path, float volume)
	{
		(void)volume; // 最小実装では無視。後でバス/ボイス管理で対応。
		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kBackend, "MiniAudio::play_one_shot (unexpected)");
			return;
		}

		std::string pathZ(path); // null終端が必要
		const ma_result r = ma_engine_play_sound(&impl_->engine, pathZ.c_str(), nullptr);
		if (r != MA_SUCCESS)
		{
			std::cerr << "play_sound failed: " << pathZ << "r=" << r << "\n";
		}

		// if (!impl_ || !impl_->initialized)
		// {
		// 	return;
		// }
		// 
		// // volume の最低限の安全化 (上位でもやるなら不要)
		// volume = std::clamp(volume, 0.0f, 10.0f);
		// 
		// // ma_engine_play_sound は音量指定ができないので、
		// // 一度 ma_sound を作って音量を設定してから再生する
		// ma_sound sound{};
		// const ma_uint32 flags = 0;
		// 
		// // path は null 終端が必要なので一度 std::string にする
		// std::string pathZ(path);
		// 
		// if (ma_sound_init_from_file(&impl_->engine, pathZ.c_str(), flags, nullptr, nullptr, &sound) != MA_SUCCESS)
		// {
		// 	return;
		// }
		// 
		// ma_sound_set_volume(&sound, volume);
		// ma_sound_start(&sound);


		// ワンショットなので fire-and-forget にしたいが、
		// ここで uninit すると音が止まる。
		// → 最小実装では「自動破棄管理」をまだ作っていないので、
		//    ma_sound を detatch してエンジンに管理させるやり方を取る。
		//
		// miniaudio の ma_engine は内部でサウンドを追跡しないため、
		// 本格的には「再生中サウンドのリスト」を backend が持ち、
		// 再生終了を検出して uninit する必要がある。
		//
		// まずは “動作確認” として、簡易に「エンジンで即時再生」したい場合は
		// ma_engine_play_sound を使ってしまうのが最短（音量指定不可）。
		//
		// ここでは「音量指定したい」を優先して、暫定策として
		// しばらく生存させる管理を後で追加する前提にする。

		// 暫定：このままだとリークします（後で必ず管理を足す必要あり）
		// なので、まずは音量指定不要なら ma_engine_play_sound を使ってください。
		// -----
		// ma_engine_play_sound(&impl_->engine, pathZ.c_str(), nullptr);
		// ma_sound_uninit(&sound);
	}

	void MiniAudioBackend::play_one_shot(const std::filesystem::path& path, float volume)
	{
		(void)volume; // 最小実装では無視。後でバス/ボイス管理で対応。
		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kBackend, "MiniAudio::play_one_shot (unexpected)");
			return;
		}

		const std::string pathZ = path.string(); // null終端が必要
		const ma_result r = ma_engine_play_sound(&impl_->engine, pathZ.c_str(), nullptr);
		if (r != MA_SUCCESS)
		{
			std::cerr << "play_sound failed: " << pathZ << "r=" << r << "\n";
		}
	}
}


