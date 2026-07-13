# ecs/component_storage の設計に利用したメモを残す

```cpp
// AoS
// struct Position
// {
//     float x,y,z;
// }
// struct AoS
// {
//     std::vector<Position> s;
// };

// SoA
// struct Position
// {
//     std::vector<float> x;
//     std::vector<float> y;
//     std::vector<float> z;
// };


// AoSoA
// struct Position3xWord
// {
//     std::size_t word = 8;
//     std::vector<float> x;
//     std::vector<float> y;
//     std::vector<float> z;
// };
// struct Position
// {
//     std::vector<Position3xWord> pos;
//     float GetPos(Index i){ return pos[i / word].x[i % word];}
// };
```