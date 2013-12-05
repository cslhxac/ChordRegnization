#include"FactorGraph.hpp"
#include <mex.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	//prhs[0] is the factor matrix cell array
	//prhs[1] is the array of number of possible values
	//Make prhs[0] and prhs[1] DOUBLE arrays!!!!
	#define FACTORMATRIX prhs[0]
	#define NODESNVALUES prhs[1]
	//NODELIKELIHOOD is a cell array
	#define NODELIKELIHOOD prhs[2]
	//FACTORTENDENCY is a cell array
	#define FACTORTENDENCY prhs[3]
	//prhs[4] is the list of node names(optional)
	const mwSize *dims0 = mxGetDimensions(prhs[0]);
	const mwSize *dims1 = mxGetDimensions(prhs[1]);
	int nFactors = mxGetNumberOfElements(prhs[0]);
	vector<vector<double>> factorM;
	vector<vector<int>> factorMt;
	factorM.resize(nFactors);
	factorMt.resize(nFactors);
	for(int i = 0;i < nFactors;i++){
		mwSize n = mxGetN(mxGetCell(prhs[0],i));
		mwSize m = mxGetM(mxGetCell(prhs[0],i));
		mwSize tsize = n*m;
		factorM[i] = vector<double>((double*) mxGetPr(mxGetCell(prhs[0],i)),(double*) mxGetPr(mxGetCell(prhs[0],i)) + tsize);
		factorMt[i].resize(factorM[i].size());
		for(int j = 0;j < factorM[i].size();++j){
			factorMt[i][j] = factorM[i][j];
		}
		//mexPrintf("Factor %i: %i,%i\n",i,mxGetN(mxGetCell(prhs[0],i)),mxGetM(mxGetCell(prhs[0],i)));
		//assert(mxGetM(mxGetCell(prhs[0],i)) == 1);
	}
	mwSize n = mxGetN(prhs[1]);
	mwSize m = mxGetM(prhs[1]);
	mwSize tsize = n*m;
	vector<double> nValues = vector<double>((double*) mxGetPr(prhs[1]),(double*) mxGetPr(prhs[1]) + tsize);
	vector<int> nValuest;
	nValuest.resize(nValues.size());
	for(int i = 0;i < nValues.size();i++){
		nValuest[i] = nValues[i];
	}
	//mexPrintf("n,m: %i,%i\n",n,m);

	//mexPrintf("factorM,nValues: %i,%i\n",factorM.size(),nValues.size());
	//assert(m == 1);
	FactorGraph* FG = new FactorGraph(factorMt,nValuest);
	//Get the Likelihood
	int nNodes = mxGetNumberOfElements(NODELIKELIHOOD);
	vector<vector<double>> nodeLikelihoodV;
	nodeLikelihoodV.resize(nNodes);
	for(int i = 0;i < nNodes;i++){
		mwSize n = mxGetN(mxGetCell(NODELIKELIHOOD,i));
		mwSize m = mxGetM(mxGetCell(NODELIKELIHOOD,i));
		mwSize tsize = n*m;
		nodeLikelihoodV[i] = vector<double>((double*) mxGetPr(mxGetCell(NODELIKELIHOOD,i)),(double*) mxGetPr(mxGetCell(NODELIKELIHOOD,i)) + tsize);
	}
	FG -> setNodeLikelihood(nodeLikelihoodV);
	//Get the Tendency
	int nTendency = mxGetNumberOfElements(FACTORTENDENCY);
	if(nTendency != nFactors){
		mexPrintf("Tendency Factor size mismatch");
		return;
	}
	for(int i = 0;i < nTendency;i++){
		map<vector<int>,double> tendencyMap;
		mwSize nDimensions = mxGetNumberOfDimensions(mxGetCell(FACTORTENDENCY,i));
		const mwSize* d = mxGetDimensions(mxGetCell(FACTORTENDENCY,i));
		if(nDimensions != FG -> getFactor(i) -> nodes.size()){
			mexPrintf("Tendency Factor nodes size mismatch");
			return;
		}
		int nElements = 1;
		for(int j = 0;j < nDimensions;++j){
			nElements *= d[j];
		}
		for(int j = 0;j < nElements;++j){
			tendencyMap[getNDindexFrom1D(nDimensions,d,j)] = mxGetPr(mxGetCell(FACTORTENDENCY,i))[j];
		}
		FG -> setFactorTendency(i,tendencyMap);
	}
	FG -> print();
}