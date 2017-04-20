function [frame] = get_confirm(handles)

    % get confirm data
    timeout = 30;   % set timeout for 3 seconds
    BytesInput = get(handles.s,'BytesAvailable');
    while(BytesInput == 0 && timeout ~= 0)
        pause(0.1); % wait for board response
        timeout = timeout - 1;
        BytesInput = get(handles.s,'BytesAvailable');
    end

    if(timeout == 0) % timeout break: no response
        handles.popup_status.String = 'No response from board';
        frame = [];
    else             % board response
        % process data got
        temp = fscanf(handles.s);   % read the first line
        frame = temp;
        handles.popup_status.String = temp;
    end
    
end