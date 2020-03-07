#include "pch.h"

//my first try
//isdigit #include <ctype.h>
//atoi #include <stdlib.h>
class StringIterator {
public:
    int ptr = 0;
    string str = "";
    char curr_c = '\0';
    int  curr_c_count = 0;
    StringIterator(string compressedString) {
        str = compressedString;
        ptr = 0;
    }
    char next() {
        
      if (ptr >= str.length() - 1){
         return ' '; 
      }
      int tmp_ptr = ptr;
      char curr = str[tmp_ptr++];
      int count_ptr = tmp_ptr;
       while (tmp_ptr < str.length()
             && isdigit(str[tmp_ptr])){
           tmp_ptr++;
       }
       if (count_ptr == tmp_ptr){
            return ' ';
       }
       int count = atoi(&str[count_ptr]);
       if (count == 0){
           return ' ';
       }
       //cover "g5g2" continous same char input case.
       int tmp_count = curr == curr_c ? (curr_c_count == 0 ? count : curr_c_count) : count;
       if (tmp_count == 0){
           return ' ';
       }
       else if (tmp_count == 1){
           //update
           ptr = tmp_ptr;
       }
        curr_c = curr;
        curr_c_count = tmp_count - 1;
       char tmp_curr =  curr == curr_c ? curr : curr_c;
       return tmp_curr;
        
    }
    
    bool hasNext() {
       
       if (curr_c_count != 0){
           return true;
       }
       int tmp_ptr = ptr;
       char curr = str[tmp_ptr++];
       char num_first = str[tmp_ptr];
       if (isdigit(num_first)){
           return true;
       }
       return false;
    }
};

/**
 * Your StringIterator object will be instantiated and called as such:
 * StringIterator* obj = new StringIterator(compressedString);
 * char param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */


//updated by referencing:
//@see https://leetcode.com/problems/design-compressed-string-iterator/discuss/103878/C%2B%2B-Java-Clean-Code

class StringIterator {
public:
    int ptr = 0;
    string str = "";
    char curr_c = '\0';
    int curr_c_count = 0;
    StringIterator(string compressedString)
    {
        str = compressedString;
        ptr = 0;
    }

    char next() {
        
    
       if (curr_c_count > 0){
           curr_c_count--;
       }else if (ptr < str.length() - 1){
            
              int tmp_ptr = ptr;
              char curr = str[tmp_ptr++];
              int count_ptr = tmp_ptr;
               while (tmp_ptr < str.length()
                     && isdigit(str[tmp_ptr])){
                   tmp_ptr++;
               }
               if (count_ptr == tmp_ptr){
                    return ' ';
               }
               int count = atoi(&str[count_ptr]);
               if (count == 0){
                   return ' ';
               }
           //update
           ptr = tmp_ptr;
           curr_c = curr;
           curr_c_count = count - 1;
       }else{
           return ' ';
       }
        
       return curr_c;        
    }
    
    bool hasNext() {
       return curr_c_count > 0 || ptr < str.length() - 1;
    }
};

/**
 * Your StringIterator object will be instantiated and called as such:
 * StringIterator* obj = new StringIterator(compressedString);
 * char param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */

