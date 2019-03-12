#include "../include/sdp.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <omp.h>

double max(double *values, size_t n)
{
	double result=0.L;
	for(int i=0; i<n; i++) if(values[i]>result) result=values[i];
	return result;
}

int argmax(double *values, size_t n)
{
	int result=0; double maximum=0.L;
	for(int i=0; i<n; i++){ if(values[i]>maximum){ result=i; maximum=values[i]; }}
	return result;
}
	
int min(int a, int b)
{
	if(a<b) return a;
	else return b;
}

double min(double a, double b)
{
	if(a<b) return a;
	else return b;
}

int max(int a, int b)
{
	if(a>b) return a;
	else return b;
}

int cap(int a, int b)
{
	int result=a;
	if(a>b) result=b;
	return result;
}

int modulo(int a, int b)
{
	int result=a%b;
	if(a<0) result = a%b+b;
	return result;
} 

void SDP_base::print_parameters()
{
	std::cout << "Number of states: " << states << std::endl;
	std::cout << "Number of actions: " << actions << std::endl;
	std::cout << "Time horizon: " << time_horizon << std::endl << std::endl;
}

void SDP_base::print_terminal_fitness()
{
	std::cout << "Terminal fitness: " << std::endl;
	for(int i=0; i<states; i++)
	{
		std::cout << "State " << i << ":\t" << terminal_fitness[i] << std::endl;
	}
	std::cout << std::endl;
}

SDP_1::SDP_1(size_t s, size_t a, size_t T, double ***transit_mat, double *term_fit)
{
	states=s; actions=a; time_horizon=T;
	terminal_fitness = new double[states];
	for(int i=0; i<states; i++) terminal_fitness[i]=term_fit[i];
	transition_matrix = new double **[actions];
	for(int i=0; i<actions; i++)
	{
		transition_matrix[i] = new double *[states];
		for(int j=0; j<states; j++)
		{
			transition_matrix[i][j] = new double[states];
			for(int k=0; k<states; k++) transition_matrix[i][j][k] = transit_mat[i][j][k];
		}
	}
	
	fitness = new double *[time_horizon];
	best_decision = new int *[time_horizon];
	for(int i=0; i<time_horizon; i++)
	{
		fitness[i] = new double[states];
		best_decision[i] = new int[states];
		for(int j=0; j<states; j++)
		{
			fitness[i][j]=0.L;
			best_decision[i][j]=0;
		}
	}	
}
	
SDP_1::~SDP_1()
{
	delete terminal_fitness;
	for(int i=0; i<actions; i++)
	{
		for(int j=0; j<states; j++)
		{
			delete transition_matrix[i][j];
		}
		delete transition_matrix[i];
	}
	delete transition_matrix;
	for(int i=0; i<time_horizon; i++)
	{
		delete fitness[i];
		delete best_decision[i];
	}
	delete fitness;
	delete best_decision;
}

