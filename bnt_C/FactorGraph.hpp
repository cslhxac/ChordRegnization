#include<vector>
#include<map>
#include<iostream>
#include<cstring>
#include <mex.h>
#ifndef FACTORGRAPH
#define FACTORGRAPH
using namespace std;
int get1DindexFromND(const mwSize nDimensions,const mwSize* d,const vector<int> indices){
	int tmp = indices[nDimensions - 1];
	for(int i = (nDimensions - 2);i >= 0;--i){
		tmp = tmp * d[i] + indices[i];
	}
	return tmp;
}
int get1DindexFromND(const int nDimensions,const vector<int> d,const vector<int> indices){
	int tmp = indices[nDimensions - 1];
	for(int i = (nDimensions - 2);i >= 0;--i){
		tmp = tmp * d[i] + indices[i];
	}
	return tmp;
}
vector<int> getNDindexFrom1D(const mwSize nDimensions,const mwSize* d,const int indices){
	vector<int> indicesT;
	indicesT.resize(nDimensions);
	int tmp = indices;
	for(int i = 0;i < nDimensions;++i){
		indicesT[i] = tmp%d[i];
		tmp /= d[i];
	}
	return indicesT;
}
vector<int> getNDindexFrom1D(const int nDimensions,const vector<int> d,const int indices){
	vector<int> indicesT;
	indicesT.resize(nDimensions);
	int tmp = indices;
	for(int i = 0;i < nDimensions;++i){
		indicesT[i] = tmp%d[i];
		tmp /= d[i];
	}
	return indicesT;
}
struct Node{
	string name;
    int nValues;
	bool isEvidence;
    vector<double> prior;//For unobserved nodes, it is the prior.It is the likelihood for the evidences
	vector<double> MSG;
	vector<double> MSGNext;
    Node(int n,string name_in = ""):nValues(n),name(name_in),isEvidence(false){
        prior.resize(n);
		MSG.resize(n);
		MSGNext.resize(n);
		for(int i = 0;i < n;++i){
			prior[i] = 1;
		}
    }
};
struct Factor{
	vector<int> nodes;
	map<vector<int>,double> tendency;
	Factor(const vector<int> & nodes_in):nodes(nodes_in){
	}
	double getValue(const vector<int> values){
		if(values.size() != nodes.size()){
			cerr << "Factor::getValue: index, node size mismatch!" << endl;
			return 0;
		}
		return tendency[values];
	}
	void setValue(const vector<int> values,double v){
		if(values.size() != nodes.size()){
			cerr << "Factor::getValue: index, node size mismatch!" << endl;
			return;
		}
		tendency[values] = v;
	}
};
class FactorGraph{
public:
	FactorGraph(const vector<vector<int>>& factorM,const vector<int>& nValues){
		nodes.resize(nValues.size());
		int itr;
		for(itr = 0;itr < nValues.size();++itr){
			nodes[itr] = new Node(nValues[itr]);
		}
		factors.resize(factorM.size());
		for(itr = 0;itr < factorM.size();++itr){
			factors[itr] = new Factor(factorM[itr]);
		}
	};
	~FactorGraph(){
		for(int i = 0;i < nodes.size();++i){
			delete nodes[i];
		}
		for(int i = 0;i < factors.size();++i){
			delete factors[i];
		}
	};
	void setEvidenceList(const vector<int>& evidences){
		for(int i = 0;i < evidences.size();++i){
			nodes[evidences[i]] -> isEvidence = true;
		}
	}
	void setNodeLikelihood(const vector<vector<double>>& prior){
		for(int i = 0;i < prior.size();++i){
			nodes[i] -> prior = prior[i];
		}
	}
	void setFactorTendency(int factorID,const map<vector<int>,double>& tendency){
		factors[factorID] -> tendency = tendency;
	}
	const Node* getNode(const int index)const{return nodes[index];};
	const Factor* getFactor(const int index)const{return factors[index];};
	int getnFactors()const{return factors.size();};
	int getnNodes()const{return nodes.size();};
	const void print()const{
		for(int i = 0;i < factors.size();++i){
			mexPrintf("Factor %i:\n",i);
			for(int j = 0;j < factors[i] -> nodes.size();++j){
				if(nodes[factors[i] -> nodes[j]] -> isEvidence){
					mexPrintf("\tNode %i: %i evidence\n",factors[i] -> nodes[j],nodes[factors[i] -> nodes[j]] -> nValues);
				}else{
					mexPrintf("\tNode %i: %i\n",factors[i] -> nodes[j],nodes[factors[i] -> nodes[j]] -> nValues);
				}
			}
		}
		mexPrintf("Likelihood\n");
		for(int i = 0;i < nodes.size();++i){
			mexPrintf("Node %i Likelihood: ",i);
			for(int j = 0;j < nodes[i]->prior.size();++j){
				mexPrintf("%f ",nodes[i]->prior[j]);
			}
			mexPrintf("\n");
		}
		mexPrintf("Factor Tendency\n");
		for(int i = 0;i < factors.size();++i){
			mexPrintf("Factor %i Tendency: \n",i);
			int nElements = 1;
			vector<int> d;
			d.resize(factors[i] -> nodes.size());
			for(int j = 0;j < factors[i] -> nodes.size();++j){
				nElements *= nodes[factors[i] -> nodes[j]] -> nValues;
				d[j] = nodes[factors[i] -> nodes[j]] -> nValues;
			}
			for(int j = 0;j < nElements;++j){
				vector<int> indices = getNDindexFrom1D(factors[i] -> nodes.size(),d,j);
				mexPrintf("<");
				for(int k = 0;k < indices.size();++k){
					mexPrintf("%i ",indices[k]);
				}
				mexPrintf(">: %f\n",factors[i] -> tendency[indices]);
			}
		}
	}
	const int maxItr = 10;
	void beliefPropagation(){
		for(int i = 0;i < maxItr;++i){
			for(int j = 0){
				
			}
		}
	};
private:
	vector<Node*> nodes;
	vector<Factor*> factors;
    void initialingMSG(){
		for(int i = 0;i < nodes.size();++i){
			nodes[i] -> MSG = nodes[i] -> prior;
		}
	};
	void resetMSGNext(){
		for(int i = 0;i < nodes.size();++i){
			nodes[i] -> MSG = nodes[i] -> MSGNext;
			nodes[i] -> MSGNext.assign(nodes[i] -> MSGNext.size(),1);
		}
	};
	void resetPrior(){
		for(int i = 0;i < nodes.size();++i){
			if(!nodes[i] -> isEvidence){
				nodes[i] -> prior.assign(nodes[i] -> prior.size(),1);
			}
		}
	}
};
#endif
