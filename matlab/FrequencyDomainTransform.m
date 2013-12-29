function [ resampledAudio,FD,newFs ] = FrequencyDomainTransform(audio,Fs)
    %FREQUENCYDOMAINTRANSFORM Summary of this function goes here
    %   Detailed explanation goes here
    samplingRate = 11025;
    window = 4096;
    hop = 512;
    newFs = samplingRate;

    w = 0.54 - 0.46 * cos(2 * pi * (0:(window-1)) / (window-1));

    if(Fs > samplingRate)
        resampledAudio = smooth(audio);
        resampledAudio = resample(resampledAudio,samplingRate,Fs);
    else
        resampledAudio = audio;
    end

    FD = zeros(window/2,floor(size(resampledAudio,1)/512));
    counter = 1;
    for i = 1:hop:size(resampledAudio,1)
        if(i + window - 1 < size(resampledAudio,1))
            sample = Convolution1D(w,resampledAudio(i:i+window-1));
            tmp = fft(sample,window)/window;
            FD(:,counter) = 2 * abs(tmp(1:window/2)).^0.5;
            counter = counter + 1;
        end
    end

end

