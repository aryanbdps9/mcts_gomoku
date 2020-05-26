#ifndef Included_NameModel_Argdict

#define Included_NameModel_Argdict

#include <iostream>
#include <unordered_map>

using namespace std;

class argdict{
private:
    unordered_map<string, int> int_args;
    unordered_map<string, double> double_args;
    unordered_map<string, string> string_args;
public:
    bool add_int_arg(string argname, int argval);
    bool add_dbl_arg(string argname, double argval);
    bool add_str_arg(string argname, string argval);

    int get_int_arg(string argname);
    double get_dbl_arg(string argname);
    string get_str_arg(string argname);

    bool contains_int(string argname);
    bool contains_dbl(string argname);
    bool contains_str(string argname);

    inline bool has_int(string argname){return contains_int(argname);}
    inline bool has_dbl(string argname){return contains_dbl(argname);}
    inline bool has_str(string argname){return contains_str(argname);}

    string print();
};

#endif