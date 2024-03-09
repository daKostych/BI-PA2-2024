#ifndef __TRAINER__
#include <algorithm>
#include <cassert>
#include <iostream>

// struktura pro kvádr
struct Cuboid
{
    int sides[3];
};

struct Array
{
    Cuboid *data;
    size_t count, capacity;
};

void addCuboid(Array &arr, const Cuboid &cub)
{
    if (arr.count == arr.capacity)
    {
        arr.capacity = arr.capacity * 2 + 100;
        Cuboid *new_arr = new Cuboid[arr.capacity];
        for (std::size_t i = 0; i < arr.count; i++)
        {
            new_arr[i] = arr.data[i];
        }
        delete[] arr.data;
        arr.data = new_arr;
    }
    arr.data[arr.count++] = cub;
}

void destroyArray(Array &arr)
{
    delete[] arr.data;
}
bool equalCuboids(const Cuboid &cub1, const Cuboid &cub2)
{
    for (int i = 0; i < 3; i++)
    {
        if (cub1.sides[i] != cub2.sides[i])
            return false;
    }
    return true;
}
#endif

bool readCuboid(Cuboid &cub)
{
    if (std::cin >> cub.sides[0] >> cub.sides[1] >> cub.sides[2])
    {
        if (cub.sides[0] <= 0 || cub.sides[1] <= 0 || cub.sides[2] <= 0)
            return false;
        return true;
    }
    else
        return false;
}

void normalizeCuboid(Cuboid &cub)
{
    std::sort(cub.sides, cub.sides + 3);
}

bool compareCuboids(const Cuboid &cub1, const Cuboid &cub2)
{
    for (int i = 0; i < 3; i++)
    {
        if (cub1.sides[i] != cub2.sides[i])
            return cub1.sides[i] < cub2.sides[i];
    }
    return false;
}

#ifndef __TRAINER__
int main()
{
    assert(compareCuboids({{1, 2, 3}}, {{2, 3, 4}}));
    assert(!compareCuboids({{2, 3, 4}}, {{1, 2, 3}}));
    assert(!compareCuboids({{2, 3, 4}}, {{2, 3, 4}}));
    assert(compareCuboids({{2, 3, 4}}, {{2, 3, 5}}));
    return 0;
}
#endif