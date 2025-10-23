#pragma once

#include "Engine/Graphics/Model/AnimationEval.hpp"


#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Engine::Component
{
	struct AnimatorComponent
	{
		int clipIndex = -1;
		float time = 0.f;
		float speed = 1.f;
		bool loop = true;

		std::vector<glm::mat4> palette;

		std::vector<Engine::Graphics::Model::Animation::TRS> localTRS;
	};

	inline bool SetClipByName
	(
		const Engine::Graphics::Model::ModelData& model,
		AnimatorComponent& a,
		std::string_view name,
		bool loop = true
	)
	{
		for (int i = 0; i < (int)model.clips.size(); ++i)
		{
			if (model.clips[i].name == name)
			{
				a.clipIndex = i;
				a.time = 0.f;
				a.loop = loop;
				return true;
			}
		}


		return false;
	}
}