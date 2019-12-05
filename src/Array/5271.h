#include "pch.h"

class Solution {
public:
    int minTimeToVisitAllPoints(vector<vector<int>>& points) {
        
        if (points.size() == 0){ return 0;}
        vector<int> start(points[0]);
        int currTime = 0;
        
        for (int i = 1; i < points.size(); i++){
            vector<int> end = points[i];
            int diffX = abs(start[0] - end[0]);
            int diffY = abs(start[1] - end[1]);
            currTime += min(diffX,diffY) + abs(diffX - diffX);
            start[0] = end[0];
            start[1] = end[1];
        }
        
        return currTime;
    }
};