function [ output_args ] = playFD(FDPosition,Fs,window,hop,audio,originalFs)
%PLAYFD Summary of this function goes here
%   Detailed explanation goes here
output_args = 1;
soundstart = (FDPosition * hop * originalFs/Fs);
soundend = ((FDPosition * hop + window) * originalFs/Fs)-1;
w = 0.54 - 0.46 * cos(2 * pi * (0:(window* originalFs/Fs-1)) / (window* originalFs/Fs-1));
soundsc(conv(audio(soundstart:soundend),w),originalFs);
end

