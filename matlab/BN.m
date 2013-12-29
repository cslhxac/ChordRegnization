function [ chordNames chord bass ] = BN(chordClasses,Cb,Cb_max,Ct,keyMaj,keyMin)
%parameters
delta = 0.1;
epsilon = 0.01;
a = [0.5,0.1,0.4,0.1];
%SIMPLEBN my_summary of this function goes here
%   Detailed explanation goes here
majSignature = [1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1]';
minSignature = [1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1]';%%The miner can both be harmonic or natual

majSignatureOnKey = circshift(majSignature,keyMaj - 1);
minSignatureOnKey = circshift(minSignature,keyMin - 1);   

tSlice = size(Ct,2);
nValues = zeros(1,tSlice * 6);
parentList = cell(1,tSlice * 6);
childrenList = cell(1,tSlice * 6);
prior = cell(1,tSlice * 6);
CPD = cell(1,tSlice * 6);
EL = zeros(1, 2 * tSlice);
%The key parameter can be either major or minor
% node i * 6 is the key
% node i * 6 + 1 is beat(i)
% node i * 6 + 2 is chord(i)
% node i * 6 + 3 is bass(i)
% node i * 6 + 4 is chord chroma(i)
% node i * 6 + 5 is bass chroma(i)

%Chroma CPD
chromaCPD = zeros(size(chordClasses,2),12);
for i = 1:size(chordClasses,2)
    chromaCPD(i,:) = chordClasses(i).Signature/3;
end
%bass CPD
bassCPD = zeros(size(chordClasses,2),12);
for i = 1:size(chordClasses,2)
	bassCPD(i,:) = chordClasses(i).Signature * 0.8;
    for j = 1:12
        if(bassCPD(i,j) == 0)
            bassCPD(i,j) = 0.2/8;
        end
    end
    my_sum = 0;
    for j = 1:12
        my_sum = my_sum + bassCPD(i,j);
    end
    for j = 1:12
        bassCPD(i,j) = bassCPD(i,j) / my_sum;
    end
end
%bass chroma CPD
bassChromaCPD = eye(12);
%chord progression matrix
%M(A,B) indicates an edge from A to B
chordProgressionMatrixMaj = zeros(size(chordClasses,2),size(chordClasses,2));
chordProgressionMatrixMin = zeros(size(chordClasses,2),size(chordClasses,2));
%maj
I = keyMaj;
ii = mod(keyMaj + 1,12) + 13;
iii = mod(keyMaj + 3,12) + 13;
IV = mod(keyMaj + 4,12) + 1;
V = mod(keyMaj + 6,12) + 1;
vi = mod(keyMaj + 8,12) + 13;
vii = mod(keyMaj + 10,12) + 13;
VIIb = mod(keyMaj + 9,12) + 1;
viidim = mod(keyMaj + 10,12) + 25;

chordProgressionMatrixMaj(I,ii) = 1;
chordProgressionMatrixMaj(I,iii) = 1;
chordProgressionMatrixMaj(I,IV) = 1;
chordProgressionMatrixMaj(I,V) = 1;
chordProgressionMatrixMaj(I,vi) = 1;
chordProgressionMatrixMaj(I,vii) = 1;
chordProgressionMatrixMaj(I,VIIb) = 1;
chordProgressionMatrixMaj(I,viidim) = 1;

chordProgressionMatrixMaj(ii,V) = 1;
chordProgressionMatrixMaj(ii,vii) = 1;
chordProgressionMatrixMaj(ii,VIIb) = 1;
chordProgressionMatrixMaj(ii,viidim) = 1;

chordProgressionMatrixMaj(iii,vi) = 1;
chordProgressionMatrixMaj(iii,IV) = 1;

chordProgressionMatrixMaj(IV,I) = 1;
chordProgressionMatrixMaj(IV,ii) = 1;
chordProgressionMatrixMaj(IV,V) = 1;
chordProgressionMatrixMaj(IV,vii) = 1;
chordProgressionMatrixMaj(IV,VIIb) = 1;
chordProgressionMatrixMaj(IV,viidim) = 1;

chordProgressionMatrixMaj(V,I) = 1;
chordProgressionMatrixMaj(V,vi) = 1;

chordProgressionMatrixMaj(vi,IV) = 1;
chordProgressionMatrixMaj(vi,ii) = 1;
chordProgressionMatrixMaj(vi,V) = 1;

chordProgressionMatrixMaj(vii,V) = 1;
chordProgressionMatrixMaj(VIIb,V) = 1;
chordProgressionMatrixMaj(viidim,V) = 1;
chordProgressionMatrixMaj(vii,I) = 1;
chordProgressionMatrixMaj(VIIb,I) = 1;
chordProgressionMatrixMaj(viidim,I) = 1;
%min
i = keyMin + 12;
ii = mod(keyMin + 1,12) + 13;
IIb = mod(keyMin,12) + 1;
iidim = mod(keyMin + 1,12) + 25;
III = mod(keyMin + 2,12) + 1;
iv = mod(keyMin + 4,12) + 13;
v = mod(keyMin + 6,12) + 13;
VI = mod(keyMin + 7,12) + 1;
VII = mod(keyMin + 9,12) + 1;

