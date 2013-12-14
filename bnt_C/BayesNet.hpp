#include<vector>
#include<map>
#include<iostream>
#include<cstring>
#include <mex.h>
#ifndef BAYESNET
#define BAYESNET
#define EPSILON 0.00000001
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
  vector<Node* >* nodesRef;
  string name;
  int nValues;
  int index;
  bool isEvidence;
  vector<double> prior;//For nodes without parents, it is the prior.It is the likelihood for the evidences
  map<vector<int>,double> CPD;
  vector<int> CPDdArray;
  int CPDnD;
  int CPDnElements;
  vector<int> parents;
  vector<int> children;
  vector<double> pi;
  vector<double> lambda;
  vector<double> BEL;
  //The key for the folling funtion is the global index of the node in the Bayes Net.
  map<int,vector<double> > piMSG;
  map<int,vector<double> > lambdaMSG;
  Node(const int n,int index_in,vector<Node* >* nodesRef_in,string name_in = ""):nValues(n),index(index_in),name(name_in),nodesRef(nodesRef_in),isEvidence(false){
    prior.resize(n);
    for(int i = 0;i < n;++i){
      prior[i] = 1;
    }
  }
  void normalizeBEL(){
    double sum1 = 0;
    for(int i = 0;i < nValues;++i){
      sum1 += lambda[i];
    }
    for(int i = 0;i < nValues;++i){
       lambda[i] /= sum1;
    }
    double sum2 = 0;
    for(int i = 0;i < nValues;++i){
      sum2 += pi[i];
    }
    for(int i = 0;i < nValues;++i){
       pi[i] /= sum2;
    }
    double sum3 = 0;
    for(int i = 0;i < nValues;++i){
      sum3 += BEL[i];
    }
    for(int i = 0;i < nValues;++i){
       BEL[i] /= sum3;
    }
  }
  void initializeBeliefs(){
    initializePi();
    initalizeLambda();
    initializeBEL();
  }
  void initializeBEL(){
    BEL.resize(nValues);
    computeBEL();
  }
  void initializePiMSG(){
    for(int i = 0;i < children.size();++i){
      piMSG[children[i]] = prior;
    }  
  }
  void initializeLambdaMSG(){
    for(int i = 0;i < parents.size();++i){
      lambdaMSG[parents[i]] = vector<double>((*nodesRef)[parents[i]] -> nValues);
      lambdaMSG[parents[i]] = (*nodesRef)[parents[i]] -> lambda;
    }
  }
  void initializePi(){
    if(parents.size() == 0 || isEvidence){
      pi = prior;
    }else{
      pi.resize(nValues);
      for(int i = 0;i < nValues;++i){
	      pi[i] = 1;
      }
    }
  }
  void initalizeLambda(){
    if(isEvidence){
      lambda = prior;
    }else{
      lambda.resize(nValues);
      for(int i = 0;i < nValues;++i){
	      lambda[i] = 1;
      }
    }
  }
  void computePi(){
    if(parents.size() == 0 || isEvidence){
      return;
    }
    for(int i = 0;i < nValues;++i){
      pi[i] = 0;
    }
    for(int i = 0;i < CPDnElements;++i){
      vector<int> values = getNDindexFrom1D(CPDnD,CPDdArray,i);
	    double tmp = 1;
      for(int j = 0;j < parents.size();++j){
		    tmp *= (*nodesRef)[parents[j]] -> piMSG[index][values[j]];
      }
	    pi[values.back()] += CPD[values] * tmp;
    }
  }
  void computeLambda(){
    if(isEvidence || children.size() == 0){
      return;
    }
    for(int i = 0;i < nValues;++i){
      lambda[i] = 1; 
    }
    for(int i = 0;i < children.size();++i){
      for(int k = 0;k < nValues;++k){
	      lambda[k] *= (*nodesRef)[children[i]] -> lambdaMSG[index][k];
	    }
    }
  }
  void computeBEL(){
    for(int i = 0;i < nValues;++i){
      BEL[i] = lambda[i] * pi[i];
    }
  }
  void computePiMSG(){
    if(isEvidence){
      return;
    }
    for(int i = 0;i < children.size();++i){
      for(int j = 0;j < nValues;++j){
        if(BEL[j] < EPSILON){
          piMSG[children[i]][j] = 0;
        }else{
          if((*nodesRef)[children[i]] -> lambdaMSG[index][j] < EPSILON){
            mexPrintf("ZERO LAMBDA MSG\n");
          }
          piMSG[children[i]][j] = BEL[j]/((*nodesRef)[children[i]] -> lambdaMSG[index][j]);
        }
      }
    }
  }
  void computeLambdaMSG(){
    for(int i = 0;i < parents.size();++i){
      for(int j = 0;j < (*nodesRef)[parents[i]] -> nValues;++j){
        lambdaMSG[parents[i]][j] = 0;
      }
    }
	  for(int i = 0;i < parents.size();++i){
      //the iterations here assumes that the current node is the last dimension in the CPD therefore will be iterated last.
      vector<double> tmpSum((*nodesRef)[parents[i]] -> nValues);
      for(int j = 0;j < (*nodesRef)[parents[i]] -> nValues;++j){
        tmpSum[j] = 0;
      }
      for(int j = 0;j < CPDnElements;++j){
        vector<int> values = getNDindexFrom1D(CPDnD,CPDdArray,j);
        double tmpProd = 1;
        for(int k = 0;k < parents.size();++k){
          if(k != i){
            tmpProd *= (*nodesRef)[parents[k]] -> piMSG[index][values[k]];
          }
        }
        tmpSum[values[i]] += CPD[values] * tmpProd;
        vector<int> valuesNext;
        if(j < CPDnElements - 1){
          valuesNext = getNDindexFrom1D(CPDnD,CPDdArray,j + 1);
        }
        if(j == CPDnElements - 1 || valuesNext.back() != values.back()){
          //mexPrintf("J change %i, for node %i\n",j,index);
          //change of value of the current node.
          for(int k = 0;k < (*nodesRef)[parents[i]] -> nValues;++k){
            lambdaMSG[parents[i]][k] += lambda[values.back()] * tmpSum[k];
            tmpSum[k] = 0;
          }
        }
      }
	  }
  }
};
class BayesNet{
public:
  BayesNet(const vector<vector<int> >& parentList,const vector<vector<int> >& childrenList,const vector<int>& nValues){
    nodes.resize(nValues.size());
    int itr;
    for(itr = 0;itr < nValues.size();++itr){
      nodes[itr] = new Node(nValues[itr],itr,&nodes);
      nodes[itr] -> parents = parentList[itr];
      nodes[itr] -> children = childrenList[itr];
    }
  };
  ~BayesNet(){
    for(int i = 0;i < nodes.size();++i){
      delete nodes[i];
    }
  };
  void beliefPropagation(int maxItr){
    for(int i = 0;i < nodes.size();++i){
      nodes[i] -> initializeBeliefs();
    }
    for(int i = 0;i < nodes.size();++i){
      nodes[i] -> initializeLambdaMSG();
    }
    for(int i = 0;i < nodes.size();++i){
      nodes[i] -> initializePiMSG();
      //nodes[i] -> computePiMSG();
    }
    //mexPrintf("here!!!\n");
    //mexPrintf("(%i)\n",nodes[3] -> piMSG[2].size());
    printBPState();
    //return;
    for(int itr = 0;itr < maxItr;++itr){
      for(int i = 0;i < nodes.size();++i){
        nodes[i] -> computePi();
        nodes[i] -> computeLambda();
        nodes[i] -> computeBEL();
      }
      //printBPState();
      for(int i = 0;i < nodes.size();++i){
        nodes[i] -> computePiMSG();
      }
      for(int i = 0;i < nodes.size();++i){
        nodes[i] -> computeLambdaMSG();
      }
    }
    for(int i = 0;i < nodes.size();++i){
      nodes[i] -> normalizeBEL();
    }   
    printBPState();
  }
  void setEvidenceList(const vector<int>& evidences){
    for(int i = 0;i < evidences.size();++i){
      nodes[evidences[i]] -> isEvidence = true;
    }
  }
  void setNodeLikelihood(const vector<vector<double> >& prior){
    for(int i = 0;i < prior.size();++i){
      nodes[i] -> prior = prior[i];
    }
  }
  void setCPD(int nodeID,const map<vector<int>,double>& CPD,int nD,vector<int> d,int nElements){
    nodes[nodeID] -> CPD = CPD;
    nodes[nodeID] -> CPDnD = nD;
    nodes[nodeID] -> CPDdArray = d;
    nodes[nodeID] -> CPDnElements = nElements;
  }
  const Node* getNode(const int index)const{return nodes[index];};
  int getnNodes()const{return nodes.size();};
  const void print()const{
    for(int i = 0;i < nodes.size();++i){
      mexPrintf("Node %i:\n",i);
      mexPrintf("\tParentList:\n");
      for(int j = 0;j < nodes[i] -> parents.size();++j){
	      mexPrintf("\t\tNode %i\n",nodes[i] -> parents[j]);
      }
      mexPrintf("\n");
      mexPrintf("\tChildrenList:\n");
      for(int j = 0;j < nodes[i] -> children.size();++j){
	      mexPrintf("\t\tNode %i\n",nodes[i] -> children[j]);
      } 
      mexPrintf("\n");
    }
    mexPrintf("Likelihood\n");
    for(int i = 0;i < nodes.size();++i){
      mexPrintf("Node %i Likelihood: ",i);
      for(int j = 0;j < nodes[i]->prior.size();++j){
	      mexPrintf("%f ",nodes[i] -> prior[j]);
      }
      mexPrintf("\n");
    }
    mexPrintf("CPD\n");
    for(int i = 0;i < nodes.size();++i){
      if(nodes[i] -> parents.size() > 0){
        mexPrintf("node %i CPD: \n",i);
        int nElements = 1;
        vector<int> d;
        d.resize(nodes[i] -> parents.size() + 1);
        for(int j = 0;j < nodes[i] -> parents.size();++j){
	        nElements *= nodes[nodes[i] -> parents[j]] -> nValues;
	        d[j] = nodes[nodes[i] -> parents[j]] -> nValues;
        }
	      nElements *= nodes[i] -> nValues;
	      d[nodes[i] -> parents.size()] = nodes[i] -> nValues;
        for(int j = 0;j < nElements;++j){
	        vector<int> indices = getNDindexFrom1D(nodes[i] -> parents.size() + 1,d,j);
	        mexPrintf("<");
	        for(int k = 0;k < indices.size();++k){
	          mexPrintf("%i ",indices[k]);
	        }
	        mexPrintf(">: %f\n",nodes[i] -> CPD[indices]);
        }
      }
    }
  }

