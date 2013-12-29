[y,Fs] = audioread('test.mp3');

Notes = {'A','A#','B','C','C#','D','D#','E','F','F#','G','G#'};
NotesFreq = zeros(1,12);
NotesFreq(1) = 440;
for i = 2:12
    NotesFreq(i) = 440 * 2^((i - 1)/12.0);
end
% Sampling frequency
T = 1/Fs;                     % Sample time
L = 1000;                     % Length of signal
t = (0:L-1)*T;                % Time vector

y = y(1000000:1004095);
NFFT = 4096; % Next power of 2 from length of y
Y = fft(y,NFFT)/L;
x = Fs/2*linspace(0,1,NFFT/2+1);

Y = Y(x > 220 & x < 440 * 3);
x = x(x > 220 & x < 440 * 3);

noteLabel = cell(size(x));
tick = zeros(size(x));
counter = 1;
for i = 1:size(x,2)
    noteLabel{i} = '';
    tmp = x(i);
    if (tmp < 220 || tmp > 440 * 3)
        continue;
    end
    while(tmp < NotesFreq(1))
        tmp = tmp * 2;
    end
    while(tmp > NotesFreq(end))
        tmp = tmp / 2;
    end
    for j = 1:size(NotesFreq,2)
        if(tmp > NotesFreq(j) * 0.99999 && tmp < NotesFreq(j) * 1.01 && (counter == 1 || strcmp(noteLabel{counter - 1},Notes{j}) == 0))
            noteLabel{counter} = Notes{j};
            tick(counter) = i;
            counter = counter + 1;
            break;
        end
    end
end
noteLabelFinal = cell(1,(counter-1));
for i = 1:counter-1
    noteLabelFinal{i} = noteLabel{i};
end
% Plot single-sided amplitude spectrum.
figure;plot(x,2*abs(Y));
set(gca, 'XTick',x(tick(1:(counter-1))));
set(gca,'XTickLabel',noteLabelFinal);
%set(gca,'XLim',[200 440*3],'XTick',1:50, 'XTickLabel',repmat(['T'],1,50)');

%title('Single-Sided Amplitude Spectrum of y(t)')
%xlabel('Frequency (Hz)')
%ylabel('|Y(f)|')