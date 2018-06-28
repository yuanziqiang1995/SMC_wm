#include "BayesianModel.h"
using namespace std;
#include <random>
#include <ctime>

BayesianModel::BayesianModel()
{
	this->model_checked = false;
}
BayesianModel::BayesianModel(string *cpd_pair, int pair_num)
{
	this->model_checked = false;
	Graph node_temp;
	CPD cpd_temp;
	for (int i = 0; i < 2*pair_num; i+=2)
	{
		if (this->node_loc.find(cpd_pair[i]) == this->node_loc.end())
		{

			cpd_temp.variable = cpd_pair[i];
			node_temp.cpd = cpd_temp;
			this->cpd_list.push_back(node_temp);
			this->node_loc.insert(pair<string, int>(cpd_pair[i], cpd_list.size() - 1));
		}
		if (this->node_loc.find(cpd_pair[i+1]) == this->node_loc.end())
		{
			cpd_temp.variable = cpd_pair[i+1];
			node_temp.cpd = cpd_temp;
			this->cpd_list.push_back(node_temp);
			this->cpd_list[cpd_list.size() - 1].father_name.push_back(cpd_pair[i]);
			this->node_loc.insert(pair<string, int>(cpd_pair[i + 1], cpd_list.size() - 1));
		}
		else
		{
			this->cpd_list[node_loc[cpd_pair[i+1]]].father_name.push_back(cpd_pair[i]);
		}
	}
	delete[]cpd_pair;
}
void BayesianModel::add_edge(CPD cpd_fa, CPD cpd_son)
{
	Graph node_temp;

	if (this->node_loc.find(cpd_fa.variable) == this->node_loc.end())
	{
		node_temp.cpd = cpd_fa;
		this->cpd_list.push_back(node_temp);
		this->node_loc.insert(pair<string, int>(cpd_fa.variable, cpd_list.size() - 1));
	}
	if (this->node_loc.find(cpd_son.variable) == this->node_loc.end())
	{
		node_temp.cpd = cpd_son;
		this->cpd_list.push_back(node_temp);
		this->cpd_list[cpd_list.size() - 1].father_name.push_back(cpd_fa.variable);
		this->node_loc.insert(pair<string, int>(cpd_son.variable, cpd_list.size() - 1));
	}
	else
	{
		this->cpd_list[node_loc[cpd_son.variable]].father_name.push_back(cpd_fa.variable);
	}
	cpd_list[node_loc[cpd_fa.variable]].cpd.variable_card = cpd_fa.variable_card;

	cpd_list[node_loc[cpd_son.variable]].cpd.evidence_num += 1;
	cpd_list[node_loc[cpd_son.variable]].cpd.evidence.push_back(cpd_fa.variable);
	cpd_list[node_loc[cpd_son.variable]].cpd.evidence_card.push_back(cpd_fa.variable_card);
}
void BayesianModel::add_cpds(int cpd_num, vector<CPD>temp_cpd)
{
	int cpd_loc;
	for (int i = 0; i < cpd_num; i++)
	{
		cpd_loc = node_loc[temp_cpd[i].variable];
		cpd_list[cpd_loc].cpd = temp_cpd[i];
	}
	//delete []temp_list;
}
double BayesianModel::query_discrete_cpd(CPD cpd_temp, map<string, double>evidence)
{
	int values_num = 1;
	int values_loc = 0;
	int father_num = cpd_temp.evidence_num;
	for (int i = 0; i < father_num; i++)
	{
		values_num *= cpd_temp.evidence_card[i];
	}
	for (int i = 0; i < father_num; i++)
	{
		string f_name = cpd_temp.evidence[i];
		values_num /= cpd_temp.evidence_card[i];
		values_loc += int(evidence[f_name]) * values_num;
	}
	return cpd_temp.values[int(evidence[cpd_temp.variable])][values_loc];/**/
}
double BayesianModel::query_continuous_cpd(CPD cpd_temp, map<string, double>evidence)
{
	int father_num = cpd_temp.evidence_num;
	double mean = cpd_temp.weights[0];
	for (int i = 0; i < father_num; i++)
	{
		string f_name = cpd_temp.evidence[i];
		mean += cpd_temp.weights[i + 1] * evidence[f_name];
	}
	return mean;
}
double BayesianModel::query_all_cpd(map<string, int>evidence)
{
	double possibility = 1;
	for (int i = 0; i < cpd_list.size(); i++)
	{
		CPD cpd_temp = cpd_list[i].cpd;
		map<string, double>evidence_temp;
		evidence_temp.insert(pair<string, double>(cpd_temp.variable, evidence[cpd_temp.variable]));
		for (int j = 0; j < cpd_list[i].father_name.size(); j++)
		{
			evidence_temp.insert(pair<string, double>(cpd_list[i].father_name[j], evidence[cpd_list[i].father_name[j]]));
		}
		possibility *= query_discrete_cpd(cpd_temp, evidence_temp);
		cout << query_discrete_cpd(cpd_temp, evidence_temp) << endl;
		map<string, double>().swap(evidence_temp);
		//cout << cpd_temp.variable << " " << query_discrete_cpd(cpd_temp, evidence_temp) <<" "<< possibility<< "\n";
	}
	map<string, int>().swap(evidence);
	return possibility;
}

