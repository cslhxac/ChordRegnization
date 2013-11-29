function [ chordwBass ] = simpleDBNDetection(chordClasses,Cb,Ct,keyMaj,keyMin)
%SIMPLEDBNDETECTION Summary of this function goes here
%   Detailed explanation goes here
%this detectes chord only based on the current chroma and key.
%N = 3; 
%dag = zeros(N,N);
%key = 1; Chord = 2; Bass = 3;
%dag(key,Chord) = 1;
%dag(Chord,Bass) = 1;

%bnet = mk_bnet(dag, [12 size(chordClass) 13], 'discrete', [Chord Bass]);
majSignature = [1, -1, 1, -1, 1, 1, -1, 1, -1,  1, -1, 1]';
minSignature = [1, -1, 1,  1,-1, 1, -1, 1,  1, -1,  1, 1]';%%The miner can both be harmonic or natual

majSignatureOnKey = circshift(majSignature,keyMaj - 1);
minSignatureOnKey = circshift(minSignature,keyMin - 1);    

chordwBassMaj = zeros(12,size(Ct,2));
chordwBassMin = zeros(12,size(Ct,2));
totalScoreMaj = 0;
totalScoreMin = 0;
for i = 1:size(Ct,2)
    maxScoreMaj = 0;
    maxScoreMin = 0;
    for j = 1:size(chordClasses,2)
        Pckmaj = majSignatureOnKey' * chordClasses(j).Signature';
        Pckmin = minSignatureOnKey' * chordClasses(j).Signature';
        Pos = Ct(:,i)' * chordClasses(j).Signature';
        Pmaj = Pckmaj * Pos;
        Pmin = Pckmin * Pos;
        if(maxScoreMaj < Pmaj)
            maxScoreMaj = Pmaj;
            chordwBassMaj(:,i) = chordClasses(j).Signature;
        end
        if(maxScoreMin < Pmaj)
            maxScoreMin = Pmin;
            chordwBassMin(:,i) = chordClasses(j).Signature;
        end
    end
    totalScoreMaj = totalScoreMaj + maxScoreMaj;
    totalScoreMin = totalScoreMin + maxScoreMin;
end
%if(totalScoreMaj > totalScoreMin)
    chordwBass = chordwBassMaj;
%    'maj'
%else
%    chordwBass = chordwBassMin;
%    'min'    
%end
end

