#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "../include/sdp.h"
#include "../include/parameters.h"

using namespace std;
double terminalfitness(int bodystate, int cachestate);

int main(int argc, char **argv)
{
	set_parameters(argc, argv);
	int states=num_body_states*num_cache_states;
	
	double *terminal_fitness=new double[states];
	for(int i=0; i<states; i++) terminal_fitness[i]=terminalfitness(i%num_body_states, (int)i/num_body_states);
	
	double **kdist=new double *[actions], **kinfo=new double *[actions];
	for(int i=0; i<actions; i++)
	{
		kdist[i] = new double[kmax];
		kinfo[i] = new double[kmax];
	}
	ifstream input_kdist; input_kdist.open("data/ksim_data.dat");
	for(int i=0; i<actions; i++)
	{
		for(int j=0; j<kmax; j++)
		{
			double data1=0.L, data2=0.L, data3; input_kdist >> ws >> data1 >> ws >> data2 >> ws;
			kinfo[i][j]=data1; kdist[i][j]=data2;
		}
	}
	input_kdist.close();
	
	SDP_s sevilleta(states, actions, time_horizon, kmax, kdist, kinfo, terminal_fitness, 's');
	sevilleta.set_additional_parameters(num_body_states, num_cache_states, num_res, stomach_capacity, carry_capacity, digestibility, cacheability, mortality, cost_action, tid, tweight);
	sevilleta.solve();
	sevilleta.print_fitness();
	sevilleta.print_best_decision();
	
	delete terminal_fitness;
	for(int i=0; i<actions; i++){ delete kdist[i]; delete kinfo[i]; }
	delete kdist, kinfo;
	delete_parameters();
}
	
double terminalfitness(int bodystate, int cachestate)
{
	if(bodystate==0) return 0.L;
	else return (double)bodystate+0.2L*(double)cachestate;
}