chordProgressionMatrixMin(I,ii) = 1;
chordProgressionMatrixMin(I,IIb) = 1;
chordProgressionMatrixMin(I,iidim) = 1;
chordProgressionMatrixMin(I,III) = 1;
chordProgressionMatrixMin(I,iv) = 1;
chordProgressionMatrixMin(I,v) = 1;
chordProgressionMatrixMin(I,VI) = 1;
chordProgressionMatrixMin(I,VII) = 1;

chordProgressionMatrixMin(ii,v) = 1;
chordProgressionMatrixMin(IIb,v) = 1;
chordProgressionMatrixMin(iidim,v) = 1;
chordProgressionMatrixMin(ii,VII) = 1;
chordProgressionMatrixMin(IIb,VII) = 1;
chordProgressionMatrixMin(iidim,VII) = 1;

chordProgressionMatrixMin(III,iv) = 1;
chordProgressionMatrixMin(III,VI) = 1;

chordProgressionMatrixMin(iv,I) = 1;
chordProgressionMatrixMin(iv,iidim) = 1;
chordProgressionMatrixMin(iv,ii) = 1;
chordProgressionMatrixMin(iv,IIb) = 1;
chordProgressionMatrixMin(iv,V) = 1;
chordProgressionMatrixMin(iv,VII) = 1;

chordProgressionMatrixMin(v,i) = 1;
chordProgressionMatrixMin(v,VI) = 1;

chordProgressionMatrixMin(VI,iv) = 1;
chordProgressionMatrixMin(VI,V) = 1;
chordProgressionMatrixMin(VI,ii) = 1;
chordProgressionMatrixMin(VI,IIb) = 1;
chordProgressionMatrixMin(VI,iidim) = 1;

chordProgressionMatrixMin(VII,III) = 1;
chordProgressionMatrixMin(VII,V) = 1;
chordProgressionMatrixMin(VII,i) = 1;
%chord CPD
simpleChordCPD = zeros(2,4,size(chordClasses,2));
for i = 1:size(chordClasses,2)
    for k = 1:4
        simpleChordCPD(1,k,i) = chordClasses(i).Signature * majSignatureOnKey;
        simpleChordCPD(2,k,i) = chordClasses(i).Signature * minSignatureOnKey;
    end
end
for i = 1:2
	for j = 1:4
        my_sum = 0;
        for k = 1:size(chordClasses,2)
            my_sum = my_sum + simpleChordCPD(i,j,k);
        end
        for k = 1:size(chordClasses,2)
            simpleChordCPD(i,j,k) = simpleChordCPD(i,j,k) / my_sum;
        end
    end
end
chordCPD = zeros(2,4,size(chordClasses,2),size(chordClasses,2));

for i = 1:size(chordClasses,2)
    if(max(chordProgressionMatrixMaj(i,:)) == 0)
        for j = 1:4
            for k = 1:size(chordClasses,2)
                chordCPD(1,j,i,k) = chordClasses(k).Signature * majSignatureOnKey;
            end
        end
    else
        for j = 1:size(chordClasses,2)
            if(chordProgressionMatrixMaj(i,j) > 0)
                for k = 1:4
                    chordCPD(1,k,i,j) = a(k);
                end
            elseif(i == j)
                for k = 1:4
                    chordCPD(1,k,i,j) = 1 - a(k);
                end
            else
                for k = 1:4
                    chordCPD(1,k,i,j) = a(k) * chordClasses(k).Signature * majSignatureOnKey * delta;
                end                  
            end
        end
    end
        
	if(max(chordProgressionMatrixMin(j,:)) == 0)
        for j = 1:4
            for k = 1:size(chordClasses,2)
                chordCPD(2,j,i,k) = chordClasses(k).Signature * minSignatureOnKey;
            end
        end
    else
        for j = 1:size(chordClasses,2)
            if(chordProgressionMatrixMin(i,j) > 0)
                for k = 1:4
                    chordCPD(2,k,i,j) = a(k);
                end
            elseif(j == k)
                for k = 1:4
                    chordCPD(2,k,i,j) = 1 - a(k);
                end
            else
                for k = 1:4
                    chordCPD(2,k,i,j) = a(k) * chordClasses(j).Signature * minSignatureOnKey * delta;
                end                  
            end
        end
    end
end
%normalize
for i = 1:2
    for j = 1:4
        for k = 1:size(chordClasses,2)
            my_sum = 0;
            for r = 1:size(chordClasses,2)
                my_sum = my_sum + chordCPD(i,j,k,r);
            end
            for r = 1:size(chordClasses,2)
                chordCPD(i,j,k,r) = chordCPD(i,j,k,r) / my_sum;
            end
        end
    end
end

