function [mappedPitch mappedFrequency] = mapPitch(frequency,Notes,NotesFreq)
    for i = 1:size(frequency(:))
        tmp = frequence(i);
        while(tmp < NotesFreq(1))
            tmp = tmp * 2;
        end
        while(tmp > NotesFreq(end))
            tmp = tmp / 2;
        end
    end
end
