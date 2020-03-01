class Solution {
public:

    /**
     * 1. Trying to split the return value into sum of two cases
     * => ways(i) = same_to_i-1(i) + diff_to_i-1(i); --------------------------(1)
     * 
     * explaination:
     * 
     * i: the number of the posts.
     * ways(i): the return value.
     * same_to_i-1(i): The sum of all possible cases of last post (i) and second last post(i-1) are in SAME color. 
     * diff_to_i-1(i): The sum of all possible cases of last post (i) and second last post(i-1) are in DIFFERENT color. 
     * 
     * 2. Trying to contruct same_to_i-1(i) and diff_to_i-1(i) by ways(x)
     * => diff_to_i-1(i) = ways(i - 1) * (k - 1); -----------------------------(2)
     * 
     * Explanation:
     * Within diff_to_i-1(i), compare all possible cases of i post and all possible cases of i-1 post,
     * we found the difference is we have (k - 1) colors new cases.(if i-1 post is some color ,we have to minus that color)
     * (if there is no limitation of 3 same colors cannot be in a row)
     * we can simply input
     * => same_to_i-1(i):  ways(i - 1) * 1 ------------------------------------(2.1)
     * so combine (2) and (2.1) into (1)
     * => ways(i) = ways(i - 1) * k ------------------------------------(2.2)
     *
     * 
     * But since there is a limitation:  3 same colors cannot be in a row
     * => same_to_i-1(i):  diff_to_i-1(i-1) * 1 ------------------------------(3)
     * Explanation:
     * diff_to_i-1(i-1): the sum of all possible cases of second last post (i - 1) and third last post((i-1)-1) are in DIFFERENT color. 
     * If the second last post (i - 1)  and the third last post (i - 2) are DIFFERENT collors, then the last post (i)  color can be safely same to the second last post(i - 1).
     * 1: last post(i) has to be as same as second last post (i - 1), so we only have ONE choice based on diff_to_i-1(i-1) scenario.
     *
     * 3. Join (2)(3) into (1)
     * => ways(i) = diff_to_i-1(i-1) * 1 + ways(i - 1) * (k - 1) --------------------(4)
     * 
     * 4. Join (2) into (4)
     * => ways(i) = ways((i - 1) - 1) * (k - 1) + ways(i - 1) * (k - 1) --------------(5)
     * 
     * 5. Simplify (5)
     * =>ways(i) = ways(i - 2) * (k - 1) + ways(i - 1) * (k - 1) --------------(6)
     * => ways(i) = (k - 1) (ways(i - 2) + ways(i - 1)) ------------------(7)
     * 
     * 6. Construct dp by (7)
     * => dp(i) = (k - 1) (dp(i-2)+dp(i-1)) ------------------(8)
     * 
     * Explaination:
     * (8) is the return value.
     */ 

    int numWays(int n, int k) {
     
        // 1. 首先先想办法构造出以下结构 (至于如何引导自己的思维来构造这个结构，这个就是本题最大难点,我也不懂),暂且按这个思路走
        // =>  ways(i) = same_to_i-1(i) + diff_to_i-1(i); ----------------------(1)
        // 说明:
        // same_to_i-1: 倒数第一个post（i）和倒数第二个post (i-1) 的颜色一样的所有情况总和
        // diff_to_i-1: 倒数第一个post(i) 和倒数第二个post(i-1)的颜色不同的所有情况总和
        
        //2. 用ways(n) 来构造第一步等号右边的两个部分

        // => diff_to_i-1(i) = ways(i - 1) * (k - 1);  --------------------------(2)
        //说明:
        // n个post与n-1个post的所有情况相比,有k种颜色, 就有k-1种新情况, 于是满足这个条件diff_to_i-1(i) = ways(i - 1) * (k - 1);
        // ways(i-1):n-1个post的所有情况
        // k - 1:  k-1 种新情况
        // => same_to_i-1(i) = diff_to_i-1(i-1) * 1; ---------------------------------(3)
        //说明:
        // 因为不能有连续3个相同的颜色并列, 那么如果第 i - 1 个post和第 i - 2 个post已经确定是不同的颜色,就不用担心第i个post
        // 破坏这个规矩了. 于是我们选则 diff_to_i-1(i-1) * 1
        // diff_to_i-1(i-1): 倒数第二个post(i-1) 和倒数第三个post(i-1-1)的颜色不同的所有情况总和
        // 1: 倒数第一个(i) 也就是当前的这个post 和 倒数第二个post(i-1) 需要相同，所以我们只有1种选择
    
        //3. 把(2)(3)合并到 (1)式
        // => ways(i) = diff_to_i-1(i-1) * 1 + ways(i - 1) * (k - 1) --------------------(4)
        //4. 把(2)合并到(4)式
        // => ways(i) = ways((i - 1) - 1) * (k - 1) + ways(i - 1) * (k - 1) --------------(5)
        //5. 把(5)式简化
        // => ways(i) = ways(i - 2) * (k - 1) + ways(i - 1) * (k - 1) --------------(6)
        // => ways(i) = (k - 1) (ways(i - 2) + ways(i - 1)) ------------------(7)

        //6. 利用(7)式来构造 dp
        //dp(i) = (k - 1) (dp(i-2)+dp(i-1))
        //说明:
        //dp(i): 有n个post k种颜色的所有情况总和, 也就是返回值

        if (n == 0 || k == 0){
            return 0;
        }
        if (n == 1){
            return k;
        }
        if (n == 2){
            return k * k;
        }
        //Base 
        int dp[n];
        dp[0] = 0;
        dp[1] = k;
        dp[2] = k*k;
        //dp loop
        for (int i = 3 ; i <= n; i++){
            //The rationale of this line came from 6 steps above.
            dp[i] = (k - 1) * (dp[i-2] + dp[i -1]);
        }
        return dp[n];
    }   

};


