function [signal] = Convolution1D(a,b)
    signal = zeros(size(a));
    for i = 1:size(a(:))
        signal(i) = a(i) * b(i);
    end
end