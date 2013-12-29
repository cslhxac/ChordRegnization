function [X] = DFT(start,N,data)
    X = zeros(1,N);
    for n = 1:N
        
        X(n) = data(start:(start+N-1)) * exp(-1i*2*pi*n*(0:N-1)/N)';

    end
end