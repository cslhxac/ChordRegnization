function [ chordwBass ] = simpleBN(chordClasses,Cb,Ct,keyMaj,keyMin)
%SIMPLEBN Summary of this function goes here
%   Detailed explanation goes here
majSignature = [1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1]';
minSignature = [1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1]';%%The miner can both be harmonic or natual
nValues = [24 size(chordClasses,2) 12];
parentList = cell(1,3);
parentList{1} = [];
parentList{2} = [0];
parentList{3} = [1];

childrenList = cell(1,3);
childrenList{1} = [1];
childrenList{2} = [2];
childrenList{3} = [];

prior = cell(1,3);
prior{1} = zeros(1,24);
prior{1}(keyMaj) = 1;
%prior{1}(12 + keyMin) = 1;

prior{2} = ones(size(chordClasses));

CPD = cell(1,3);
CPD{1} = [];
CPD{2} = zeros(24,size(chordClasses,2));
%Maj CPD
for i = 1:12
    for j = 1:size(chordClasses,2)
        majSignatureOnKey = circshift(majSignature,i - 1);
        CPD{2}(i,j) = majSignatureOnKey' * chordClasses(j).Signature';
    end
end
%Min CPD
for i = 1:12
    for j = 1:size(chordClasses,2)
        minSignatureOnKey = circshift(minSignature,i - 1);
        CPD{2}(i + 12,j) = minSignatureOnKey' * chordClasses(j).Signature';
    end
end
%Chroma CPD
for i = 1:size(chordClasses,2)
    CPD{3}(i,:) = chordClasses(i).Signature;
end
EL = [0 2];

chordwBass = zeros(12,size(Ct,2));
for i = 1:size(Ct,2)
    i
    prior{3} = Ct(:,i);
    BEL = CreateFactorGraph(parentList,childrenList,nValues,prior,CPD,EL,5);
    [C,Index] = max(BEL{2}(:));
    chordwBass(:,i) = chordClasses(Index).Signature;
end

end