#include"GM.h"
#include <fstream>
GM::GM()
{

}
void GM::readUAI(string filename)
{
	string type;
	ifstream infile(filename);
	//if (!infile) cout << "error" << endl;
	int num_variables;
	string tmp_string;
	infile >> tmp_string;
	//cout << tmp_string << endl;
	if (tmp_string.compare("BAYES") == 0)
	{
		type = "BAYES";
	}
	else if (tmp_string.compare("MARKOV") == 0)
	{
		type = "MARKOV";
	}
	if (type.compare("BAYES") == 0)
	{
		//cout << "Reading Bayesian Network\n";
		infile >> num_variables;
		vector<CPD>cpds;
		vector<int> func_order;
		for (int i = 0; i < num_variables; i++)
		{
			CPD temp_cpd;
			int variable_card;
			infile >> variable_card;
			temp_cpd.variable_card = variable_card;
			temp_cpd.variable = to_string(i);
			if (variable_card != 0)
			{
				temp_cpd.is_continuous = false;
			}
			else temp_cpd.is_continuous = true;
			cpds.push_back(temp_cpd);

		}
		int num_functions;
		infile >> num_functions;

		for (int i = 0; i < num_functions; i++)
		{
			int num_cpds;
			infile >> num_cpds;
			vector<int>cpd_var;
			for (int j = 0; j < num_cpds; j++)
			{
				int temp;
				infile >> temp;
				cpd_var.push_back(temp);
			}
			num_cpds--;
			func_order.push_back(cpd_var[num_cpds]);
			for (int j = 0; j < num_cpds; j++)
			{
				this->bayesnet.add_edge(cpds[cpd_var[j]], cpds[cpd_var[num_cpds]]);
			}

		}
		for (int i = 0; i < num_variables; i++)
		{
			int var = i;
			int num_probabilities;
			double temp_double;
			infile >> num_probabilities;
			if (this->bayesnet.cpd_list[this->bayesnet.node_loc[to_string(var)]].cpd.variable_card > 0)
			{
				vector<double>values(num_probabilities);
				for (int j = 0; j < num_probabilities; j++)
				{
					infile >> values[j];
				}
				this->bayesnet.cpd_list[this->bayesnet.node_loc[to_string(var)]].cpd.setValues(values);
			}
			else
			{
				for (int j = 0; j < num_probabilities - 1; j++)
				{

					infile >> temp_double;
					this->bayesnet.cpd_list[this->bayesnet.node_loc[to_string(var)]].cpd.weights.push_back(temp_double);
				}
				infile >> temp_double;
				this->bayesnet.cpd_list[this->bayesnet.node_loc[to_string(var)]].cpd.var = temp_double;
			}
		}
		this->bayesnet.check_model();
		infile.close();
	}
}
