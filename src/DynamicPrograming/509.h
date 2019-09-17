class Solution {
public:
/**
 * Hello world DP
 * O(N)
 * S(31) -> S(1) 
 */
    int fib(int N) {
        if (N == 0) {
            return 0;
        }
        if (N == 1){
            return 1;
        }
        int dp [31] = {};
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2 ; i <= N; i++){
           dp[i] = dp[i - 1] + dp [i - 2];
        }
        return dp[N];
    }
};