Thank you for your explanation, it's really hard to come up with this solution. 
I made it more specific into 6 steps. Hopefully can bring more light here:


 
1. Try to split the return value into sum of two cases
```
=> ways(i) = same_to_i-1(i) + diff_to_i-1(i); --------------------------(1)
```

Explaination:


**i** : the number of the posts => n
**ways(i)**: the return value.
**same_to_i-1(i)**: The sum of all possible cases of last post (i) and second last post(i-1) are in SAME color. 
**diff_to_i-1(i)**: The sum of all possible cases of last post (i) and second last post(i-1) are in DIFFERENT color. 


2. Try to contruct same_to_i-1(i) and diff_to_i-1(i) by ways(x)

```
=> diff_to_i-1(i) = ways(i - 1) * (k - 1); -----------------------------(2)
```

Explanation:

Within **diff_to_i-1(i)**, compare all possible cases of i post and all possible cases of i-1 post,
we found the difference is we have (k - 1) colors new cases.(if i-1 post is some color ,we have to minus that color)
(if there is no limitation of 3 same colors cannot be in a row)
we can simply input
```
=> same_to_i-1(i):  ways(i - 1) * 1 ------------------------------------(2.1)
```
so combine (2) and (2.1) into (1)

```
=> ways(i) = ways(i - 1) * k ------------------------------------(2.2)
```


But since there is a limitation:  3 same colors cannot be in a row
```
=> same_to_i-1(i):  diff_to_i-1(i-1) * 1 ------------------------------(3)
```

Explanation:

**diff_to_i-1(i-1)**: the sum of all possible cases of second last post (i - 1) and third last post((i-1)-1) are in DIFFERENT color. 
If the second last post (i - 1)  and the third last post (i - 2) are DIFFERENT collors, then the last post (i)  color can be safely same to the second last post(i - 1).
**1**: last post(i) has to be as same as second last post (i - 1), so we only have ONE choice based on diff_to_i-1(i-1) scenario.

3. Join (2)(3) into (1)
```
=> ways(i) = diff_to_i-1(i-1) * 1 + ways(i - 1) * (k - 1) --------------------(4)
```
4. Join (2) into (4)
```
=> ways(i) = ways((i - 1) - 1) * (k - 1) + ways(i - 1) * (k - 1) --------------(5)
```
5. Simplify (5)
```
=>ways(i) = ways(i - 2) * (k - 1) + ways(i - 1) * (k - 1) --------------(6)
=> ways(i) = (k - 1) * (ways(i - 2) + ways(i - 1)) ------------------(7)
```
6. Construct dp by (7)
```
=> dp(i) = (k - 1) * (dp(i-2) + dp(i-1)) ------------------(8)
```
Explaination:
(8) is the return value.
     
```
int numWays(int n, int k) {

    if (n == 0 || k == 0){
        return 0;
    }
    if (n == 1){
        return k;
    }
    if (n == 2){
        return k * k;
    }
    //Base 
    int dp[n];
    dp[0] = 0;
    dp[1] = k;
    dp[2] = k*k;
    //dp loop
    for (int i = 3 ; i <= n; i++){
        //The rationale of this line came from 6 steps above.
        dp[i] = (k - 1) * (dp[i-2] + dp[i -1]);
    }
    return dp[n];
}
```


