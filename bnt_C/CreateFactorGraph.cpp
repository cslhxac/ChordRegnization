#include"FactorGraph.hpp"
#include <mex.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	//prhs[0] is the factor matrix
	//prhs[1] is the array of number of possible values
	//prhs[2] is the list of node names(optional)
	const mwSize *dims0 = mxGetDimensions(prhs[0]);
	const mwSize *dims1 = mxGetDimensions(prhs[1]);

	double* factorM = (double*) mxGetPr(prhs[0]);
	double* nValues = (double*) mxGetPr(prhs[0]);

	mexPrintf("Hello World!\n");
}