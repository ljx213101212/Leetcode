#include "pch.h"
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        
        int ret = 0;
        sort(intervals.begin(), intervals.end());
        vector<int> meetingOffTime = {};
        int currentOccupy = 0;
        for (vector<int> inter : intervals)
        {
            currentOccupy = 1;
            int start = inter[0];
            int end = inter[1];
            vector<int> removeTimeIndex = {};
            for (int i = meetingOffTime.size() - 1; i >= 0; i--)
            {
                if (start >= meetingOffTime[i]){
                    removeTimeIndex.push_back(i);
                }else{
                    currentOccupy++;
                }
            }
            for (int idx : removeTimeIndex){
                meetingOffTime.erase(meetingOffTime.begin() + idx);
            }

            meetingOffTime.push_back(end);
            ret = max(ret, currentOccupy);
        }

        return ret;
    }
};