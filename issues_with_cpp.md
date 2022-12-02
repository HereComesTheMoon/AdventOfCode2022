### Issues with C++
Day2: 
You can't hash a tuple or pair in C++ by default. 
std::map and std::unordered_map have different behavior here. Since unordered_map uses actual hashing, you cannot use a tuple or pair as a key. std::map is a RB-Tree, so it is possible.

