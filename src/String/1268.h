#include "pch.h"
class Solution {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        
        vector<vector<string>> ret;
        sort(products.begin(), products.end());
        string currSearch = "";
        for (char c : searchWord){
             currSearch += c;
             int searchSize = currSearch.length();
            vector<string> currTurn;
             for (string p : products){
                string subP = p.substr(0,searchSize);
                if (subP == currSearch &&
                     currTurn.size() < 3){
                    currTurn.push_back(p);
                }
             }
   
             ret.push_back(currTurn);
            
        }
        return ret;
    }
};