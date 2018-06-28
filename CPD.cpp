#include "CPD.h"
#include<string.h>
#include<vector>
using namespace std;
CPD::CPD()
{
	this->variable_card = 0;
	this->evidence_num = 0;
}
CPD::CPD(bool is_continuous)
{
	this->variable_card = 0;
	this->evidence_num = 0;
	this->is_continuous = is_continuous;
}
CPD::CPD(string variable, int variable_card, double *values, int evidence_num,string *evidence_inputed,int *evidence_card)
{
	int matrix_columns_num = 1;
	this->variable = variable;

	this->variable_card = variable_card;
	this->evidence_num = evidence_num;
	for (int i = 0; i < evidence_num; i++)
	{
		matrix_columns_num *= evidence_card[i];
		this->evidence_card.push_back(evidence_card[i]);
		this->evidence.push_back(evidence_inputed[i]);
	}
	for (int i = 0; i < variable_card; i++)
	{
		vector<double>vec_temp;
		for (int j = 0; j < matrix_columns_num; j++)
		{
			int temp_num = i * matrix_columns_num + j;
			vec_temp.push_back(values[temp_num]);
		}
		this->values.push_back(vec_temp);
		vector<double>().swap(vec_temp);
	}
	delete[]evidence_inputed;
	delete[]evidence_card;
	delete[]values;
}

void CPD::setValues(vector<double> temp_values)
{
	int matrix_columns_num = 1;
	for (int i = 0; i < evidence_num; i++)
	{
		matrix_columns_num *= evidence_card[i];
	}
	for (int i = 0; i < variable_card; i++)
	{
		vector<double>vec_temp;
		for (int j = 0; j < matrix_columns_num; j++)
		{
			int temp_num = j * variable_card + i;
			vec_temp.push_back(temp_values[temp_num]);
		}
		this->values.push_back(vec_temp);
		vector<double>().swap(vec_temp);
	}
}
void CPD::get_cpd_info()
{
	cout << "variable: "<<variable << endl;
	cout << "variable_card: " << variable_card << endl;
	if (is_continuous == false)
	{
		cout << "values: " << endl;
		for (int i = 0; i < values.size(); i++)
		{
			for (int j = 0; j < values[i].size(); j++)
			{
				cout << values[i][j] << " ";
			}
			cout << "\n";
		}
		cout << "evidence_num: " << evidence_num << endl;
		for (int i = 0; i < evidence_num; i++)
		{
			cout << "evidence:" << evidence[i] << "   evidence_card:" << evidence_card[i] << endl;
		}
	}
	else
	{
		cout << "weights:";
		for (int i = 0; i < weights.size(); i++)
		{
			cout << weights[i] << "";
		}
		cout << "\n";
		cout << "var:" << var << endl;
	}
	cout << endl;
}
