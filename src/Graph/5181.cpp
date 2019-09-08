#include "5181.hpp"


/**
    Solution * sol = new Solution();
    vector<int> input = {1,2,3,4};
    int output = sol->distanceBetweenBusStops(input,0,3);
 */

/**
 * 
 */
int Solution::distanceBetweenBusStops(vector<int>& distance, int start, int destination) {  

    int curr = start;
    int disStartToDest = calculateStartToDest(distance,start,destination);
    int disDestToStart = calculateStartToDest(distance,destination,start);
    return std::min(disStartToDest,disDestToStart);
}

int Solution::calculateStartToDest(vector<int>& distance,int start,int destination){
  int res = 0;
  int curr = start;
  int disLen = distance.size();
  while (curr != destination){
       res += distance[curr];
        if (curr + 1 == disLen)
        {
            curr = 0;
        }
        else
        {
            curr++;
        }
    }
    return res;
}