
samplingRate = 11025;
window = 4096;
hop = 512;
    
[y,Fs] = audioread('test.mp3');

[ resampledAudio,FD ] = FrequencyDomainTransform(y,Fs);
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
imagesc(S_final);
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
%convert beat from sec to frame#
beat = round(beat * samplingRate / hop);

Ct_sync = zeros(12,size(beat,2) - 1);
for j = 1:12;
    for m = 1:size(beat,2) - 1
        Ct_sync(j,m) = median(Cw(j,beat(m):(beat(m+1)-1)));
    end
end

for m = 1:size(beat,2) - 1
	Ct_sync(:,m) = Ct_sync(:,m) / max(Ct_sync(:,m));
end
Ct_sync(isnan(Ct_sync)) = 0;
[keyMajor, keyMinor] = keyDetection(Ct_sync);