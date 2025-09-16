# memo

## `HIT EVENT`

## 目的
1. `SkillHitBox`と`CharacterHitBox`の衝突検知後
`HitEvent`によって`SkillDef`から`CC有無`を抽出し`CCFSMReqComp`へ`None->CC`遷移リクエストを追加する

2.`HitEvent`の詳細設計



<details>
<summary id ="prototype"><strong> 初期設計 </strong> </summary>

## シナリオ
0. `Collision Detection`: {`Skill Entity ID `, `Character Entity ID`, `time`, `Collision digestion complete flag`}
1. `Collision Detection` -> `Hit Event`: {`Hit Skill Entity = skill entity id`, `Hit Character Entity = Character Entity ID`, }
2. `Hit Event` -> `Collision Detection`: {`Collision digestion Complete flag = true`}
3. `Hit Event` -> `CC(Effect) execution`: { `Create a CC Transition Request`} -> `Hit Event`: {`CC execution flag = true`}
4. if `Hit Event` : {`Hit event digestion completion flag = true`} then `Hit Event Manager` : {{`Hit Event`} = `Delete`}


## `Data`
* `Collision Detection`:
    - `Skill Entity ID`
    - `Character Entity ID`
    - `World Clock time`
    - `Collision digestion complete flag`

* `Hit Event`:
    - `skill`(`Hit Skill Entity`)
    - `target`(`Hit Character Entiy`)
    - `Skill ID`
    - `Event ID`
    - `Spawn Time`(world time)
    - `CC execution flag`
    - `Hit event digestion completion flag`

* `HitEventDatabase`:
    - `std::vector<HitEvent>`

## `System`
* `Collision Detection System`: (Core features)
    1. Detecting collisions between skill hitbox shapes and character hitboxes
    2.  if `Collision was Detected` `Character Entity` and `Skill Entity` then `Create Hit Event`

* `Hit Event Manager System`: 
    1. Execution management of Hit Event contents: `CC execution flag`
    2. Hit Event Lifespan Management: if `hit event digestion completion flag = true` then `Delete` Delete the `hit event`

* `CC(Effect) execution System`: 
    1. if `hit event` has `CC effect` then `Create CC Transition Request` on the `character Entity`
    2. Change the `CC execution flag = true` of the `hit event`


* `Init Hit Event Database`: 
    1. Create a "HitEventDatabase" resource in the ECS global resource.


### System model
`Collision Detection` -> `Hit Event Manager` -> `CC execution` -> `CC FSM`

</details>

<details>
<summary id="section-id"> <strong>詳細設計 </strong> </summary>

## 動作目的(責務)
- 

## Data Flow (model)

</details>

---
---

<details>
<summary id="debug"> <strong>デバッグ </strong> </summary>

## デバッグ
> [!Caution] 問題
> 同一targetとskillhitboxの衝突を何度もpushしてしまう問題

- 同一衝突を同一イベントだと識別するための仕組みが必要
- ->HitEventに衝突判定前にその衝突をすでにイベント化済みかどうかを判定する
- ->HitEventデータ側にそのためにデータが必要

### 問題の構造
- HitEventはDatabaseに`std::vector`で詰められている
- 衝突検出に使われるデータは`target`の`entity`とskillhitboxの`entity`である
- 1. つまり，`std::vector`を`entity`で検索できればいい
- 2. もしくは，Collision側で同じ衝突についての検出処理を制限するか
- -> 
- しかし`std::vector`では不可能である
- -> `hash`などを利用して別の指数を用いるか

> [!note] 解決案
> 2を採用<br>
> `Skillhitbox`に衝突済み`target`のデータ構造`HitmemoComponent`を追加する
>
> 1. 衝突した`target`を`HitmemoComponent`に追加する
> 2. `HitmemoComponent`を検索しすでに衝突した`target`との衝突計算をスキップする

</details>

