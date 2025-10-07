```mathematica
Game/
  Character/
    Animation/
      Common/
        LastAppliedAnimation.hpp
      Query/
        AnimationQueryComponent.hpp
        BuildAnimationQuerySystem.hpp
        BuildAnimationQuerySystem.cpp
      Resolve/
        MovementAnimDecisionComponent.hpp
        MovementAnimationResolverSystem.hpp
        MovementAnimationResolverSystem.cpp
        // （将来）SkillAnimDecisionComponent.hpp, SkillAnimationResolverSystem.cpp
        // （将来）CCAnimDecisionComponent.hpp,    CCAnimationResolverSystem.cpp
      Arbiter/
        FinalAnimationDecisionComponent.hpp
        AnimationArbiterSystem.hpp
        AnimationArbiterSystem.cpp
      Apply/
        ApplyFinalAnimationDecisionSystem.hpp
        ApplyFinalAnimationDecisionSystem.cpp
      Profile/          // ←“データ駆動”を入れるときに追加
        AnimationProfileComponent.hpp
        AnimationProfileDatabase.hpp
```