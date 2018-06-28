#pragma once
#include<iostream>
#include"CPD.h"
#include"BayesianModel.h"
#include"Sampling.h"
#include<vector>
void build_net_by_code()
{
	int value_card;
	int evidence_num;
	string *evidence;
	int* evidence_card;
	double* values;
	vector<CPD>cpd_list;
	//瀹氫箟鑺傜偣G

	evidence = new string[2]{ "I", "D" };
	evidence_card = new int[2]{ 2,2 };
	values = new double[12]{ 0.3, 0.05, 0.9, 0.5,
		0.4, 0.25, 0.08, 0.3,
		0.3, 0.7, 0.02, 0.2 };

	value_card = 3;
	evidence_num = 2;
	CPD cpd_g("G", value_card, values, evidence_num, evidence, evidence_card);
	cpd_list.push_back(cpd_g);

	//瀹氫箟鑺傜偣L
	evidence = new string[1]{ "G" };
	evidence_card = new int[1]{ 3 };
	values = new double[12]{ 0.1, 0.4, 0.99,
		0.9, 0.6, 0.01 };
	value_card = 2;
	evidence_num = 1;
	CPD cpd_l("L", value_card, values, evidence_num, evidence, evidence_card);
	cpd_list.push_back(cpd_l);

	//瀹氫箟鑺傜偣S

	evidence = new string[1]{ "I" };
	evidence_card = new int[1]{ 2 };
	values = new double[4]{ 0.95, 0.2 ,
		0.05, 0.8 };

	value_card = 2;
	evidence_num = 1;
	CPD cpd_s("S", value_card, values, evidence_num, evidence, evidence_card);
	cpd_list.push_back(cpd_s);

	//瀹氫箟鑺傜偣D
	evidence = new string[2];
	evidence_card = new int[2];
	values = new double[4]{ 0.6, 0.4 };

	value_card = 2;
	evidence_num = 0;
	CPD cpd_d("D", value_card, values, evidence_num, evidence, evidence_card);
	cpd_list.push_back(cpd_d);

	//瀹氫箟鑺傜偣I
	evidence = new string[2];
	evidence_card = new int[2];
	values = new double[2]{ 0.7, 0.3 };
	value_card = 2;
	evidence_num = 0;
	CPD cpd_i("I", value_card, values, evidence_num, evidence, evidence_card);
	cpd_list.push_back(cpd_i);


	string *cpd_pair;
	int pair_num = 4;
	cpd_pair = new string[10]{ "D", "G",  "I", "G",  "G", "L",  "I", "S" };
	BayesianModel bayesnet(cpd_pair, pair_num);
	//bayesnet.GetCPDinfo();
	bayesnet.add_cpds(5, cpd_list);
	bayesnet.check_model();




	map<string, int>qe;
	qe.insert(pair<string, int>("D", 0));
	qe.insert(pair<string, int>("G", 1));
	qe.insert(pair<string, int>("I", 0));
	qe.insert(pair<string, int>("L", 1));
	qe.insert(pair<string, int>("S", 0));


	double jointfromQuery = bayesnet.query_joint(qe);
	Sampling sample1(bayesnet);
	sample1.gibbs_sampler(2000);
	//sample1.OutputSample();
	double jointfromSampler = sample1.count_possibility(qe);
	cout << jointfromQuery << " " << jointfromSampler << "\n";
}
