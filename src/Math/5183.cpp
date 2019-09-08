
#include "5183.hpp"

/**
 *  Solution * sol = new Solution();
    int year = 2019;
    int month = 9;
    int day = 8;
    string output = sol->dayOfTheWeek(day,month,year);
 */


/**
 * 

where  denotes the integer floor function,
k is day (1 to 31)
m is month (1 = March, ..., 10 = December, 11 = Jan, 12 = Feb) Treat Jan & Feb as months of the preceding year
C is century (1987 has C = 19)
Y is year (1987 has Y = 87 except Y = 86 for Jan & Feb)
W is week day (0 = Sunday, ..., 6 = Saturday)
 */
string Solution::dayOfTheWeek(int day, int month, int year){
        int C = year / 100;
        int Y = year % 100;
        if (month == 1 || month == 2){
            Y--;
        }
        int W = (day + (int)floor(2.6 * month - 0.2) - 2 * C + Y + (int)floor(Y /4 ) + (int)floor(C/4)) % 7;
        return to_string(W);
}