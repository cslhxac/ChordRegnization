function [ M_s ] = Ms()
%MS Summary of this function goes here
%   Detailed explanation goes here
mid = 440;
low = 27.5;
high = 1760;

samplingRate = 11025;

window = 4096;
w = 0.54 - 0.46 * cos(2 * pi * (0:(window-1)) / (window-1));


n = log2(high / low) * 36;
M_s = zeros(n,window/2);
for i = 1:n
    freq = low * 2^(i/36.0);
    sineWave = sin(2 * pi * freq * (0:(window-1))/samplingRate);
    tmp = fft(Convolution1D(w,sineWave),window);
    tmp = 2 * abs(tmp(1:window/2)).^0.5;
    L2 = sqrt(sum(tmp.^2));
    M_s(i,:) = tmp / L2;
end

end

