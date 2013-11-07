function [ M_c ] = Mc()
%MS Summary of this function goes here
%   Detailed explanation goes here
mid = 440;
low = 27.5;
high = 1760;

samplingRate = 11025;

window = 4096;
w = 0.54 - 0.46 * cos(2 * pi * (0:(window-1)) / (window-1));


n = log2(high / low) * 36;
M_c = zeros(n,window/2);
for i = 1:n
    freq = low * 2^(i/36.0);
    root = sin(2 * pi * freq * (0:(window-1))/samplingRate);
    partial1 = 0.9 * sin(2 * pi * freq * (0:(window-1))/samplingRate * 2);
    partial2 = 0.81 * sin(2 * pi * freq * (0:(window-1))/samplingRate * 3);
    partial3 = 0.729 * sin(2 * pi * freq * (0:(window-1))/samplingRate * 4);
    tmp = root;
    if(freq * 2 < high)
        tmp = tmp + partial1;
    end
    if(freq * 3 < high)
        tmp = tmp + partial2;
    end
    if(freq * 4 < high)
        tmp = tmp + partial3;
    end
    tmp = fft(Convolution1D(w,tmp),window);
    tmp = 2 * abs(tmp(1:window/2)).^0.5;
    L2 = sqrt(sum(tmp.^2));
    M_c(i,:) = tmp / L2;
end

end
