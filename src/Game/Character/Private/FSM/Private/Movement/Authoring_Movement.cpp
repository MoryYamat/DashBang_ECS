//#include "Engine/FSM/Public/Core/Authoring.hpp"
//#include "Engine/FSM/Public/Core/Registry.hpp"
//
//#include <memory>
//
//namespace Game::Character::FSM::Movement
//{
//	using namespace Engine::FSM::Core;
//
//	void RegisterMovementAuthoring(FSMRegistry& reg)
//	{
//		AuthoringFSM def;
//		def.axis = "Movement";
//		def.states = { "Idle" , "Move" };
//		def.conds = { "CanAndWantMove", "ShouldStop" };
//		def.transitions =
//		{
//			{ "Idle", "Move", "CanAndWantMove", 100},
//			{"Move", "Idle", "ShouldStop", 100},
//		};
//		def.version = 1;
//
//		reg.add(std::move(def));
//	}
//
//}