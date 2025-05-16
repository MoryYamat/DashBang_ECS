//// Switch mapping depending on the state (title screen/in game)
//// ��ԁi�^�C�g����ʁ^�Q�[�����j�ɉ����ă}�b�s���O��ؑ�	
//
//#pragma once
//
//#include "Game/Input/InputAction.h"
//#include "Game/Input/InputMapping.h"
//
//#include <unordered_map>
//
//enum class InputContext
//{
//	Title,
//	Gameplay,
//	Pause,
//	UI,
//};
//
//class InputContextManager
//{
//public:
//	void setContext(InputContext ctx)
//	{
//		currentCTX = ctx;
//	}
//
//	InputContext getContext() const
//	{
//		return currentCTX;
//	}
//
//	void bindContext(InputAction ctx, InputMapping& mapping)
//	{
//		contextMappings[ctx] = mapping;
//	}
//
//	const InputMapping& getCurrentMapping() const
//	{
//		return contextMappings.at(currentCTX);
//	}
//
//private:
//	InputContext currentCTX = InputContext::Gameplay;
//
//	std::unordered_map<InputContext, InputMapping> contextMappings;
//};