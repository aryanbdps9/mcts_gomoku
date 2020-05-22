#ifndef Included_NameModel_Argdict

#define Included_NameModel_Argdict

#include <bits/stdc++.h>

using namespace std;

class argdict{
private:
    unordered_map<string, int> int_args;
    unordered_map<string, double> double_args;
public:
    bool add_int_arg(string argname, int argval);
    bool add_dbl_arg(string argname, double argval);

    int get_int_arg(string argname);
    int get_dbl_arg(string argname);

    bool contains_int(string argname);
    bool contains_dbl(string argname);

    inline bool has_int(string argname){return contains_int(argname);}
    inline bool has_dbl(string argname){return contains_dbl(argname);}
};

#endif