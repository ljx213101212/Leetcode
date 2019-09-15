#include "pch.h"
class Solution {
public:


/**
 *  vector<string> input = {"gin", "zen", "gig", "msg"};
    int output = sol->uniqueMorseRepresentations(input);

*/

/**
 * Runtime: 4 ms, faster than 81.81% of C++ online submissions for Unique Morse Code Words.
Memory Usage: 9.1 MB, less than 61.54% of C++ online submissions for Unique Morse Code Words.

set string ascii 
 */ 
    int uniqueMorseRepresentations(vector<string>& words) {
        vector<string> morse = {".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};
        set<string> counter;
        for (string word : words){
            string currMorse = "";
            for (char ww : word){  
                int currIdx = ww - 'a';
                currMorse+= morse[currIdx];
            }
            counter.insert(currMorse);
        }
        return counter.size();
    }
};