for i = 0:tSlice - 1
    for k = 1:6
        CPD{i * 6 + k} = [];
    end
    
    nValues(i * 6 + 1:(i + 1) * 6) = [2 4 size(chordClasses,2) 12 12 12];
    EL(i * 2 + 1) = i * 6 + 4;
    EL(i * 2 + 2) = i * 6 + 5;
    %parentList
    if(i == 0)
        parentList{1} = [];
        parentList{2} = [];
        parentList{3} = [0 1];
        parentList{4} = [2];
        parentList{5} = [2];
        parentList{6} = [3];
    else
        parentList{i * 6 + 1} = [(i - 1) * 6];
        parentList{i * 6 + 2} = [(i - 1) * 6 + 1];
        parentList{i * 6 + 3} = [i * 6, i * 6 + 1,(i - 1) * 6 + 2]; %P(chord(i + 1) | key, beat(i + 1), chord(i))
        parentList{i * 6 + 4} = [i * 6 + 2];
        parentList{i * 6 + 5} = [i * 6 + 2];
        parentList{i * 6 + 6} = [i * 6 + 3]; 
    end
    %childrenList
    if(i == tSlice - 1)
        childrenList{i * 6 + 1} = [i * 6 + 2];
        childrenList{i * 6 + 2} = [i * 6 + 2];
        childrenList{i * 6 + 3} = [i * 6 + 3,i * 6 + 4]; %P(chord(i + 1) | key, beat(i + 1), chord(i))
        childrenList{i * 6 + 4} = [i * 6 + 5];
        childrenList{i * 6 + 5} = [];
        childrenList{i * 6 + 6} = []; 
    else
        childrenList{i * 6 + 1} = [i * 6 + 2,(i + 1) * 6];
        childrenList{i * 6 + 2} = [i * 6 + 2, (i + 1) * 6 + 1];
        childrenList{i * 6 + 3} = [i * 6 + 3,i * 6 + 4, (i + 1) * 6 + 2];
        childrenList{i * 6 + 4} = [i * 6 + 5];
        childrenList{i * 6 + 5} = [];
        childrenList{i * 6 + 6} = [];  
    end
    %priors
    prior{i * 6 + 1} = ones(1,2) / 2;%[1 0];%ones(1,2) / 2;
    prior{i * 6 + 2} = ones(1,4) / 4;
    prior{i * 6 + 3} = ones(1,size(chordClasses,2)) / size(chordClasses,2);
    prior{i * 6 + 4} = ones(1,12) / 12;
    prior{i * 6 + 5} = Ct(:,i + 1);
    prior{i * 6 + 6} = Cb(1:12,i + 1);
    
    my_sum = 0;
    for j = 1:12
        my_sum = my_sum + Ct(j,i + 1);
    end    

    if(my_sum < 0.1)
        prior{i * 6 + 5} = ones(12,1) / 12;
    end

    if(Cb(13,i + 1) < 0.01 * Cb_max)
        prior{i * 6 + 6} = ones(12,1) / 12;
    end

    for j = 1:12
        if(prior{i * 6 + 5}(j) < epsilon)
            prior{i * 6 + 5}(j) = epsilon;
        end
        if(prior{i * 6 + 6}(j) < epsilon)
            prior{i * 6 + 6}(j) = epsilon;
        end
    end
    %normalize
    my_sum = 0;
    for j = 1:12
        my_sum = my_sum + prior{i * 6 + 5}(j);
    end
    for j = 1:12
        prior{i * 6 + 5}(j) = prior{i * 6 + 5}(j) / my_sum;
    end
    
    my_sum = 0;
    for j = 1:12
        my_sum = my_sum + prior{i * 6 + 6}(j);
    end
    for j = 1:12
        prior{i * 6 + 6}(j) = prior{i * 6 + 6}(j) / my_sum;
    end   
    %CPD
    %P(beat(t + 1) | beat(t))
    if(i > 0)
        CPD{i * 6 + 1} = eye(2);
        CPD{i * 6 + 2} = [
            0 , 1-epsilon , epsilon , 0;
            0 , 0 , 1-epsilon , epsilon;
            epsilon , 0 , 0 , 1-epsilon;
            1-epsilon , epsilon , 0 , 0];
    end
    %P(chord(i + 1) | key, beat(i + 1), chord(i))
    if(i == 0)
        CPD{i * 6 + 3} = simpleChordCPD;
    else
        CPD{i * 6 + 3} = chordCPD;
    end
    %P(bass(i) | chord(i))
    CPD{i * 6 + 4} = bassCPD;
    %P(chroma(i) | chord(i))
    CPD{i * 6 + 5} = chromaCPD;
    %P(chroma(i) | chord(i))
    CPD{i * 6 + 6} = bassChromaCPD;
end

chord = zeros(12,size(Ct,2));
bass = zeros(1,size(Ct,2));
chordNames = cell(1,size(Ct,2));

BEL = CreateFactorGraph(parentList,childrenList,nValues,prior,CPD,EL,32);

for i = 0:tSlice - 1
    [beat index] = max(BEL{i*6 + 2});
    [C, indexC] = max(BEL{i*6 + 3});
    [B, indexB] = max(BEL{i*6 + 3});
    chord(:,i+1) = chordClasses(indexC).Signature;
    chordNames{i + 1} = chordClasses(indexC).Name;
    bass(:,i+1) = indexB;
end
end


