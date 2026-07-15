# gfx loader の設計・実装のメモ

```
// TODO:
// file asset ではなく sub-asset key を正式導入する。
// 例:
//   model: res://foo.glb#scene=0
//   anim : res://foo.glb#anim=0
//   prim : res://foo.glb#prim=12
//
// AssetManager は key -> AssetID のみ担当し、
// 各 loader/system が key 生成・解釈を担当する。
```

設計改善メモ
```cpp
/* 
* @TODO 
    A node is not necessarily a bone (or joint). 
    However, it is necessary to account for the rotation and scaling of nodes that do not correspond to joints.
    This is not currently implemented; a solution is required.
*/
// parentNode = import.nodes[nodeIndex].parent
// skin.jointNodes[i]: The index number of the node that has that skin
// i: 親 node の skin 番号 (親 bone)
int FindParentBoneIndex(const ImportSkin& skin, int parentNode)
{
    if (parentNode < 0)
        return -1;

    for (std::size_t i = 0; i < skin.jointNodes.size(); i++)
    {
        if (parentNode == skin.jointNodes[i])
            return static_cast<int>(i);
    }

    return -1;
}
```