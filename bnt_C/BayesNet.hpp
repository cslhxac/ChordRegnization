#include<vector>
#include<map>
#include<iostream>
#include<cstring>
#include <mex.h>
#ifndef BAYESNET
#define BAYESNET
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
  vector<double> prior;//For nodes without parents, it is the prior.It is the likelihood for the evidences
  map<vector<int>,double> CPD;
  vector<Node*> parents;
  vector<Node*> children;
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
class BaeysNet{
public:
  BayesNet(const vector<vector<int>>& parentList,const vector<vector<int>>& childrenList,const vector<int>& nValues){
    nodes.resize(nValues.size());
    int itr;
    for(itr = 0;itr < nValues.size();++itr){
      nodes[itr] = new Node(nValues[itr]);
      nodes[itr] -> parents = parentList[itr];
      nodes[itr] -> children = childrenList[itr];
    }
  };
  ~BayesNet(){
    for(int i = 0;i < nodes.size();++i){
      delete nodes[i];
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
  void setCPD(int nodeID,const map<vector<int>,double>& CPD){
    nodes[nodeID] -> CPD = CPD;
  }
  const Node* getNode(const int index)const{return nodes[index];};
  int getnNodes()const{return nodes.size();};
  const void print()const{
    for(int i = 0;i < nodes.size();++i){
      mexPrintf("Node %i:\n",i);
      mexPrintf("\tParentList:");
      for(int j = 0;j < nodes[i] -> parentList.size();++j){
	mexPrintf("\t\tNode %i\n",nodes[i] -> parents[j]);
      }
      mexPrintf("\tChildrenList:");
      for(int j = 0;j < nodes[i] -> childrenList.size();++j){
	mexPrintf("\t\tNode %i\n",nodes[i] -> children[j]);
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
    mexPrintf("CPD\n");
    for(int i = 0;i < nodes.size();++i){
      mexPrintf("node %i CPD: \n",i);
      int nElements = 1;
      vector<int> d;
      d.resize(nodes[i] -> parentList.size());
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
