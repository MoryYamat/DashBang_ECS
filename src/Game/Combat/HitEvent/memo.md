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