function [S_pre] = Spre(S_s,S_c)
S_pre = zeros(size(S_s));
for i = 1:size(S_pre,2)
    for j = 9 : (size(S_pre,1) - 9)
        sub_s = S_s((j - 9 + 1): (j + 9),i);
        sub_c = S_c((j - 9 + 1): (j + 9),i);
        
        mu_s = sum(sub_s(:))/18;
        mu_c = sum(sub_c(:))/18;
        
        sigma_s = sqrt(sum((sub_s(:) - mu_s).^2)/18);
        sigma_c = sqrt(sum((sub_c(:) - mu_c).^2)/18);
        %if(S_s(j,i) > mu_s + sigma_s)
        if(S_s(j,i) > mu_s + sigma_s && S_c(j,i) > mu_c + sigma_c)
            S_pre(j,i) = S_s(j,i) * S_c(j,i);
        end
    end
end
end