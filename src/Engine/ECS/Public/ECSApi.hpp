#pragma once

#include "Engine/Window/Public/Window.h"
#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"
#include "Engine/WorldSystem/Private/Query/View.hpp"
#include "Engine/WorldSystem/Private/Query/Filter.hpp"

#include "Engine/ECS/Public/Types.hpp"

#include <functional>
#include <iostream>

namespace Engine::ECS::Core
{
	namespace Init
	{
		template<typename Component>
		bool ApplyDefferedInit(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Window::Window& window)
		{
			InitPhase phase = GetInitPhase<Component>();

			if (phase == InitPhase::deffered)
			{
				std::cout << "[Init] Deferred Init for: " << typeid(Component).name() << std::endl;
				auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw, Engine::WorldSystem::Query::All<Component>{});
				std::cout << " - Found " << ents.size() << " entities." << std::endl;

				for (auto e : ents)
				{
					auto& comp = ctx.ww.Get<Component>(e);
					InitSystem<Component>::Init(comp, e, ctx, window);
				}
			}

			return true;
		}

		template<typename ...Components>
		bool ApplyAllDefferedInit(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Window::Window& window)
		{
			if (!(ApplyDefferedInit<Components>(ctx, window),...))
			{
				std::cout << "[Deffered Initialization] failed to deffered init\n";
				return false;
			}

			std::cout << "[Deffered Initialization] deffered initialization completed successfully\n";
			return true;
		}
	}
}