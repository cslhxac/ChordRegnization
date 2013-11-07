[y,Fs] = audioread('test.mp3');
[ resampledAudio,FD ] = FrequencyDomainTransform(y,Fs);
M_s = Ms();
M_c = Mc();
S_s = S(M_s,FD);
S_c = S(M_c,FD);
S_pre = Spre(S_s,S_c);
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