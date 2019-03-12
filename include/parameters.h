#ifndef CONSTANTS_H
#define CONSTANTS_h
#include <stdlib.h>

int num_res=0;
int num_body_states=0;
int num_cache_states=0;
int actions=0;
int time_horizon=0;
int kmax=0;
double tmax_bout=0.L;
double velocity=0.L;
int configurations=0;
int stomach_capacity=0;
int carry_capacity=0;

double *alpha, *c, *resgain, *digestibility, *cacheability, *ht, *mortality;
int *tid, *cost_action; double *tweight;

void set_parameters(int argc, char **argv)
{
	int count=1;
	num_res=atof(argv[count++]);
	num_body_states=atof(argv[count++]);
	num_cache_states=atof(argv[count++]);
	actions=atof(argv[count++]);
	time_horizon=atof(argv[count++]);
	kmax=atof(argv[count++]);
	tmax_bout=atof(argv[count++]);
	velocity=atof(argv[count++]);
	configurations=atof(argv[count++]);

	alpha=new double[num_res]; 
	c=new double[num_res];
	resgain=new double[num_res];
	digestibility=new double[num_res];
	cacheability=new double[num_res];
	ht=new double[num_res];
	mortality=new double[num_res+1];
	tid=new int[actions]; 
	tweight=new double[actions];
	cost_action = new int[actions];
	for(int i=0; i<num_res; i++) alpha[i]=atof(argv[count++]);
	for(int i=0; i<num_res; i++) c[i]=atof(argv[count++]);
	for(int i=0; i<num_res; i++) resgain[i]=atof(argv[count++]);
	for(int i=0; i<num_res; i++) digestibility[i]=atof(argv[count++]);
	for(int i=0; i<num_res; i++) cacheability[i]=atof(argv[count++]);
	for(int i=0; i<num_res; i++) ht[i] = atof(argv[count++]);
	for(int i=0; i<num_res+1; i++) mortality[i] = atof(argv[count++]);
	for(int i=0; i<actions; i++) tid[i] = atof(argv[count++]);
	for(int i=0; i<actions; i++) tweight[i] = atof(argv[count++]);
	stomach_capacity=atof(argv[count++]);
	carry_capacity=atof(argv[count++]);
	for(int i=0; i<actions; i++) cost_action[i]=atof(argv[count++]);
}

void delete_parameters()
{
	delete alpha, c, resgain, digestibility, ht, cacheability, mortality, tweight;
	delete tid, cost_action;
}

#endif
