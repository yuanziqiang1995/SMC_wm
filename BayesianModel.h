#pragma once
#include "CPD.h"
#include<cstring>
#include<vector>
#include<iostream>
#include<map>
using namespace std;
class BayesianModel
{
public:
	BayesianModel();
	BayesianModel(string *cpd_pair,int pair_num);
	void get_cpd_info();
	void add_cpds(int cpd_num, vector<CPD>cpd_list);
	struct Graph
	{
		CPD cpd;
		vector<string>father_name;
	};
	vector<Graph> cpd_list;
	map<string, int >node_loc;
	
	double query_joint(map<string, int>evidence);
	bool model_checked;
	bool check_model();
	void add_edge(CPD cpd_fa, CPD cpd_son);
	double query_discrete_cpd(CPD cpd_temp, map<string, double>evidence);
	double query_continuous_cpd(CPD cpd_temp, map<string, double>evidence);
	
private:
	
	double query_all_cpd(map<string, int>evidence);
	bool check_loop(int beg);
	bool flag_loop[100000];
	
};

