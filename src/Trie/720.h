#include "pch.h"
class Solution
{
public:

/**
 *  1.利用set构建字符映射字典
 *  2.排序原vector(比长短，若长短一样，比alphabet order)
 *  3.循环排序后的数组，判断每一个元素是否满足题目要求，若满足 则是答案
 */ 
    string longestWord(vector<string> &words)
    {
        set<string> s;
        for (string word : words)
        {
            s.insert(word);
        }
        sort(words.begin(), words.end(), sortByLength);
        for (string word : words)
        {
            bool isWordPossible = true;
            string currString = "";
            for (char w : word)
            {
                currString += w;
                if (!s.count(currString))
                {
                    isWordPossible = false;
                    break;
                }
            }
            if (isWordPossible)
            {
                return word;
            }
        }
        return "";
    }

    static bool sortByLength(string s1, string s2)
    {
        if (s1.length() == s2.length())
        {
            return s1 < s2;
        }
        return s1.length() > s2.length();
    }

      /** Initialize your data structure here. */
    struct TrieNode{
        TrieNode* arr[26];
        bool eow;
    };
    
    /**Build a function to make a node and return TrieNode **/
    TrieNode* getNode()
    {
        TrieNode* node = new TrieNode;
        for(int i=0;i<26;i++)
            node->arr[i]=NULL;
        node->eow = false;
        
        return node;
    }
    
    /** Make the root TrieNode **/
    TrieNode* root = getNode();

      /** Insert a word into Trie **/
    void insert(string word)
    {
        TrieNode* temp = root;
        for(auto w: word)
        {
            int index = w - 'a';
            if(temp->arr[index]==NULL)
                temp->arr[index] = getNode();
            temp = temp->arr[index];
            
        }
        temp->eow = true;
    }

      /** SPECIAL Search function specially for this problem **/
    
    string search()
    {
        string ans="";
        int maxvalue=0;
        spsearch(root,0,"",maxvalue,ans);
        
        return ans;
    }
    
    void spsearch(TrieNode* r,int curr,string currstr,int& max,string& ans)
    {
        TrieNode* temp = r;
        
        //For each node check all alphabets from a-z (Since lexico) 
        for(int i=0;i<26;i++)
        {
            if(temp->arr[i]==NULL)
                continue;
            
            if(temp->arr[i]->eow)
            {
                curr++;
                currstr+=(char)(i+'a');
                
                // cout<<"--------------------"<<endl;
                // cout<<" Word: "<<(char)(i+'a')<<"   "<<curr<<"  y: "<<currstr<<endl;
                ans = curr>max ? currstr : ans;
                max = curr>max ? curr : max;
                
                
                // cout<<" Word: "<<(char)(i+'a')<<"   "<<max<<"  y: "<<ans<<endl;
                // cout<<"--------------------"<<endl;
                spsearch(temp->arr[i],curr,currstr,max,ans);

                curr--;
                currstr.pop_back();
            }
            
        }//for
    }

      string longestWord_2(vector<string> &words){
             
        for(auto word:words)
            insert(word);
        
        string result = search();
        
        return result;
      }
    
};