void SDP_1::print_transition_matrix()
{
	std::cout << "Transition matrix: " << std::endl;
	for(int i=0; i<actions; i++)
	{
		std::cout << "Action " << i << ":" << std::endl;
		for(int j=0; j<states; j++)
		{
			for(int k=0; k<states; k++)
			{
				std::cout << transition_matrix[i][j][k] << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}


void SDP_1::solve()
	{
	for(int state=0; state<states; state++) fitness[time_horizon-1][state]=terminal_fitness[state];
	for(int t=time_horizon-2; t>=0; t--)
		{
		for(int state1=0; state1<states; state1++)
			{
			fitness[t][state1]=0.L;
			double fitness_for_action[actions]={0.L};
			for(int action=0; action<actions; action++)
				{
				for(int state2=0; state2<states; state2++) fitness_for_action[action] += transition_matrix[action][state1][state2]*fitness[t+1][state2];
				}
			fitness[t][state1]=max(fitness_for_action, actions);
			best_decision[t][state1]=argmax(fitness_for_action, actions);
			}
		}

	}
	
void SDP_1::print_fitness()
{
	std::cout << "Fitness: " << std::endl;
	for(int i=0; i<time_horizon; i++)
	{
		std::cout << "t = " << i << ':' << std::endl;
		for(int j=0; j<states; j++)
		{
			std::cout << "State " << j << ":\t" <<  fitness[i][j] << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void SDP_1::print_best_decision()
{
	std::cout << "Best decisions: " << std::endl;
	for(int i=0; i<time_horizon; i++)
	{
		std::cout << "t = " << i << ':' << std::endl;
		for(int j=0; j<states; j++)
		{
			std::cout << "State " << j << ":\t" << best_decision[i][j] << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


SDP_2::SDP_2(size_t s, size_t a, size_t T, size_t trans_states, double **trans_dist, double **trans_seq, double *term_fit, char bound)
{
	states=s; actions=a; time_horizon=T; translate_states=trans_states; boundary=bound;
	terminal_fitness = new double[states];
	for(int i=0; i<states; i++) terminal_fitness[i]=term_fit[i];
	translate_dist = new double *[actions];
	translate_seq = new double *[actions];
	for(int i=0; i<actions; i++)
	{
		translate_dist[i] = new double [translate_states];
		translate_seq[i] = new double [translate_states];
		for(int j=0; j<translate_states; j++){	translate_dist[i][j] = trans_dist[i][j]; translate_seq[i][j] = trans_seq[i][j]; }
	}
	
	fitness = new double *[time_horizon];
	best_decision = new int *[time_horizon];
	for(int i=0; i<time_horizon; i++)
	{
		fitness[i] = new double[states];
		best_decision[i] = new int[states];
		for(int j=0; j<states; j++)
		{
			fitness[i][j]=0.L;
			best_decision[i][j]=0;
		}
	}	
}
	
SDP_2::~SDP_2()
{
	delete terminal_fitness;
	for(int i=0; i<actions; i++)
	{
		delete translate_dist[i];
		delete translate_seq[i];
	}
	delete translate_dist;
	delete translate_seq;
	for(int i=0; i<time_horizon; i++)
	{
		delete fitness[i];
		delete best_decision[i];
	}
	delete fitness;
	delete best_decision;
}

void SDP_2::print_translate_dist()
{
	std::cout << "Translation distribution: " << std::endl;
	for(int i=0; i<actions; i++)
	{
		std::cout << "Action " << i << ":" << std::endl;
		for(int j=0; j<translate_states; j++)
		{
			std::cout << translate_seq[i][j] << '\t' << translate_dist[i][j] << std::endl;
		}
		std::cout << std::endl;
	}
}



void SDP_2::solve()
{
	for(int state=0; state<states; state++) fitness[time_horizon-1][state]=terminal_fitness[state];
	for(int t=time_horizon-2; t>=0; t--)
	{
		for(int state1=0; state1<states; state1++)
		{
			fitness[t][state1]=0.L;
			double fitness_for_action[actions]={0.L};
			for(int action=0; action<actions; action++)
			{
				for(int state2=0; state2<translate_states; state2++)
				{
					if(boundary=='m') fitness_for_action[action] += translate_dist[action][state2]*fitness[t+1][modulo(state1+(int)translate_seq[action][state2],states)];
					if(boundary=='c') fitness_for_action[action] += translate_dist[action][state2]*fitness[t+1][cap(state1+(int)translate_seq[action][state2],states)];
				}
			}
			fitness[t][state1]=max(fitness_for_action, actions);
			best_decision[t][state1]=argmax(fitness_for_action, actions);
		}
	}

}
	
void SDP_2::print_fitness()
{
	std::cout << "Fitness: " << std::endl;
	for(int i=0; i<time_horizon; i++)
	{
		std::cout << "t = " << i << ':' << std::endl;
		for(int j=0; j<states; j++)
		{
			std::cout << "State " << j << ":\t" <<  fitness[i][j] << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void SDP_2::print_best_decision()
{
	std::cout << "Best decisions: " << std::endl;
	for(int i=0; i<time_horizon; i++)
	{
		std::cout << "t = " << i << ':' << std::endl;
		for(int j=0; j<states; j++)
		{
			std::cout << "State " << j << ":\t" << best_decision[i][j] << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


SDP_s::SDP_s(size_t s, size_t a, size_t T, size_t trans_states, double **trans_dist, double **trans_seq, double *term_fit, char bound) : SDP_2(s, a, T, trans_states, trans_dist, trans_seq, term_fit, bound)
{
}

SDP_s::~SDP_s()
{
	delete digestibility, mortality;
	delete cost_action;
}


void SDP_s::set_additional_parameters(int num_body_states, int num_cache_states, int num_res, int num_stomach_capacity, int num_carry_capacity, double *digest, double *cache, double *mortal, int *cost, int *ti, double *tw)
{
	body_states=num_body_states;
	cache_states=num_cache_states;
	num_resources=num_res;
	stomach_capacity = num_stomach_capacity;
	carry_capacity = num_carry_capacity;
	digestibility = new double[num_resources]; 
	cacheability = new double[num_resources]; 
	mortality = new double[actions];
	cost_action = new int[actions];
	tid = new int[actions];
	tweight = new double[actions];
	for(int i=0; i<num_resources; i++){ digestibility[i]=digest[i]; cacheability[i]=cache[i]; mortality[i]=mortal[i]; } mortality[num_resources]=mortal[num_resources];
	for(int i=0; i<actions; i++){ cost_action[i] = cost[i]; tid[i]=ti[i]; tweight[i]=tw[i]; }
}

int SDP_s::addition_rule(int state1, int bounty, int action)
{
	int bodystate1=state1%body_states, cachestate1=(int)state1/body_states;
	int bodystate2=min(body_states-1, bodystate1-cost_action[action]+min(bounty,stomach_capacity));
	int cachestate2=cachestate1;
	if(bounty>0) bounty -= (bodystate2-bodystate1);
	if(bodystate2<bodystate1+stomach_capacity){ cachestate2=max(0,cachestate1-(body_states-1-bodystate2)); bodystate2 += cachestate1-cachestate2; }
	if(bodystate2<0) bodystate2=0;
	cachestate2=min(cache_states-1, cachestate2+(int)min(cacheability[tid[action]]*(double)bounty,(double)carry_capacity));
	return cachestate2*body_states+bodystate2;
}

void SDP_s::solve()
{
	for(int state=0; state<states; state++) fitness[time_horizon-1][state]=terminal_fitness[state];
	std::cout << "Solving SDP: T = " << time_horizon-1 << ' ';
	for(int t=time_horizon-2; t>=0; t--)
	{
		std::cout << t << ' ' << std::flush;
		#pragma omp parallel for
		for(int state1=0; state1<states; state1++)
		{
			fitness[t][state1]=0.L;
			double fitness_for_action[actions]={0.L};
			for(int action=0; action<actions; action++)
			{
				for(int k=0; k<translate_states; k++)
				{
					int state2=addition_rule(state1,(int)translate_seq[action][k],action); 
					if((state2%body_states)!=0) fitness_for_action[action] += translate_dist[action][k]*fitness[t+1][state2];
				}
				fitness_for_action[action] *= (1.L-mortality[action]);
			}
			fitness[t][state1]=max(fitness_for_action, actions);
			best_decision[t][state1]=argmax(fitness_for_action, actions);
		}
		for(int cachestate=0; cachestate<cache_states; cachestate++) fitness[t][cachestate*body_states]=0.L;
	}
	std::cout << std::endl;

}


void SDP_s::print_fitness()
{
	std::cout << "Writing fitness matrices onto data/fitness/ ..." << std::endl;
	char outputfilename[40]="data/fitness/T=00";
	char outputfilename2[time_horizon][40];
	for(int i=0; i<time_horizon; i++) for(int j=0; j<40; j++) outputfilename2[i][j]=outputfilename[j];
	std::ofstream output[time_horizon];
	#pragma omp parallel for
	for(int i=0; i<time_horizon; i++)
	{
		outputfilename2[i][15]=(char)(48+int(i/10));
		outputfilename2[i][16]=(char)(48+int(i%10));
		output[i].open(outputfilename2[i]);
		for(int j=0; j<body_states; j++)
		{
			for(int k=0; k<cache_states; k++)
			{
				output[i] << fitness[i][k*body_states+j] << '\t';
			}
			output[i] << std::endl;
		}
		output[i].close();
	}
}

void SDP_s::print_best_decision()
{
	std::cout << "Writing decision matrices onto data/decision/ ..." << std::endl;
	char outputfilename[40]="data/decision/T=00";
	char outputfilename2[time_horizon][40];
	for(int i=0; i<time_horizon; i++) for(int j=0; j<40; j++) outputfilename2[i][j]=outputfilename[j];
	std::ofstream output[time_horizon];
	#pragma omp parallel for
	for(int i=0; i<time_horizon; i++)
	{
		outputfilename2[i][16]=(char)(48+int(i/10));
		outputfilename2[i][17]=(char)(48+int(i%10));
		output[i].open(outputfilename2[i]);
		for(int j=0; j<body_states; j++)
		{
			for(int k=0; k<cache_states; k++)
			{
				output[i] << best_decision[i][k*body_states+j] << '\t';
			}
			output[i] << std::endl;
		}
		output[i].close();
	}
}

