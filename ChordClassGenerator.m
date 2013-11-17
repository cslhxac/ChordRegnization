function [ chordClasses ] = ChordClassGenerator()
%CHORDCLASSGENERATOR Summary of this function goes here
%   Detailed explanation goes here
%generate all chord classes.


chordClasses = struct('Name','','Signature',zeros(1,12));
chordClasses(1:4*12)=chordClasses;
KeyNames = {'A','A#','B','C','C#','D','D#','E','F','F#','G','G#'};

for i = 1:12
    %%Major
    majorSignature = zeros(1,12);
    majorSignature(i) = 1;
    majorSignature(mod(i + 3,12) + 1) = 1;
    majorSignature(mod(i + 6,12) + 1) = 1;
    chordClasses(i * 1).Name = strcat(KeyNames(i),'maj');
    chordClasses(i * 1).Signature = majorSignature;

    %%Minor
    minorSignature = zeros(1,12);
    minorSignature(i) = 1;
    minorSignature(mod(i + 2,12) + 1) = 1;
    minorSignature(mod(i + 6,12) + 1) = 1;
    chordClasses(i * 2).Name = strcat(KeyNames(i),'min');
    chordClasses(i * 2).Signature = minorSignature;
    
    %%diminished
    dimSignature = zeros(1,12);
    dimSignature(i) = 1;
    dimSignature(mod(i + 2,12) + 1) = 1;
    dimSignature(mod(i + 5,12) + 1) = 1;
    chordClasses(i * 3).Name = strcat(KeyNames(i),'dim');
    chordClasses(i * 3).Signature = dimSignature;
    
    %%Augmented
    augSignature = zeros(1,12);
    augSignature(i) = 1;
    augSignature(mod(i + 3,12) + 1) = 1;
    augSignature(mod(i + 7,12) + 1) = 1;
    chordClasses(i * 4).Name = strcat(KeyNames(i),'aug');
    chordClasses(i * 4).Signature = augSignature;

end
end

