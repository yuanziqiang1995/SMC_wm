
/**
 * the interface to the external software (C code) which wants to use the online model
 * checker
 */
#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "checker.hpp"
#include<stdio.h>
#include<vector>
#include"BayesianModel.h"
#include"Sampling.h"
#include"GM.h"
#include"interface.hpp"
#include <sys/types.h>
#include <dirent.h>
#include<fstream>
class interface
{
public:
    interface();
    string tracefile;
    vector<string> state_vars;
    vector<pair<string, double> > state;
    int length_explored;
    Checker *c;
    void read_property(char *);
    void init_signals(vector<string>);
    valType advance(vector<double>);
    int checkmodel(char*,char*);
    bool check_trace(Sampling,char*,char*);
};
#endif