double BayesianModel::query_joint(map<string, int>evidence)
{
	if (model_checked == false)
	{
		cout << "Please check the bayesianmodel!" << "\n";
		return 0;
	}
	int cpd_num = cpd_list.size();
	int flag_sufficient_evidence = 0;
	double result = 0;
	map<string, int>temp_evidence;
	temp_evidence = evidence;
	for (int i = 0; i < cpd_num; i++)
	{
		if (evidence.find(cpd_list[i].cpd.variable) == evidence.end())
		{
			for (int j = 0; j < cpd_list[i].cpd.variable_card; j++)
			{

				if (j == 0)
				    temp_evidence.insert(pair<string, int>(cpd_list[i].cpd.variable, j));
				else temp_evidence[cpd_list[i].cpd.variable] = j;
				result += query_joint(temp_evidence);
			}
			map<string, int>().swap(temp_evidence);
			//evidence.swap(map<string, int>());
			return result;
		}
	}
	map<string, int>().swap(temp_evidence);
	//evidence.swap(map<string, int>());
	return query_all_cpd(evidence);
}

bool BayesianModel::check_model()
{
	int error_checked = 0;
	int cpd_num = cpd_list.size();
	if (cpd_num == 0)
	{
		cout << "Please add CPDs first!" << "\n";
		return 0;
	}
	//cout<<"Checking the model" << "\n";
	//
        /*
	for (int i = 0; i < cpd_num; i++)
	{
		if (error_checked > 0)break;
		for (int j = 0; j < cpd_list[i].father_name.size(); j++)
		{
			if (find(cpd_list[i].cpd.evidence.begin(), cpd_list[i].cpd.evidence.end(), cpd_list[i].father_name[j]) == cpd_list[i].cpd.evidence.end())
			{
				error_checked = 1;
				break;
			}
		}
		for (int j = 0; j < cpd_list[i].cpd.evidence.size(); j++)
		{
			if (find(cpd_list[i].father_name.begin(), cpd_list[i].father_name.end(), cpd_list[i].cpd.evidence[j]) == cpd_list[i].father_name.end())
			{
				error_checked = 1;
				break;
			}
		}
	}
        */
	//
	if (error_checked == 0)
	{
		bool *flag = new bool[cpd_num];
		for (int i = 0; i < cpd_num; i++)
		{
			flag[i] = 0;
		}
		for (int i = 0; i < cpd_num; i++)
		{
			memset(flag_loop, 0, sizeof(flag_loop));
			if (check_loop(i) == 1)
				error_checked = 2;
		}
	}
	//
	double sum_possible;
	int situation_num;
	for (int i = 0; i < cpd_num; i++)
	{
		if (error_checked > 0)break;
		situation_num = 1;
		if (cpd_list[i].cpd.is_continuous == false)
		{
			for (int j = 0; j < cpd_list[i].cpd.evidence_num; j++)
			{
				if (cpd_list[node_loc[cpd_list[i].cpd.evidence[j]]].cpd.is_continuous == true)
				{
					error_checked = 4;
					break;
				}
			}
			if (cpd_list[i].cpd.values.size() == 0)
			{
				for (int j = 0; j < cpd_list[i].cpd.variable_card; j++)
				{
					vector<double>temp;
					temp.push_back(1 / double(cpd_list[i].cpd.variable_card));
					cpd_list[i].cpd.values.push_back(temp);
				}
			}
			for (int j = 0; j < cpd_list[i].cpd.evidence_num; j++)
			{
				situation_num *= cpd_list[i].cpd.evidence_card[j];
			}
			for (int j = 0; j < situation_num; j++)
			{
				sum_possible = 0;
				for (int k = 0; k < cpd_list[i].cpd.variable_card; k++)
				{
					sum_possible += cpd_list[i].cpd.values[k][j];
				}
				if (sum_possible > 1.0 + 0.0000001 || sum_possible < 1.0 - 0.0000001)
				{
					error_checked = 3;
					break;
				}
			}
		}
	}

	switch (error_checked)
	{
	case 1:
		cout << "error:Graph structure and CPD information do not match" << "\n";
		break;
	case 2:
		cout << "error:There is a loop in the graph structure"<<"\n";
		break;
	case 3:
		cout << "error:Illegal input of values" << "\n";
		break;
	case 4:
		cout << "error:continuous CPDs can not have discrete child" << "\n";
		break;
	default:
		//cout << "Model Checkeed" << "\n";
		model_checked = true;
		break;
	}
	if (error_checked > 0)return 1;
	else return 0;
}

bool BayesianModel::check_loop(int beg)
{
	int result = 0;
	if (flag_loop[beg] == 1)
		return 1;
	else
	{
		for (int i = 0; i < cpd_list[beg].father_name.size(); i++)
		{
			int next = node_loc[cpd_list[beg].father_name[i]];
			flag_loop[beg] = 1;
			result = check_loop(next);
			flag_loop[next] = 0;
			if (result == 1)
				break;
		}
		return result;
	}
}

void BayesianModel::get_cpd_info()
{
	for (int i = 0; i < cpd_list.size(); i++)
	{
		cout << "cpd:"<<cpd_list[i].cpd.variable<<"\nfathers:";
		for (int j = 0; j < cpd_list[i].father_name.size(); j++)
		{
			cout << cpd_list[i].father_name[j] << " ";
		}
		cout << "\n";
	}
}
