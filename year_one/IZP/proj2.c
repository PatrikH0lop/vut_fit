/*
	Author : Patrik Holop, xholop01@stud.fit.vutbr.cz
	Name   : proj2.c
	Date   : November 2016
	Project: Iterative calculations
	About  : Implement logarithm and exponencial function
             with Taylors series and partial fractions
*/

//Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Defined Values
#define WRONG_ARG -1
#define ERROR -1

//List of functions
int process_arguments(int argc, char *argv[]);
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
double mylog(double x);
double mypow(double x, double y);

/**
 * Main function
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 if completed successfully, else ERROR
 */
int main(int argc, char *argv[])
{
    int input_state = process_arguments(argc,argv);
    if (input_state == WRONG_ARG) {
        fprintf(stderr,"Wrong arguments\n");
        return ERROR;
    }
    return 0;
}

/**
 * Calculates logarithm with taylors series or sum
 * @param x Fixed number
 * @param n Number of iterations
 * @return Value of logarithm if successful, or INPUT_ERROR if not
 */
double taylor_log(double x, unsigned int n) {

	double iterations_left = n;

    if (x==0.0)
        return -INFINITY;
    else if (x<0.0)
        return NAN;
	if (isnan(x))
		return NAN;
	if (isinf(x))
		return INFINITY;
	
    // Taylors series for log(1-x) = -x-(x^2)/2 - ...
    else if (x<1.0) {
        double taylors_x = 1-x;
        double numerator = taylors_x;
        double denominator = 1.0;
        double result = 0.0;

        while (iterations_left > 0) {
            result-=numerator/denominator;
        	numerator*=taylors_x; // x of higher power
        	denominator++;
        	iterations_left--;
        }

        return result;
    }

    // Sum for log(x) = (((x-1)/x)^n)/n ...
    else if (x>=1.0) {
        double sum = 0.0;
        double numerator = ((x-1)/x);
        double denominator = 1.0;
        for (double i = 1; i <= iterations_left; i++) {
            denominator = i;
            sum += numerator/denominator;
            numerator *= ((x-1)/x);
        }
        return sum;
    }

    return WRONG_ARG;
}

/**
 * Calculates logarithm with method of partial fractions
 * @param Fixed number
 * @param Number of iterations
 * @return Value of logarithm if successful, or INPUT_ERROR
 */
double cfrac_log(double x, unsigned int n) {

	
    if (x==0.0)
        return -INFINITY;
    else if (x<0.0)
        return NAN;
	if (isnan(x))
		return NAN;
	if (isinf(x))
		return INFINITY;
    if (x==1.0)
        return 0;

    double z = (1-x)/(-x-1);
    double fraction = 0.0;
    double iterations_left = n-1;
    while (iterations_left>=1) {
        fraction=(iterations_left*iterations_left*z*z)/(((iterations_left+1)*2-1)-fraction);
        iterations_left--;
    }
    double denominator = 1.0-fraction;
    double result = 2.0*z/denominator;
    return result;
}

/**
 * Calculate exponencial function (with taylor_log)
 * @param x Base number
 * @param y Power number
 * @param n Number of iteration
 * @return Return x^y
 */
double taylor_pow(double x, double y, unsigned int n) {

	if (x==1.0) 
		return 1.0;
	if (y==0.0)
		return 1.0;
	if (x==0.0)
		return 0.0;
    if (x<0.0)
        return NAN;
	if (isinf(x) && y>0) 
		return INFINITY;
	else if (isinf(x) && y<0) 
		return 0;
	if (isnan(x) || isnan(y))
		return NAN;
    if (y==1.0)
	return x;
    // x^y = 1 +(y*ln x)/1! + (y^2 * ln^2 x)/2! + ...
    double iterations_left = n;
    double denumerator = 1.0;
    double num = y*taylor_log(x,n);
    double numerator = num;
    double sum = 1.0+numerator/denumerator;
    
    for (double i = 2.0; i<=iterations_left; i++) {
            numerator*=num;
            denumerator = denumerator*i;            
            sum+=numerator/denumerator;
    }

    return sum;
}

/**
 * Calculate exponencial function (with cfrag_log)
 * @param x Base number
 * @param y Power number
 * @param n Number of iteration
 * @return Return x^y if successfull
 */
double taylorcf_pow(double x, double y, unsigned int n) {
   
	if (x==1.0) 
		return 1.0;
	if (y==0.0)
		return 1.0;
	if (x==0.0)
		return 0.0;
    if (x<0.0)
        return NAN;
	if (isinf(x) && y>0) 
		return INFINITY;
	else if (isinf(x) && y<0) 
		return 0;
	if (isnan(x) || isnan(y))
		return NAN;
	if (y==1.0)
        	return x;
    // x^y = 1 +(y*ln x)/1! + (y^2 * ln^2 x)/2! + ...
    double iterations_left = n;
    double denumerator = 1.0;
    double num = y*cfrac_log(x,n);
    double numerator = num;	
    double sum = 1.0+numerator/denumerator;
    
    for (double i = 2.0; i<=iterations_left; i++) {
            numerator*=num;
            denumerator = denumerator*i;            
            sum+=numerator/denumerator;
    }

    return sum;
}

