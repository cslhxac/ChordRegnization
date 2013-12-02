#include<vector>
#include<iostream>
#include<cstring>
#ifndef FACTORGRAPH
#define FACTORGRAPH
using namespace std;
struct Node{
	string name;
    int nValues;
    vector<double> likelihood;
    Node(int n,string name_in = ""):nValues(n),name(name_in){
        likelihood.resize(n);
		for(vector<double>::iterator i = likelihood.begin();i != likelihood.end();++i){
			*i = 1;
		}
    }
};
struct Factor{
	int nNode;
	vector<Node*> nodes;
	vector<double> tendency;
	Factor(std::vector<Node*> & nodes_in):nodes(nodes_in){
		int N = 1;
		for(vector<Node*>::iterator i = nodes.begin();i != nodes.end();++i){
			N *= (*i) -> nValues;
		}
	}
	double getValue(const vector<int> values)const{
		if(values.size != nodes.size){
			cerr << "Factor::getValue: index, node size mismatch!" << endl;
			return 0;
		}
		int index = 0;
		vector<Node*>::const_iterator i = nodes.begin();
		vector<int>::const_iterator j = values.begin();
		index = *j;
		for(++j;j != values.end();++i,++j){
			index = index * (*i) -> nValues + *j;
		}
		return tendency[index];
	}
	void setValue(const vector<int> values,double v){
		if(values.size != nodes.size){
			cerr << "Factor::getValue: index, node size mismatch!" << endl;
			return;
		}
		int index = 0;
		vector<Node*>::const_iterator i = nodes.begin();
		vector<int>::const_iterator j = values.begin();
		index = *j;
		for(++j;j != values.end();++i,++j){
			index = index * (*i) -> nValues + *j;
		}
		tendency[index] = v;
	}
};
class FactorGraph{
public:
	FactorGraph(int* factorM,int* nValues,int n,int nfactor){
		nodes.resize(n);
		for(int i = 0;i < n;++i){
			nodes[i] = new Node(nValues[i]);
		}
	};
	~FactorGraph(){};
private:
	vector<Node*> nodes;
	vector<Factor*> factors;
    
};
#endif
