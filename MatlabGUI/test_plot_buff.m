%% this is a note for using Serial with matlab

% initial port
s = serial('COM5');
set(s,'BaudRate',115200);
fopen(s);

% initial plot
figure(1); cla reset;
Y_Max = 100; Y_Min = -100;
ylim([Y_Min Y_Max]); 
grid on;     hold on;

% initial frame
formatSpec = 'E2,%d,%d,%d,00';
% frame = sprintf(formatSpec,Ts,X1,X2);
% fprintf(s,frame);
for i = 1: 1000

    % write to serial port: |ID|Order=2|,|Ts|,|X1|,Y1|,|CRC|LF|
    Ts = i;
    X1 = i*2-500;
    X2 = i*2-400;
    frame = sprintf(formatSpec,Ts,X1,X2);
    fprintf(s,frame);  

    pause(0.1);
    
    % read from serial port
    BytesInput = get(s,'BytesAvailable');
    if(BytesInput>1)
        ScanRead = fscanf(s);   % read the first line very fast
    end
    data_read = str2num(ScanRead(4:end));
    % decode frame read
    Ts_read = data_read(1);
    X1_read = data_read(2);
    Y1_read = data_read(3);
    
    % set axis for ploting
    if(i<200) 
        xlim([0 220]); 
    else
%         xlim([i-200 i+20]); 
        xlim([0 i+20]); 
    end
    
    if(X1_read>Y_Max)
        Y_Max=X1_read;
    end
    if(X1_read<Y_Min)
        Y_Min=X1_read;
    end
    if(Y1_read>Y_Max)
        Y_Max=Y1_read;
    end
    if(Y1_read<Y_Min)
        Y_Min=Y1_read;
    end
    ylim([Y_Min Y_Max]); 
    
    plot(Ts_read,X1_read,'r-x');
    plot(Ts_read,Y1_read,'b-o');
    
    pause(0.05);
     
end

% close terminal
newobjs = instrfind;     % Read Serial port Object that from memory, that are connected before
fclose(newobjs);          % close all opened ports/index
delete(newobjs);          % clear all Objects from memory