  void printBPState(){
    mexPrintf("BPState!!!\n");
    for(int i = 0;i < nodes.size();++i){
      mexPrintf("Node %i:\n",i);
      mexPrintf("\tPi :(");
      for(int j = 0;j < nodes[i] -> nValues;++j){
        mexPrintf("%f,",nodes[i] -> pi[j]);
      }
      mexPrintf("\b)\n");
      mexPrintf("\tLambda :(");
      for(int j = 0;j < nodes[i] -> nValues;++j){
        mexPrintf("%f,",nodes[i] -> lambda[j]);
      }
      mexPrintf("\b)\n");
      mexPrintf("\tBEL :(");
      for(int j = 0;j < nodes[i] -> nValues;++j){
        mexPrintf("%f,",nodes[i] -> BEL[j]);
      }
      mexPrintf("\b)\n");
      mexPrintf("\tPiMSG:\n");
      for(int k = 0;k < nodes[i] -> children.size();++k){
        mexPrintf("\t\tPi %i:(",nodes[i] -> children[k]);
        for(int j = 0;j < nodes[i] -> nValues;++j){
          mexPrintf("%f,",nodes[i] -> piMSG[nodes[i] -> children[k]][j]);
        }
        mexPrintf("\b)\n");
      }
      mexPrintf("\tLambdaMSG:\n");
      for(int k = 0;k < nodes[i] -> parents.size();++k){
        mexPrintf("\t\tLambda %i:(",nodes[i] -> parents[k]);
        for(int j = 0;j < nodes[nodes[i] -> parents[k]] -> nValues;++j){
          mexPrintf("%f,",nodes[i] -> lambdaMSG[nodes[i] -> parents[k]][j]);
        }
        mexPrintf("\b)\n");
      }
    }
  }
private:
  vector<Node*> nodes;
  void resetPrior(){
    for(int i = 0;i < nodes.size();++i){
      if(!nodes[i] -> isEvidence){
	      nodes[i] -> prior.assign(nodes[i] -> prior.size(),1);
      }
    }
  }
};
#endif
