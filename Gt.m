function [gt] = Gt()
    gt = zeros(1,92);
    low1 = 44;
    low2 = 56;
    high1 = 68;
    high2 = 92;
    for i = low1:low2
        gt(i) = (i - low1)/(low2 - low1);
    end
    for i = low2:high1
        gt(i) = 1;
    end
    for i = high1:high2
        gt(i) = 1 - (i - high1)/(high2 - high1);
    end
end