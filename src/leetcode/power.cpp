class Solution {
public:
    double myPow(double x, long long n) {
    if (x == 1) return 1;
    else if (x == -1) {
        if (n % 2 == 0) return 1;
        else return -1;
    }
    if (n == 0) return 1;
	else if (n > 0 && n % 2 == 0) return myPow(x * x, n / 2);
	else if (n > 0) return myPow(x, n - 1) * x;
	else return 1.0 / myPow(x, -n);     
    }
};
