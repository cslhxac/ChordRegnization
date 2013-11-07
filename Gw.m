function [gw] = Gw()
    gw = zeros(1,92);
    low1 = 27;
    low2 = 33;
    high1 = 68;
    high2 = 92
    for i = low1:low2
        gw(i) = (i - low1)/(low2 - low1);
    end
    for i = low2:high1
        gw(i) = 1;
    end
    for i = high1:high2
        gw(i) = 1 - (i - high1)/(high2 - high1);
    end
end