#ifndef SDP_H
#define SDP_H

#include <stdio.h>
double max(double *values, size_t n);
int argmax(double *values, size_t n);
int min(int a, int b);
int max(int a, int b);
int cap(int a, int b);
int modulo(int a, int b);


class SDP_base
{
	protected:
	size_t states, actions, time_horizon;
	double *terminal_fitness;
	double **fitness;
	int **best_decision;
		
	public:
	void print_parameters();
	void print_terminal_fitness();
};


class SDP_1 : public SDP_base
{
	protected:
	double ***transition_matrix;
	double **fitness;
	int **best_decision;
		
	public:
	SDP_1(size_t s, size_t a, size_t T, double ***transit_mat, double *term_fit);
	~SDP_1();
	void print_transition_matrix();
	void solve();
	void print_fitness();
	void print_best_decision();
};

class SDP_2 : public SDP_base
{
	protected:
	size_t translate_states;
	double **translate_dist; // instead of transition matrix. translate_dist gives the probability of translating a state by an amount given in translate_seq
	double **translate_seq;
	char boundary;
		
	public:
	SDP_2(size_t s, size_t a, size_t T, size_t trans_states, double **trans_dist, double **trans_seq, double *term_fit, char bound);
	~SDP_2();
	void print_translate_dist();
	void solve();
	void print_fitness();
	void print_best_decision();
};

class SDP_s : public SDP_2
{
	protected:
	int body_states, cache_states, stomach_capacity, carry_capacity, num_resources;
	int *cost_action;
	double *digestibility, *cacheability, *mortality;
	int *tid;
	double *tweight;

	public:
	SDP_s(size_t s, size_t a, size_t T, size_t trans_states, double **trans_dist, double **trans_seq, double *term_fit, char bound);
	~SDP_s();
	void set_additional_parameters(int num_body_states, int num_cache_states, int num_res, int num_stomach_capacity, int num_carry_capacity, double *digest, double *cache, double *mortal, int *cost, int *ti, double *tw);
	int addition_rule(int state1, int bounty, int action);
	void solve();
	void print_fitness();
	void print_best_decision();
};


#endif
