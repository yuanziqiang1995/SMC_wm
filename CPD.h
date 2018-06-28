#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;
class CPD
{
public:
	CPD();
	CPD(bool is_continuous);
	CPD(string variable,int variable_card, double *values, int evidence_num,string *evidence,int *evidence_card);
	bool is_continuous;
	void get_cpd_info();
	int variable_card;
	int evidence_num;
	string variable;
	vector<vector<double> > values;
	vector<string>evidence;
	vector<int>evidence_card;
	void setValues(vector<double> temp_values);
	vector<double>weights;
	double var;
};

