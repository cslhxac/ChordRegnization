function [ midiMatrix ] = SynthesisMIDI( chroma,beat )
%SYNTHESESMIDI Summary of this function goes here
%   Detailed explanation goes here
%the beat should be the same size as chroma.

N = size(find(chroma > 0),1);
midiMatrix = zeros(N,6);
counter = 1;
for m = 1:size(chroma,2)
    for j = 1:12
        if(chroma(j,m) > 0)
            midiMatrix(counter,1) = 1;         % all in track 1
            midiMatrix(counter,2) = 1;         % all in channel 1
            midiMatrix(counter,3) = (56 + j);      % note numbers: one ocatave starting at middle C (60)
            midiMatrix(counter,4) = round(80*chroma(j,m));  % lets have volume ramp up 80->120
            midiMatrix(counter,5) = beat(m);  % note on:  notes start every .5 seconds
            midiMatrix(counter,6) = beat(m + 1);   % note off: each note has duration .5 seconds 
            
            counter = counter + 1;
        end
    end
end

end

