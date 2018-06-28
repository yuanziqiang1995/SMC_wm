#include "Sampling.h"
#include<stdlib.h>
#include <random>
#include <ctime>
#include<cmath>
std::uniform_real_distribution<double> dist(0, 1.0);
std::mt19937 rng;
int randomint(int min, int max)
{
	return rand() % (max - min) + min;
}
double randomfloat()
{
	rng.seed(std::random_device{}());
	return abs(dist(rng));
}
double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;
	if (phase == 0) {
		do {
			double U1 = randomfloat();
			double U2 = randomfloat();
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);
		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	return X;
}
Sampling::Sampling()
{
	this->sample_size = 0;
	this->model_loaded = false;
	this->sampled = false;
	this->sampling_interval = 1;
	init_sampleresult();
}
Sampling::Sampling(BayesianModel bayesnet)
{
	this->bayesnet = bayesnet;
	this->sample_size = 0;
	this->model_loaded = true;
	this->sampled = false;
    this->sampling_interval = 1;
	init_sampleresult();
}
void Sampling::init_sampleresult()
{
	this->sampling_result.clear();
}
int Sampling::RandomChosse(vector<double> possiblelist)
{
	double sumpossible = 0.0;
	double randomfloatnum = randomfloat();
	for (int i = 0; i < possiblelist.size(); i++)
	{
		sumpossible += possiblelist[i];
	}
	randomfloatnum *= sumpossible;
	for (int i = 0; i < possiblelist.size(); i++)
	{
		randomfloatnum -= possiblelist[i];
		if (randomfloatnum < 0)
			return i;
	}
	return possiblelist.size() - 1;
}
void Sampling::get_one_sample()
{
	int cpd_num = bayesnet.cpd_list.size();
	if (this->sample_size == 0)
	{
		if (model_loaded == false)
		{
			//cout << "Please load a bayesianmodel!" << "\n";
			return;
		}
		if (bayesnet.model_checked == false)
		{
			//cout << "Please check the bayesianmodel!" << "\n";
			return;
		}
		//cout << "Sampling" << "\n";
		/*
		for (int i = 0; i < cpd_num; i++)
		{
			file1 << bayesnet.cpd_list[i].cpd.variable.c_str() << " ";
		}
		file1 << endl;*/
		for (int i = 0; i < cpd_num; i++)
		{

			cpd_order.push_back(bayesnet.cpd_list[i].cpd.variable);
			if (bayesnet.cpd_list[i].cpd.is_continuous == false)
			{
				original_sample.insert(pair<string, double>(bayesnet.cpd_list[i].cpd.variable, randomint(0, bayesnet.cpd_list[i].cpd.variable_card)));
				old_sample.push_back(randomint(0, bayesnet.cpd_list[i].cpd.variable_card));
			}
			else
			{
				original_sample.insert(pair<string, double>(bayesnet.cpd_list[i].cpd.variable, bayesnet.cpd_list[i].cpd.weights[0]));
				old_sample.push_back(bayesnet.cpd_list[i].cpd.weights[0]);
			}
		}
	}
	if ((sample_size + 1) % 500 == 0)
	{
		//cout << (sample_size + 1) << " samples have been generated" << "\n";
	}
	for (int i = 0; i < sampling_interval; i++)
	{
		for (int j = 0; j < cpd_num; j++)
		{
			double most_possible_result;
			if (bayesnet.cpd_list[j].cpd.is_continuous == false)
			{
				vector<double>posssible_list(bayesnet.cpd_list[j].cpd.variable_card);
				for (int k = 0; k < bayesnet.cpd_list[j].cpd.variable_card; k++)
				{
					original_sample[cpd_order[j]] = k;
					posssible_list[k] = bayesnet.query_discrete_cpd(bayesnet.cpd_list[j].cpd, original_sample);
				}
				most_possible_result = double(RandomChosse(posssible_list));
			}
			else
			{
				double mean = bayesnet.query_continuous_cpd(bayesnet.cpd_list[j].cpd, original_sample);
				double var = bayesnet.cpd_list[j].cpd.var;
				most_possible_result = gaussrand() * var + mean;
			}
			original_sample[cpd_order[j]] = most_possible_result;
			old_sample[j] = most_possible_result;
		}
	}
	vector<double> temp_samp(cpd_num);
	for (int j = 0; j < cpd_num; j++)
	{
		temp_samp[j] = original_sample[cpd_order[j]];
		//file1 << original_sample[cpd_order[j]] << " ";
	}
	//file1 << endl;
	//sampling_result.push_back(temp_samp);
	this->sample_size++;
	temp_samp.clear();
	sampled = true;
}
void Sampling::gibbs_sampler(int size)
{
	if (model_loaded == false)
	{
		//cout << "Please load a bayesianmodel!" << "\n";
		return;
	}
	if (bayesnet.model_checked == false)
	{
		//cout << "Please check the bayesianmodel!" << "\n";
		return;
	}
	int cpd_num = bayesnet.cpd_list.size();
	if (this->sample_size == 0)
	{
		//cout << "Sampling" << "\n";
		/*
		for (int i = 0; i < cpd_num; i++)
		{
			file1 << bayesnet.cpd_list[i].cpd.variable.c_str() << " ";
		}
		file1 << endl;*/
		for (int i = 0; i < cpd_num; i++)
		{

			cpd_order.push_back(bayesnet.cpd_list[i].cpd.variable);
			if (bayesnet.cpd_list[i].cpd.is_continuous == false)
			{
				original_sample.insert(pair<string, double>(bayesnet.cpd_list[i].cpd.variable, randomint(0, bayesnet.cpd_list[i].cpd.variable_card)));
				old_sample.push_back(randomint(0, bayesnet.cpd_list[i].cpd.variable_card));
			}
			else
			{
				original_sample.insert(pair<string, double>(bayesnet.cpd_list[i].cpd.variable, bayesnet.cpd_list[i].cpd.weights[0]));
				old_sample.push_back(bayesnet.cpd_list[i].cpd.weights[0]);
			}
		}
	}
	for (int i = 0; i < size*sampling_interval; i++)
	{
		if ((sampling_result.size() + 1) % 500 == 0 && i != 0 && (i + 1) % sampling_interval == 0)
		{
			cout << (sampling_result.size() + 1) << " samples have been generated" << "\n";
		}
		for (int j = 0; j < cpd_num; j++)
		{
			double most_possible_result;
			if (bayesnet.cpd_list[j].cpd.is_continuous == false)
			{
				vector<double>posssible_list(bayesnet.cpd_list[j].cpd.variable_card);
				for (int k = 0; k < bayesnet.cpd_list[j].cpd.variable_card; k++)
				{
					original_sample[cpd_order[j]] = k;

					posssible_list[k] = bayesnet.query_discrete_cpd(bayesnet.cpd_list[j].cpd, original_sample);
				}
				most_possible_result = double(RandomChosse(posssible_list));
			}
			else
			{
				double mean = bayesnet.query_continuous_cpd(bayesnet.cpd_list[j].cpd, original_sample);
				double var = bayesnet.cpd_list[j].cpd.var;
				most_possible_result = gaussrand() * var + mean;
			}
			original_sample[cpd_order[j]] = most_possible_result;
			old_sample[j] = most_possible_result;
		}
		if (i%sampling_interval == 0)
		{
			vector<double> temp_samp(cpd_num);
			for (int j = 0; j < cpd_num; j++)
			{
				temp_samp[j] = original_sample[cpd_order[j]];
				//file1 << original_sample[cpd_order[j]] << " ";
			}
			//file1 << endl;
			sampling_result.push_back(temp_samp);
			this->sample_size++;
			temp_samp.clear();
		}
	}
	sampled = true;
}
double Sampling::count_possibility(map<string, int>condition)
{
	if (sampled == false)
	{
		cout << "Plsase get samples first" << "\n";
		return 0;
	}
	vector<int>temp_list;
	int same_result = 0;
	for (int i = 0; i < cpd_order.size(); i++)
	{
		if (condition.find(cpd_order[i]) == condition.end())
		{
			temp_list.push_back(-1);
		}
		else
		{
			temp_list.push_back(condition[cpd_order[i]]);
		}
		//cout << temp_list[i] << " ";
	}//cout << endl;
	for (int i = 0; i < sampling_result.size(); i++)
	{
		int flag_same = 0;
		for (int j = 0; j < cpd_order.size(); j++)
		{
			if (temp_list[j] != -1 && temp_list[j] != int(sampling_result[i][j]))
				break;
			if (j == cpd_order.size() - 1)
				flag_same = 1;
		}
		if (flag_same == 1)
			same_result++;
	}
	return double(same_result) / double(sample_size);
}
void Sampling::setNet(BayesianModel BN)
{
	this->bayesnet = BN;
	this->model_loaded = true;
}
void Sampling::output_sample()
{
	if (sampled == false)
	{
		cout << "Plsase get samples first" << "\n";
		return;
	}
	for (int i = 0; i < cpd_order.size(); i++)
	{
		cout << cpd_order[i] << " ";
	}
	cout << "\n";
	for (int i = 0; i < sample_size; i++)
	{
		for (int j = 0; j < cpd_order.size(); j++)
		{
			cout << sampling_result[i][j] << " ";
		}
		cout << "\n";
	}
}
void Sampling::closefile()
{
	//this->file1.close();
}
void Sampling::settracefile(string filename)
{
	const char *p = filename.c_str();
	//file1.open(p);
}