/**
 * Calculates minimal number of iterations for result
 * @param x Fixednumber
 * @return Logarithm of number
 */
double mylog(double x) {

    double eps = 0.00000001;
    
    if (x==0.0)
        return -INFINITY;
    else if (x<0.0)
        return NAN;
	if (isnan(x))
		return NAN;
	if (isinf(x))
		return INFINITY;
    if (x==1.0)
        return 0;
    
    else {    	
        double result=0.0;
        double previous_result=2.0;
        double iterations_left = 1;
        double fraction = 0.0;

        while (fabs(eps*(result-previous_result))<fabs(result-previous_result)) {        	
			previous_result = result;
            double z = (1-x)/(-x-1);
            double iterations = iterations_left;
            fraction = 0.0;
            while (iterations>=1) {
                fraction=(iterations*iterations*z*z)/(((iterations+1)*2-1)-fraction);
                iterations--;
            }
            double denominator = 1.0-fraction;
            result = 2.0*z/denominator;
            iterations_left++;
        }
        
        return result;
    }
    return 0;
}

/**
 * Calculates minimal number of iterations for result
 * @param x Base number
 * @param y Power
 * @return Base^Power
 */
double mypow(double x, double y) {
    // eps
    long double eps = 0.00000001;

	if (x==1.0) 
		return 1.0;
	if (y==0.0)
		return 1.0;
	if (x==0.0)
		return 0.0;
    if (x<0.0)
        return NAN;
	if (isinf(x) && y>0) 
		return INFINITY;
	else if (isinf(x) && y<0) 
		return 0;
	if (isnan(x) || isnan(y))
		return NAN;
    if (y==1.0)
        return x;

		
    double sum = 1.0;
    double denumerator = 1.0;
    double num = y*mylog(x);
    double numerator = num;
    double previous_sum= 0.0;
    double member = 1.0;
    double iterations = 2.0;

    // fractions
    while (fabs(sum-previous_sum)>fabs(eps*(sum-previous_sum))) {
            member=numerator/denumerator;
            numerator*=num;
            previous_sum = sum;
            sum+=member;
            denumerator = denumerator*iterations;
            iterations++;
    }
    return sum;
}

/**
 * Process arguments for specific behavior based on arguments
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 if successful, else WRONG_ARG
 */
int process_arguments(int argc, char *argv[]) {
	// arguments: --mylog X
	if (argc==3) {
		if (strcmp("--mylog",argv[1])==0) {
			char *x_test;
            double x = strtod(argv[2],&x_test);
            // not valid arguments
            if (*x_test!='\0')
            	return WRONG_ARG;
            else {
            	double result = mylog(x);
	        printf("log :  %.7e\n", log(x));
            	printf("mylog: %.7e\n",result);
            }
		}
		else
			return WRONG_ARG;
	}
	// arguments: --log X N or --mypow X Y
    else if (argc==4) {
		if (strcmp("--mypow",argv[1])==0) {
			char *x_test, *y_test;
            double x = strtod(argv[2],&x_test);
			double y = strtod(argv[3],&y_test);
			// Not valid arguments
            if (*x_test!='\0' || *y_test!='\0')
            	return WRONG_ARG;
            else {
            	double result = mypow(x,y);
		printf("pow:   %.7e\n",pow(x,y));
            	printf("mypow: %.7e\n",result);
            }
		}
        else if (strcmp("--log",argv[1])==0) {
        		char *x_test, *n_test;
            	double x = strtod(argv[2],&x_test);
            	unsigned int n = strtol(argv[3],&n_test,10);
            	// not valid arguments
            	if (*x_test!='\0' || *n_test!='\0' || n<=0)
	                return WRONG_ARG;
        	    double ln = log(x);
            	printf("       log(%g) = %.12g\n",x,ln);
            	ln = cfrac_log(x,n);
            	printf(" cfrac_log(%g) = %.12g\n",x,ln);
            	ln = taylor_log(x,n);
            	printf("taylor_log(%g) = %.12g\n",x,ln);
        }
        else
            return WRONG_ARG;
    }
    // arguments: --pow X Y N
    else if (argc==5) {
        if (strcmp("--pow",argv[1])==0) {
        	char *x_test, *y_test, *n_test;
            double x = strtod(argv[2],&x_test);
            double y = strtod(argv[3],&y_test);
            long int n = strtol(argv[4],&n_test,10);
            // not valid arguments
            if (*x_test!='\0' || *n_test!='\0' || *y_test!='\0' || n<=0)
                return WRONG_ARG;
            double pw = pow(x,y);
            printf("         pow(%g,%g) = %.12g\n",x,y,pw);
            pw = taylor_pow(x,y,n);
            printf("  taylor_pow(%g,%g) = %.12g\n",x,y,pw);
            pw = taylorcf_pow(x,y,n);
            printf("taylorcf_pow(%g,%g) = %.12g\n",x,y,pw);
        }
        else
            return WRONG_ARG;
    }
    else
        return WRONG_ARG;
    return 0;
}
