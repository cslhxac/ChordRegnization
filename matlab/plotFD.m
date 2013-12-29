function [ output_args ] = plotFD(FD,Fs,Window)
%PLOTFD Summary of this function goes here
%   Detailed explanation goes here
output_args = 1;
Notes = {'A','A#','B','C','C#','D','D#','E','F','F#','G','G#'};
NotesFreq = zeros(1,12);
NotesFreq(1) = 440;
for i = 2:12
    NotesFreq(i) = 440 * 2^((i - 1)/12.0);
end

x = Fs/2*linspace(0,1,Window/2+1);

Y = FD(x > 27 & x < 1670);
x = x(x > 27 & x < 1670);

noteLabel = cell(size(x));
tick = zeros(size(x));
counter = 1;
for i = 1:size(x,2)
    noteLabel{i} = '';
    tmp = x(i);
    if (tmp < 27 || tmp > 1670)
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
figure;plot(x,Y);
set(gca, 'XTick',x(tick(1:(counter-1))));
set(gca,'XTickLabel',noteLabelFinal);
end

