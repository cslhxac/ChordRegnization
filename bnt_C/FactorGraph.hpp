#include<vector>
#include<map>
#include<iostream>
#include<cstring>
#include <mex.h>
#ifndef FACTORGRAPH
#define FACTORGRAPH
using namespace std;
struct Node{
	string name;
    int nValues;
	bool isEvidence;
    vector<double> likelihood;
    Node(int n,string name_in = ""):nValues(n),name(name_in){
        likelihood.resize(n);
		for(int i = 0;i < n;++i){
			likelihood[i] = 0;
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
	void setEvidenceList(vector<int> evidences){
		for(int i = 0;i < evidences.size();++i){
			nodes[evidences[i]] -> isEvidence = true;
		}
	}
	void setNodeLikelihood(const vector<vector<double>>& likelihood){
		for(int i = 0;i < likelihood.size();++i){
			nodes[i] -> likelihood = likelihood[i];
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
				mexPrintf("\tNode %i: %i\n",factors[i] -> nodes[j],nodes[factors[i] -> nodes[j]] -> nValues);
			}
		}
	}
private:
	vector<Node*> nodes;
	vector<Factor*> factors;
    
};
#endif
