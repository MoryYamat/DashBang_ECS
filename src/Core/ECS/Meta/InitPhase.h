// コンポーネントの初期化フェーズを指定するためのテンプレート群
// Templates for specifying the initialization phase of a component

#pragma once


enum class InitPhase
{
	Immediate,
	Deferred
};

template<typename Component>
constexpr InitPhase GetInitPhase()
{
	return InitPhase::Immediate;
}