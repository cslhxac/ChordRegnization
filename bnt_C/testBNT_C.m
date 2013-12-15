load('matlab.mat')
mex CreateFactorGraph.cpp
BEL = CreateFactorGraph(parentList,childrenList,nValues,prior,CPD,EL,4);
