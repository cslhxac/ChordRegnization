function [ chordNames chord bass ] = ChordDetection( audioFileName, MidiOutput, audioOutput)
%CHORDDETECTION Summary of this function goes here
%   Detailed explanation goes here
mex bnt_C\CreateFactorGraph.cpp
addpath 'matlab-midi-master/src/'
addpath 'matlab/'

samplingRate = 11025;
window = 4096;
hop = 512;
    
[y,Fs] = audioread(audioFileName);
%%cast y to one channel!!!
y_new = zeros(size(y,1),1);
for i = 1 : size(y,2)
    y_new(:) = y_new(:) + y(:,i);
end
[ resampledAudio,FD ] = FrequencyDomainTransform(y_new,Fs);
M_s = Ms();
M_c = Mc();
S_s = S(M_s,FD);
S_c = S(M_c,FD);
S_pre = Spre(S_s,S_c);
%tuning!!
Sbar = sum(S_pre,2)/size(S_pre,2);
SbarFFT = fft(Sbar); 
%the tuning shift
t = unwrap(angle(SbarFFT));
delta = wrapToPi(-t(49)- 2 * pi / 3) / 2 / pi;
tuneA = 440 * 2^(delta/12);
bin49 = log2(tuneA/27.5) * 12 + 1;

gridOrigin = 1:216;
gridNew = (bin49-49):(bin49 + 166);
for i = 1:size(S_pre,2)
    S_pre(:,i) = max(interp1(gridOrigin,S_pre(:,i),gridNew,'linear'),0);
end
%tuning done.
S_final = zeros(72,size(S_pre,2));
for i = 1:72
    S_final(i,:) = sum(S_pre(i*3-2:i*3,:),1);
end

gb = Gb();
gt = Gt();
gw = Gw();

Cb = zeros(12,size(S_final,2));
Ct = zeros(12,size(S_final,2));
Cw = zeros(12,size(S_final,2));
for i = 1:12
    for j = 0:5
        Cb(i,:) = Cb(i,:) + S_final(i + j * 12,:) * gb(i + 20 + j * 12);
    end
end

for i = 1:12
    for j = 0:5
        Ct(i,:) = Ct(i,:) + S_final(i + j * 12,:) * gt(i + 20 + j * 12);
    end
end

for i = 1:12
    for j = 0:5
        Cw(i,:) = Cw(i,:) + S_final(i + j * 12,:) * gw(i + 20 + j * 12);
    end
end

beat = beat2(resampledAudio,11025);
beatSec = beat;
%convert beat from sec to frame#
beat = round(beat * samplingRate / hop);

Ct_sync = zeros(12,size(beat,2) - 1);
for j = 1:12;
    for m = 1:size(beat,2) - 1
        Ct_sync(j,m) = median(Ct(j,beat(m):(beat(m+1)-1)));
    end
end

for m = 1:size(beat,2) - 1
	Ct_sync(:,m) = Ct_sync(:,m) / max(Ct_sync(:,m));
end


Cb_sync = zeros(13,size(beat,2) - 1);
for j = 1:12;
    for m = 1:size(beat,2) - 1
        Cb_sync(j,m) = median(Cb(j,beat(m):(beat(m+1)-1)));
    end
end
Cb_max = max(Cb(:));
for m = 1:size(beat,2) - 1
    Cb_sync(13,m) = max(Cb_sync(1:12,m));
	Cb_sync(1:12,m) = Cb_sync(1:12,m) / max(Cb_sync(1:12,m)); 
end

Ct_sync(isnan(Ct_sync)) = 0;
[keyMajor, keyMinor] = keyDetection(Ct_sync);

[ chordClasses ] = ChordClassGenerator();
%%
%%now iterate through the chords to find the best chord match
%%
chordChroma = zeros(12,size(beat,2) - 1);
for m = 1:size(beat,2) - 1
    previouScore = 0;
    for j = 1:size(chordClasses,2)
        score = Ct_sync(:,m)'* chordClasses(j).Signature';
        if(score > previouScore)
            chordChroma(:,m) = chordClasses(j).Signature';
            previouScore = score;
        end
    end
end




[ chordNames chord bass ] = BN(chordClasses,Cb_sync,Cb_max,Ct_sync,keyMajor,keyMinor);
[ midiMatrix ] = SynthesisMIDI( chord,beatSec );
midi_new = matrix2midi(midiMatrix);
writemidi(midi_new, MidiOutput);


a = midi2audio(midi_new);
tmp = zeros(size(y_new));
tmp(1:size(a,2)) = a;
wavwrite(y_new/2 + tmp/2,Fs,audioOutput);

end

