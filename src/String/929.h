#include "pch.h"
class Solution {
public:
/**
 * vector<string> input = {"test.email+alex@leetcode.com","test.e.mail+bob.cathy@leetcode.com","testemail+david@lee.tcode.com"};
    int output = sol->numUniqueEmails(input);

 * set substr find string concat
 */ 
    using Emails = vector< string >;
    using Unique = unordered_set< string >;
    int numUniqueEmails(vector<string>& emails){
        set<string> res;
        for (string email : emails){
            size_t at = email.find('@');
            string pendingStr = email.substr(0,at);
            string endStr = email.substr(at);
            string startStr = ""; 
            for (char c : pendingStr){
                if (c == '.'){
                    continue;
                }else if (c == '+'){
                    break;
                }else{
                    startStr += c;
                }
            }
            string finalStr = startStr + endStr;
            res.insert(finalStr);
        }
        return res.size();
    }

    /**
     * C++ erase remove idiom
     * @ref:https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
     */

    int numUniqueEmails_2(vector<string> &emails, Unique unique={}){

        for (auto &e : emails)
        {
            auto pivot = e.find_first_of('@');
            auto name = e.substr(0, pivot),
                 domain = e.substr(pivot);

            //C++ erase remove idiom
            string::iterator it = remove(name.begin(), name.end(), '.');
            name.erase(it, name.end());
            auto pos = name.find_first_of('+');
            unique.insert((pos != string::npos) ? name.erase(pos) + domain : name + domain);
        }
        return static_cast< int >( unique.size() );
    }
};