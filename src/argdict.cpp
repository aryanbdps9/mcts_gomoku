#include <bits/stdc++.h>
#include "argdict.h"

using namespace std;

bool argdict::add_int_arg(string argname, int argval){
    if (int_args.find(argname) != int_args.end()) return false;
    int_args[argname] = argval;
    return true;
}

bool argdict::add_dbl_arg(string argname, double argval){
    if (double_args.find(argname) != double_args.end()) return false;
    double_args[argname] = argval;
    return true;
}

int argdict::get_int_arg(string argname){
    if (int_args.find(argname) == int_args.end())
        throw logic_error("argdict::get_int_arg: argument: \""+argname+"\" of type int is not present in the argdict\n");
    
    return int_args[argname];
}

int argdict::get_dbl_arg(string argname){
    if (double_args.find(argname) == double_args.end())
        throw logic_error("argdict::get_dbl_arg: argument: \""+argname+"\" of type double is not present in the argdict\n");
    return double_args[argname];
}

bool argdict::contains_int(string argname){
    return (int_args.find(argname) != int_args.end());
}

bool argdict::contains_dbl(string argname){
    return (double_args.find(argname) != double_args.end());
}