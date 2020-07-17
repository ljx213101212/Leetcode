## Template  

```cpp
//https://leetcode.com/submissions/detail/367935487/
class Solution {
public:
    struct comp
    {
        template<typename T>
        bool operator()(const T& l, const T& r) const
        {
            if (l.second != r.second)
                return r.second < l.second;

            return l.first < r.first;
        }
    };
    vector<int> topKFrequent(vector<int>& nums, int k) {
        //1.map counter
        //2.set template
        
        vector<int> ret;
        map<int,int> m;
        for (auto num : nums){
            m[num]++;
        }
        set<pair<int,int>,comp> myset;
        for (auto const &p: m){
            myset.insert(p);
        }
        int count = 0;
        for (auto const &p : myset){
            if (count++ == k) break;
            ret.push_back(p.first);
        }
        return ret;
    }
};
```