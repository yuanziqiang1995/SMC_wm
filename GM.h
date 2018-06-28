#pragma once
#include "Sampling.h"
class GM
{
public:
	GM();
	BayesianModel bayesnet;
	Sampling sampler;
	void readUAI(string filename);
};