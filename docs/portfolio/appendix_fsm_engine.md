# Appendix: FSM-Engine

ここでは以前実装した FSM-Engine の設計について説明する．  

なお、この実装は現行実装には含まれていない．

## もくじ
- [全体構造](#全体構造)
- [Axis / Profile](#axis--profile)
- [現行実装との関係](#現行実装との関係)
- [課題](#課題)
- [設計背景](#設計背景)
- [設計思想](#設計思想)
- [付録](#付録)

## 全体構造

FSM-Engine は、状態遷移を Gameplay コードに直接埋め込むのではなく、  
Authoring 用の定義データから Runtime 用のデータ構造を構築し、  
ECS 上の Component / System と接続して実行する構造を目指した．

全体の流れは次のようになっている．

```txt
Authoring Definition
    ↓
Build
    ↓
Canonical ID
    ↓
Runtime FSM Data
    ↓
FSM Instance Component
    ↓
FSM System Tick
    ↓
State Change / Event
```

### 1. Authoring Definition

ここでは、軸、状態、条件、遷移、効果などを人間が扱いやすい形で定義する．  

- 文字列や論理名によって、Axis 定義・FSM 定義を行う

以下は、Movement Axis と Basic FSM を Authoring Definition として定義する例である．
```cpp
// 軸定義では、FSM 定義のために使うラベルをすべて宣言する
static void makeAxis_Movement(FSMRegistry& reg)
{
    // 軸定義
	AxisDTO ax;
	ax.axis = "Movement";
	ax.stateU = {"Idle", "Moving" };
	ax.condU = { "canMove", "shouldStop", "AlwaysTrue" };
	ax.slotU = { "Transition", "Stop" };
	ax.profileU = { "Default" };
	ax.condDefs = {
		{.cond = "canMove",    .kind = "CompareF", .field = "movementInputMag", .op = ">",  .th = "0.00" },
		{.cond = "shouldStop",    .kind = "CompareF", .field = "movementInputMag", .op = "<=",  .th = "0.00" },
	};
	ax.version = 1;
	reg.add(std::move(ax));
}

// FSM 定義は、軸定義 の部分集合として FSM を定義する
static void makeFSM_Movement(FSMRegistry& reg)
{
	FSMDTO f;
	f.axis = "Movement";
	f.fsm = "Basic";
	f.states = { "Idle", "Moving" };
	f.profiles = { "Default" };

	// Profile 定義 (同一 遷移構造 に対する 遷移条件 の差し替え)
	f.profile_defs.push_back(
        ProfileDefDTO{
		.name = "Default",
		.binds = {
			ProfileBindDTO{
			.slot = "Transition",
			.cond = "canMove",},
			ProfileBindDTO{
			.slot = "Stop",
			.cond = "shouldStop",}
            }
        });

	// 遷移定義
	f.transitions.push_back({ "Idle", "Moving", "Transition", 0 });
	f.transitions.push_back({ "Moving", "Idle", "Stop", 0 });
}
```

### 2. Build
Build では、Authoring Definition を Runtime で扱いやすい形に変換する．

この段階では、文字列や論理名を Canonical ID に変換し、  
状態集合、条件、遷移関係を Runtime 用の table / graph として構築する．

### 3. Canonical ID

Canonical ID は、Runtime で State、Condition、Profile、Axis  
などを高速かつ一貫して参照するための識別子である．

Authoring 時には人間が読みやすい名前を使い、  
Runtime では ID を使うことで、文字列比較や不安定な参照を避けることを目指した．

### 4. Runtime FSM Data

Runtime FSM Data は、Build によって構築された実行用データである．  

Runtime では、このデータを参照して、  
現在 State、入力条件、Profile、Axisに応じた遷移を解決する．

### 5. FSM Instance Component
FSM Instance Component は、ECS 上の Entity が現在どの State にいるかを保持する．

Runtime FSM Data が共有される定義データであるのに対して、  
FSM Instance Component は Entity ごとの状態を表す．

```cpp
struct MovementStateComp
{
	StateID curState =  kInvalidState;		// 軸の中でのID StateID.v
	StateID prevState = kInvalidState;	    

	ProfileID curProf = kInvalidProfile;	// 軸の中でのID Profile.v
	ProfileID prevProf = kInvalidProfile;   
};
```

### 6. FSM System Tick

FSM System Tick では、Entity が持つ FSM Instance と、  
Component から構成される runtime state をもとに条件を評価し、  
必要に応じて State を更新する．

State が変化した場合は、遷移時の effect / event を発行し、  
Gameplay Logic や Animation などの後続処理へ接続することを想定した．

## Axis / Profile

Axis は FSM が **何を制御するFSMか** のラベルである．  

例えば、 Movement Axis であれば、移動に関する状態遷移を扱う．  
Attack Axis であれば、攻撃に関する状態遷移を扱う．

Profile は **同じ 遷移構造上で、遷移条件の差し替えを行うための** ラベル である．

例えば、Skill FSM において、ほとんどのスキルは以下のような状態遷移を経ると仮定する．

```
予備動作(詠唱)->攻撃判定発生->硬直時間->最初へ戻る
```

このとき、スキル毎に異なるのは、予備動作や攻撃判定の時間などの**遷移条件**であると考えられる．

このような違いを Profile として扱うことで、同じ制御ドメインに対して共通ルールを適用しつつ、  
Actor ごとの構造的な違いを表現することを目指した．

- Axis      : 制御対象を表す
- Profile   : 同じ FSM 上 での遷移条件の差し替えを行う
- Parameter : speed や attack power などの値的差異を表す


## 現行実装との関係

FSM-Engine は現行実装に統合していない．  

FSM-Engine は現行実装における、State / Resolver 層に相当する役割を、  
より汎用的に扱うための設計として試作した．

現行実装では、 Actor の状態管理や入力要求を、  
`enum` や個別 System による比較的単純な実装で行っている．

一方で、 FSM-Engine は現行実装の外側にある別機能ではなく、  
State / Resolver 層をより体系化した場合の設計案として位置づけられる．

現時点では、FSM-Engine をそのまま統合すると  
API 境界や定義量の問題によって実装が複雑になる．

このため、現行実装では、簡易的な State / Resolver 構造にとどめている．


## 課題

FSM-Engineの実装の課題は以下のようなものがある．

### 1. API を整理し、ユーザビリティと保守性、可読性を向上させる

この FSM-Engine の実装は API 境界の設計が甘いため、ユーザ側で定義する必要のあるデータが多い．  

このため、可読性と保守性が低く、一貫性のある直感的な使用が困難になっている．

今後は、API境界の再設計を行い、ユーザビリティと保守性を向上させることを目指す．

### 2. イベント処理をECSへ寄せる

ECS と FSM-Engine の責務境界と連携をより明確に考える必要がある．  

FSM-Engine の実装時は、FSM-Engine 側に System (Effect / Event) 処理を引き込んでしまった．  
このため、ECS の構造的利点やキャッシュ局所性を有効活用することが難しくなっていた．

今後は、ECS に標準的な System 実装の構造に合わせていく方向で調整していく．

### 3. Editor / Tool 化や BehaviorTree などの既存手法との連携

Authoring -> ID 化 という構造を目指した理由は、  
Runtime の効率性だけでなく、Tool としての利便性を想定したからである．  

一方で、この FSM-Engine では、FSM 定義の外部 data 化 などは行っていない．

今後は、可視化や JSON ファイルなど外部ファイルとの連携などによって、  
Editor / Tool を検討したい．


## 設計背景
以降では、FSM-Engine を試作しようと思った背景を説明する．

FSM-Engine を試作した理由は、  
ECS 上の Component と Game Rule と その設計の関係を整理したいという問題意識があったからである．

ECS の Component 群は、位置、速度、入力、パラメータなど、Game Runtime における状態を保持する．  
これらの多くは連続値、または疑似的な連続値として扱われる．

一方で、Game Rule は、それらの値に対して境界を作ることで制御や判定を行う．  

例えば、サッカーにおけるゴール判定は、ボール位置という連続的な値に対して、  
ゴールラインを超えたかどうかという境界を設定することで成立する．  

このように考えると、Game Rule の設計は、runtime state に対して  
どのような境界を作り、どのような判定や状態遷移を発生させるかを定義することと関係していると思われる．

そこで、ECS 上の Component が持つ値に対して、条件、状態、遷移、遷移時の効果を  
できるだけ分離して扱う仕組みとして、FSM-Engine を試作した．

## 設計思想
FSM-Engine では、状態遷移を個別の`if 文`として Gameplay コードに埋め込むことを回避し、  
状態集合、入力、条件、遷移、遷移時の効果を分離して扱うことを目指した．

この考え方の基礎として、Deterministic Finite Automaton (DFA) の構造を参考にした．  

DFA は、状態集合、入力、遷移関数、開始状態、受理状態 によって定義される．  

この試作では、特に、状態集合、入力、遷移関数の分離に注目した．
これにより、設計者が **どの状態が存在するか** や、 **どの条件で遷移するか**   
を明示的に定義するための枠組みとして利用できると考えた．

ただし、Game Runtime では、次のことが追加で必要になると考えた．  
- Component の値を読む**条件関数**
- 遷移時に発生する **event / effect**
- Actor ごとの差異
- ECS との接続

そのため、FSM-Engine では以下を分離して扱うことを目指した．

- State: 状態
- Condition: 遷移条件 と その値
- Effect / Event: 遷移時／状態中／遷移後のイベント処理
- Axis: FSMが制御する対象の名前(例: Movement_State、Attack_Stateなど)(FSMの集合の集合)
- FSM: Axis の部分集合としてのFSMの名前・ラベル
- Profile: 同一遷移構造に対して条件を差し替えるための仕組み
- Instance: 実際に Actor などが Runtime で持つ現在状態


### 付録
過去のエンジン実装は、以下のURLから確認できます．  
https://github.com/MoryYamat/Dashbang_ECS_OLD