/* Embedded Control System modeling in SystemC
 */


#include "utils.h"


// get max
double get_max(const double x, const double y) {
	if (x > y)
		return x;
	else return y;
}


/*
 * r : the random generator
 * K : size of the outcomes
 * P : array containts the probabilities of the discrete events, 
 * they must be positive
 * output : the discrete random number from 0 to K - 1
 */ 
size_t get_discrete(const gsl_rng *r, size_t K, const double *P) {
	gsl_ran_discrete_t *r_discrete;
	size_t rndvalue;

	r_discrete = gsl_ran_discrete_preproc(K,P);
	rndvalue = gsl_ran_discrete(r,r_discrete);
	// release the memory
	gsl_ran_discrete_free(r_discrete);

	return rndvalue;
}


/*
 * r : the random generator
 * sigma : sigma value of the gaussion distribution
 * mu : mu value of the gaussian distribution
 * rndvalue : the random value of the gaussian distribution
 */
double get_gaussian(const gsl_rng *r, double sigma, double mu) {
	double rndvalue;

	rndvalue = gsl_ran_gaussian(r,sigma);
	rndvalue = rndvalue + mu;

	return rndvalue;
}


/*
 * r : the random generator
 * p : the probability parameter P(0) = 1 - p, P(1) = p
 * rndvalue : the random value of the bernoulli distribution
 */
int get_bernoulli(const gsl_rng *r, double p) {
	int rndvalue;

	rndvalue = gsl_ran_bernoulli(r,p);

	return rndvalue;
}


/*
 * r : the random generator
 * mu : the mean of the distribution = 1 / rate 
 * rndvalue : the random value of the exponential distribution
 */
double get_exponential(const gsl_rng *r, double mu) {
	double rndvalue;

	rndvalue = gsl_ran_exponential(r,mu);

	return rndvalue;
}


double get_exponential_P(double x, double mu) {
	return gsl_cdf_exponential_P(x,mu);
}

