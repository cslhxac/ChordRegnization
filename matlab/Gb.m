function [gb] = Gb()
    gb = zeros(1,92);
    low1 = 27;
    low2 = 33;
    high1 = 44;
    high2 = 56;
    for i = low1:low2
        gb(i) = (i - low1)/(low2 - low1);
    end
    for i = low2:high1
        gb(i) = 1;
    end
    for i = high1:high2
        gb(i) = 1 - (i - high1)/(high2 - high1);
    end
end