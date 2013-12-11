#include"BayesNet.hpp"
#include <mex.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	//prhs[0] is the factor matrix cell array
	//prhs[1] is the array of number of possible values
	//Make prhs[0] and prhs[1] DOUBLE arrays!!!!
	//EVERY INDEX IN THOUSE MATRICES ARE STARTED FROM 0!!!!!!!!!!!!!
    #define PARENTLIST prhs[0]
    #define CHILDRENLIST prhs[1]
	#define NODESNVALUES prhs[2]
	//NODELIKELIHOOD is a cell array
	#define NODELIKELIHOOD prhs[3]
	//FACTORTENDENCY is a cell array
	#define CPDIN prhs[4]
	#define EVIDENCELIST prhs[5]
	//prhs[4] is the list of node names(optional)
	int nParents = mxGetNumberOfElements(PARENTLIST);
	vector<vector<double> > parentListD;
	vector<vector<int> > parentListI;
	parentListD.resize(nParents);
	parentListI.resize(nParents);
	for(int i = 0;i < nParents;i++){
		mwSize n = mxGetN(mxGetCell(PARENTLIST,i));
		mwSize m = mxGetM(mxGetCell(PARENTLIST,i));
		mwSize tsize = n*m;
		if(tsize > 0){
			parentListD[i] = vector<double>((double*) mxGetPr(mxGetCell(PARENTLIST,i)),(double*) mxGetPr(mxGetCell(PARENTLIST,i)) + tsize);
			parentListI[i].resize(parentListD[i].size());
			for(int j = 0;j < parentListD[i].size();++j){
				parentListI[i][j] = parentListD[i][j];
			}
		}
		//mexPrintf("Factor %i: %i,%i\n",i,mxGetN(mxGetCell(prhs[0],i)),mxGetM(mxGetCell(prhs[0],i)));
		//assert(mxGetM(mxGetCell(prhs[0],i)) == 1);
	}
    int nChildren = mxGetNumberOfElements(CHILDRENLIST);
    vector<vector<double> > childrenListD;
    vector<vector<int> > childrenListI;
    childrenListD.resize(nChildren);
    childrenListI.resize(nChildren);
    for(int i = 0;i < nChildren;i++){
	  mwSize n = mxGetN(mxGetCell(CHILDRENLIST,i));
	  mwSize m = mxGetM(mxGetCell(CHILDRENLIST,i));
	  mwSize tsize = n*m;
	  if(tsize > 0){
		 childrenListD[i] = vector<double>((double*) mxGetPr(mxGetCell(CHILDRENLIST,i)),(double*) mxGetPr(mxGetCell(CHILDRENLIST,i)) + tsize);
		 childrenListI[i].resize(childrenListD[i].size());
		 for(int j = 0;j < childrenListD[i].size();++j){
			childrenListI[i][j] = childrenListD[i][j];
		}
	  }
	  //mexPrintf("Factor %i: %i,%i\n",i,mxGetN(mxGetCell(prhs[0],i)),mxGetM(mxGetCell(prhs[0],i)));                    
	  //assert(mxGetM(mxGetCell(prhs[0],i)) == 1);                                                                      
	}
	mwSize n = mxGetN(NODESNVALUES);
	mwSize m = mxGetM(NODESNVALUES);
	mwSize tsize = n*m;
	vector<double> nValues = vector<double>((double*) mxGetPr(NODESNVALUES),(double*) mxGetPr(NODESNVALUES) + tsize);
	vector<int> nValuest;
	nValuest.resize(nValues.size());
	for(int i = 0;i < nValues.size();i++){
		nValuest[i] = nValues[i];
	}
	//mexPrintf("n,m: %i,%i\n",n,m);

	//mexPrintf("factorM,nValues: %i,%i\n",factorM.size(),nValues.size());
	//assert(m == 1);
	BayesNet* BN = new BayesNet(parentListI,childrenListI,nValuest);
	//Get the Likelihood
	int nNodes = mxGetNumberOfElements(NODELIKELIHOOD);
	vector<vector<double> > nodeLikelihoodV;
	nodeLikelihoodV.resize(nNodes);
	for(int i = 0;i < nNodes;i++){
		mwSize n = mxGetN(mxGetCell(NODELIKELIHOOD,i));
		mwSize m = mxGetM(mxGetCell(NODELIKELIHOOD,i));
		mwSize tsize = n*m;
		nodeLikelihoodV[i] = vector<double>((double*) mxGetPr(mxGetCell(NODELIKELIHOOD,i)),(double*) mxGetPr(mxGetCell(NODELIKELIHOOD,i)) + tsize);
	}
	BN -> setNodeLikelihood(nodeLikelihoodV);
	//Get the CPD
	int nCPD = mxGetNumberOfElements(CPDIN);
	if(nCPD != nParents){
		mexPrintf("CPD nodes size mismatch");
		return;
	}
	for(int i = 0;i < nCPD;i++){
		map<vector<int>,double> CPDMap;
		mwSize nDimensions = mxGetNumberOfDimensions(mxGetCell(CPDIN,i));
		const mwSize* d = mxGetDimensions(mxGetCell(CPDIN,i));
		//mexPrintf("node %i: %i %i %i %i\n",i,BN -> getNode(i) -> parents.size(),nDimensions,d[0],d[1]);
		if(nDimensions != 0 && d[0] != 0 && nDimensions != BN -> getNode(i) -> parents.size() + 1){
			mexPrintf("CPD node parent size mismatch\n");
			return;
		}
		int nElements = 1;
		vector<int> dV;
		dv.resize(nDimensions);
		for(int j = 0;j < nDimensions;++j){
		  nElements *= d[j];
		  dv[j] = d[j];
		}	
		for(int j = 0;j < nElements;++j){
		  CPDMap[getNDindexFrom1D(nDimensions,d,j)] = mxGetPr(mxGetCell(CPDIN,i))[j];
		}
		BN -> setCPD(i,CPDMap,nDimensions,dv,nElements);
	}
	mwSize nEvidence = mxGetNumberOfElements(EVIDENCELIST);
	double* evidenveListPr = mxGetPr(EVIDENCELIST);
	vector<int> evidenveList;
	evidenveList.resize(nEvidence);
	for(int i = 0;i < nEvidence;++i){
		evidenveList[i] = evidenveListPr[i];
	}
	BN -> setEvidenceList(evidenveList);
	BN -> print();
}
