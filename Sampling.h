#pragma once
#include "BayesianModel.h"
#include<string>
#include<vector>
#include<iostream>
#include <fstream>
using namespace std;
class Sampling
{
public:
	Sampling();
	Sampling(BayesianModel);
	void gibbs_sampler(int size);
	void output_sample();
	double count_possibility(map<string, int>condition);
	vector<string>cpd_order;
	vector<vector<double> >sampling_result;
	vector<double>old_sample;
	void setNet(BayesianModel BN);
	void init_sampleresult();
	int sample_size;
	int RandomChosse(vector<double>);
	void get_one_sample();
	void closefile();
	void settracefile(string);
private:
	map<string, double>original_sample;
	BayesianModel bayesnet;
	string trace_file;
	bool model_loaded;
	bool sampled;
    int sampling_interval;
};

