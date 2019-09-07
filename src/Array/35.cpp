#include "35.hpp"

int searchInsert(vector<int> &nums, int target)
{
    int finalIndex = 0;
    for (auto num : nums)
    {
        if (num >= target)
        {
            return finalIndex;
        }
        finalIndex++;
    }
    return finalIndex;
}
