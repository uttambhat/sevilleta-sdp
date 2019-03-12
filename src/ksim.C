#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <stdlib.h>
#include <omp.h>
#include "../include/parameters.h"

using namespace std;

int main(int argc, char **argv)
{
	set_parameters(argc, argv);

	char outputfilename[20]="data/ksim_data.dat";
	ofstream output;
	output.open(outputfilename);
	
 	int seed=1;
	mt19937 gen(seed);

	double probability[actions][kmax]={0.L}, proportion0[actions][kmax]={0.L}, proportionall[actions][kmax]={0.L};
	
	output << 0.L << '\t' << 1.L << endl;
	for(int i=1; i<kmax; i++) output << (double)i << '\t' << 0.L << endl;
	output << endl;
	
	cout << "Calculating kdist for action = ";
	for(int target=1; target<actions; target++)
		{
		cout << target << ' ' << flush;
		bernoulli_distribution bernoulli(1.L-tweight[target]);
		double data[configurations], data0[configurations];
		//#pragma omp parallel for
		for(int config=0; config<configurations; config++)
			{
			int number_of_successes[num_res]={0};
			int nearest_resource=0;
			double t=0.L;
			double distance_to_resource[num_res]={0.L};
			double nearest_distance=0.L;
			while(t<tmax_bout)
				{
				for(int i=0; i<num_res; i++)
					{
					gamma_distribution<double> gammadist(alpha[i],1.L/c[i]);
					double lambda=gammadist(gen);
					exponential_distribution<double> exponential(lambda);
					distance_to_resource[i]=exponential(gen);
					}
				int distancetuple0=0;
				double distancetuple1=1.e100L;
				for(int i=0; i<num_res; i++)
					{
					if(distance_to_resource[i]<distancetuple1)
						{
						distancetuple0=i;
						distancetuple1=distance_to_resource[i];
						}
					}
				double nearest_distance=distancetuple1;
				int nearest_resource=distancetuple0;
				if(nearest_resource==tid[target])
					{
					number_of_successes[tid[target]]++;
					t += ht[tid[target]];
					}
				else
					{
					if(bernoulli(gen)==1)
						{
						number_of_successes[nearest_resource]++;
						t += ht[nearest_resource];
						}
					}
				
				
				t += nearest_distance/velocity;
					
				}
			double total_kilojoules=0.L;
			for(int i=0; i<num_res; i++) total_kilojoules += resgain[i]*number_of_successes[i];
			data[config]=total_kilojoules;
			data0[config] = resgain[0]*number_of_successes[0];
			}
			
		double datamin=1.e100L, datamax=0.L, epsilon=1.e-10L;
		for(int i=0; i<configurations; i++)
			{
			if(data[i]<datamin) datamin=data[i];
			if(data[i]>datamax) datamax=data[i];
			}
		for(int config=0; config<configurations; config++)
			{
			if(datamin!=datamax)
				{
				int k=(int)((double)kmax*(data[config]-datamin)/(datamax-datamin+epsilon));
				probability[target][k] += 1.L/(double)configurations;
				proportion0[target][k] += data0[config];
				proportionall[target][k] += data[config];
				}
			else probability[target][0] += 1.L/(double)configurations;
			}	
			
		
		if(datamin!=datamax)
			{
			for(int i=0; i<kmax; i++) output << (double)i*(datamax-datamin+epsilon)/(double)kmax + datamin << '\t' << probability[target][i] << endl;
			//for(int i=0; i<kmax; i++) output << (double)i*(datamax-datamin+epsilon)/(double)kmax + datamin << '\t' << probability[target][i] << '\t' << proportion0[target][i]/proportionall[target][i] << endl;
			}
		else
			{
			output << datamin << '\t' << probability[target][0] << endl;
			for(int i=0; i<kmax-1; i++) output << datamin*(1.01L+(double)i*0.01L) << '\t' << 0.L << endl;
			}
		output << endl;
		}
	cout << endl;
	output.close();
	
	delete_parameters();
	}
