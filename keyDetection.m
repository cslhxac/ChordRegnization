function [ keyMaj, keyMin ] = keyDetection( chroma )
%KEYDETECTION Summary of this function goes here
%   Detailed explanation goes here
% Given the chroma of the music it output the possible key(Both maj and min)
%%%%!!!!!!!!!!!The first key is A in the chroma!!!!!!!!!!!!%%%%%
majSignature = [1 -1 1 -1 1 1 -1 1 -1  1 -1 1]';
minSignature = [1 -1 1 1 -1 1 -1 1  1 -1  1 1]';%%The miner can both be harmonic or natual

scoreMajor = zeros(1,12);
scoreMinor = zeros(1,12);
for i = 1:12
    majSignatureOnKey = circshift(majSignature,i - 1);
    minSignatureOnKey = circshift(minSignature,i - 1);
    
    scoreMajor(i) = sum(majSignatureOnKey' * chroma);
    scoreMinor(i) = sum(minSignatureOnKey' * chroma);
end

[~, keyMaj] = max(scoreMajor);
[~, keyMin] = max(scoreMinor);
end

