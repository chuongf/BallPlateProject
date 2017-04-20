function varargout = BallPlateControl(varargin)
% BALLPLATECONTROL MATLAB code for BallPlateControl.fig
%      BALLPLATECONTROL, by itself, creates a new BALLPLATECONTROL or raises the existing
%      singleton*.
%
%      H = BALLPLATECONTROL returns the handle to a new BALLPLATECONTROL or the handle to
%      the existing singleton*.
%
%      BALLPLATECONTROL('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in BALLPLATECONTROL.M with the given input arguments.
%
%      BALLPLATECONTROL('Property','Value',...) creates a new BALLPLATECONTROL or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before BallPlateControl_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to BallPlateControl_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help BallPlateControl

% Last Modified by GUIDE v2.5 09-Apr-2017 08:04:35

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @BallPlateControl_OpeningFcn, ...
                   'gui_OutputFcn',  @BallPlateControl_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before BallPlateControl is made visible.
function BallPlateControl_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to BallPlateControl (see VARARGIN)

% find com ports
handles.COMPORT_Select.String = getAvailableComPort();
handles.ComPort = cell2mat(handles.COMPORT_Select.String(1));
handles.ComBaud = 115200;
if(isempty(handles.ComPort))
    handles.COM_Connect.Enable = 'off';
    handles.COMPORT_Select.String = 'No COM found';
end

% reset all COM ports that it connected
newobjs = instrfind;     % Read Serial port Object from memory.
% fclose(newobjs);          % close all opened ports/index
delete(newobjs);          % clear all Objects from memory if any
newobjs = instrfind;
if(~size(newobjs)) 
    handles.COM_Connect.Enable = 'on';
    guidata(hObject,handles);
end

% disable control setting and offset setting
dis_CtrlSetting(handles);

% mode selection
handles.radiobutton_SettingMode.Enable = 'off';
handles.radiobutton_RunMode.Enable = 'off';
handles.radiobutton_StandbyMode.Enable  = 'off';

handles.radiobutton_SettingMode.Value = 0;
handles.radiobutton_RunMode.Value = 0;
handles.radiobutton_StandbyMode.Value  = 0;



handles.CtrlRun = 1;
handles.RX_Read.Value = 0;

handles.Model_K = 0;

% plot configure
%%
% plot X position
    axes(handles.X_Pos); hold on; grid on
    ylim([0 800]); 
    title('X Position (Meas: blue)');
%     xlabel('second'); ylabel('PXL');
    
% plot X speed
    axes(handles.X_Speed); hold on; grid on
    ylim([-500 500]); 
    title('X Speed (Diff: blue)');
% plot X kalman
    axes(handles.X_KF); hold on; grid on
    ylim([-1000 1000]); 
    title('X Variances (P1: blue, P2: red)');
%%
% plot Y position

    axes(handles.Y_Pos); hold on; grid on
    ylim([0 600]);
    title('Y Position (Meas: blue)');
% plot Y speed
    axes(handles.Y_Speed); hold on ; grid on
    ylim([-500 500]); 
    title('Y Speed (Diff: blue)');
% plot Y kalman
    axes(handles.Y_KF); hold on; grid on
    ylim([-1000 1000]); 
    title('Y Variances (P1: blue, P2: red)');
    
% Choose default command line output for BallPlateControl
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes BallPlateControl wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = BallPlateControl_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on slider movement.
function X_Offset_Slide_Callback(hObject, eventdata, handles)
% hObject    handle to X_Offset_Slide (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

OffsetDeg = round(10*(get(hObject,'Value')),1);
OffsetPWM = 10*OffsetDeg;

handles.edit_XDeg.String  = OffsetDeg;
handles.edit_Xus.String  = OffsetPWM;
handles.popup_status.String =(['X Offset: ' num2str(OffsetDeg) '°']);

%   send to board
formatSpec = 'SO,X,%d';
frame = sprintf(formatSpec,OffsetPWM);
fprintf(handles.s,frame);
 get_confirm(handles);

        

% --- Executes on slider movement.
function Y_Offset_Slide_Callback(hObject, eventdata, handles)
% hObject    handle to Y_Offset_Slide (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

OffsetDeg = round(10*(get(hObject,'Value')),1);
OffsetPWM = 10*OffsetDeg;

handles.edit_YDeg.String  = OffsetDeg;
handles.edit_Yus.String  = OffsetPWM;
handles.popup_status.String =(['Y Offset: ' num2str(OffsetDeg) '°']);

%   send to board
formatSpec = 'SO,Y,%d';
frame = sprintf(formatSpec,OffsetPWM);
fprintf(handles.s,frame);
 get_confirm(handles);

    
% --- Executes on button press in pushbutton_Load.
function pushbutton_Load_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_Load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% ask the board for current settings
fprintf(handles.s,'G');     % get
handles.popup_status.String = 'Loading setting...';
pause(0.1); % wait for board response
timeout = 30;   % set timeout for 3 seconds
BytesInput = get(handles.s,'BytesAvailable');
while(BytesInput == 0 && timeout ~= 0)
    pause(0.1); % wait for board response
    timeout = timeout - 1;
    BytesInput = get(handles.s,'BytesAvailable');
end

if(timeout == 0) % timeout break: no response
    handles.popup_status.String = 'No response from board';
else             % board response
    % show data got
    % "G,MODE,ORDER,Ctr_K1,Ctr_K2,Ctr_K3,Ctr_V,XOffset,YOffset\n"
    ScanRead = fscanf(handles.s);   % read the first line
    data_read = str2num(ScanRead(3:end));
    
    % process data
    MODE = data_read(1);
    %MODE = 1: Run, 2: Setting, 4:Test, 0: Standby
    handles.radiobutton_StandbyMode.Value = 0;
    handles.radiobutton_SettingMode.Value = 0;
    handles.radiobutton_RunMode.Value = 0;
    if(MODE == 0) 
        handles.radiobutton_StandbyMode.Value = 1;
    elseif(MODE == 1) 
        handles.radiobutton_RunMode.Value = 1;
        handles.radiobutton_Plot.Visible = 'on';
        handles.radiobutton_Plot.Value = 0;
        handles.radiobutton_Plot.Enable = 'on';
    elseif(MODE == 2) 
        handles.radiobutton_SettingMode.Value = 1;
        % enable offset calibration feature
        en_CtrlSetting(handles);
        if(handles.radiobutton_CtrOder3rd.Value == 0)
            handles.edit_CtrPole3.Enable  = 'off';
            handles.edit_FBgain3.Enable  = 'off';
        end

    else % testing mode
        handles.radiobutton_StandbyMode.Value = 1;
    end
    
    % ORDER, Ctr_K1, K2, K3: Calculate back to poles and show them.
    ORDER = data_read(2);
    handles.radiobutton_CtrOder2nd.Value = 0; 
    handles.radiobutton_CtrOder3rd.Value = 0; 
    K1 = data_read(3)/1000;
    K2 = data_read(4)/1000;
    K3 = data_read(5)/1000;
    if(ORDER == 2)
        handles.edit_FBgain1.String = K1;
        handles.edit_FBgain2.String = K2;
        handles.edit_FBgain3.String = '-';
        handles.edit_CtrPole3.Enable = 'off';
        handles.radiobutton_CtrOder2nd.Value = 1; 
        handles.radiobutton_CtrOder3rd.Enable = 'off'; 
        handles.edit_CtrPole3.Enable = 'off';
        handles.edit_CtrPole3.String = '-';
    elseif(ORDER == 3)
        handles.edit_FBgain1.String = K1;
        handles.edit_FBgain2.String = K2;
        handles.edit_FBgain3.String = K3;
        handles.radiobutton_CtrOder3rd.Value = 1; 
        handles.radiobutton_CtrOder2nd.Enable = 'off'; 
    else
        disp('Not support higher than 3 order');
    end
    
    % Ctr_V
    Ctr_V = data_read(6);
    handles.edit_CtrFilter.String = Ctr_V/1000;
    
    % XOffset,YOffset
    XOffsetPWM = data_read(7);
    YOffsetPWM = data_read(8);
    handles.edit_Xus.String = XOffsetPWM;
    handles.edit_Yus.String = YOffsetPWM;
    handles.edit_XDeg.String = XOffsetPWM/10;
    handles.edit_YDeg.String = YOffsetPWM/10;
    handles.X_Offset_Slide.String = XOffsetPWM/100;
    handles.Y_Offset_Slide.String = YOffsetPWM/100;

%     handles.popup_status.String = temp;
    handles.popup_status.String = 'Loading setting...done';
end



    
% --- Executes on selection change in COMPORT_Select.
function COMPORT_Select_Callback(hObject, eventdata, handles)
% hObject    handle to COMPORT_Select (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns COMPORT_Select contents as cell array
%        contents{get(hObject,'Value')} returns selected item from COMPORT_Select
% COM_PORT_OP = get(hObject,'String');
% COM_PORT_ID = get(hObject,'Value');
% handles.ComPort = cell2mat(COM_PORT_OP(COM_PORT_ID));
% guidata(hObject,handles);
% disp(handles.ComPort);


% --- Executes during object creation, after setting all properties.
function COMPORT_Select_CreateFcn(hObject, eventdata, handles)
% hObject    handle to COMPORT_Select (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on selection change in COMBAUD_select.
function COMBAUD_select_Callback(hObject, eventdata, handles)
% hObject    handle to COMBAUD_select (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns COMBAUD_select contents as cell array
%        contents{get(hObject,'Value')} returns selected item from COMBAUD_select
COM_BAUD_OP = get(hObject,'String');
COM_BAUD_ID = get(hObject,'Value');
% COM_BAUD = COM_CNT(COM_BAUD_ID)
handles.ComBaud = str2num(cell2mat(COM_BAUD_OP(COM_BAUD_ID)));
guidata(hObject,handles);
handles.popup_status.String =(handles.ComBaud);

% --- Executes on button press in COM_Connect.
function COM_Connect_Callback(hObject, eventdata, handles)
% hObject    handle to COM_Connect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

newobjs = instrfind;
if(~size(newobjs)) 
    COM_PORT_OP = get(handles.COMPORT_Select,'String');
    COM_PORT_ID = get(handles.COMPORT_Select,'Value');
    handles.ComPort = cell2mat(COM_PORT_OP(COM_PORT_ID));
    guidata(hObject,handles);
%     disp(handles.ComPort);

    handles.s = serial(handles.ComPort);
    set(handles.s,'BaudRate',handles.ComBaud);
    fopen(handles.s);
    
    newobjs = instrfind;
    handles.popup_status.String =...
        ([newobjs.Port ' ' num2str(newobjs.BaudRate) ' ' newobjs.Status] );
    handles.COM_Connect.Enable = 'off';
    guidata(hObject,handles);
    
    % mode selection
    handles.radiobutton_SettingMode.Enable = 'on';
    handles.radiobutton_RunMode.Enable = 'on';
    handles.radiobutton_StandbyMode.Enable  = 'on';

    % save/load options
    handles.pushbutton_Load.Enable = 'on';
    handles.pushbutton_Save.Enable = 'on';
    
    % clear buffer if any
    BytesInput = get(handles.s,'BytesAvailable');
    while(BytesInput)
        temp = fscanf(handles.s);   % read the first line
        BytesInput = get(handles.s,'BytesAvailable');
    end

else
    handles.popup_status.String =('Can not connect to COM port.');
    handles.popup_status.String =('Press Connect button to try again');
    % fclose(newobjs);          % close all opened ports/index
    delete(newobjs);          % clear all Objects from memory if any
end




% --- Executes on button press in COM_disconnect.
function COM_disconnect_Callback(hObject, eventdata, handles)
% hObject    handle to COM_disconnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

newobjs = instrfind;     % Read Serial port Object from memory.
% fclose(newobjs);          % close all opened ports/index
delete(newobjs);          % clear all Objects from memory
newobjs = instrfind;
if(~size(newobjs)) 
    handles.COM_Connect.Enable = 'on';
    guidata(hObject,handles);
	handles.popup_status.String =('Com port disconnected');
end
dis_CtrlSetting(handles);

% mode selection
handles.radiobutton_SettingMode.Enable = 'off';
handles.radiobutton_RunMode.Enable = 'off';
handles.radiobutton_StandbyMode.Enable  = 'off';

handles.radiobutton_SettingMode.Value = 0;
handles.radiobutton_RunMode.Value = 0;
handles.radiobutton_StandbyMode.Value  = 0;
% save/load options
handles.pushbutton_Load.Enable = 'off';
handles.pushbutton_Save.Enable = 'off';
handles.radiobutton_Plot.Visible = 'off';

function edit_XDeg_Callback(hObject, eventdata, handles)
% hObject    handle to edit_XDeg (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_XDeg as text
%        str2double(get(hObject,'String')) returns contents of edit_XDeg as a double

OffsetDeg = round(str2double(get(hObject,'String')),1);
OffsetPWM = 10*OffsetDeg;

handles.edit_Xus.String  = OffsetPWM;
handles.popup_status.String =(['X Offset: ' num2str(OffsetDeg) '°']);
temp = OffsetDeg/10;
if(temp>=-1 &&temp<=1) 
  handles.X_Offset_Slide.Value = temp;
elseif (temp<-1) 
  handles.X_Offset_Slide.Value = -1;
else
  handles.X_Offset_Slide.Value = 1;
end

%   send to board
formatSpec = 'SO,X,%d';
frame = sprintf(formatSpec,OffsetPWM);
fprintf(handles.s,frame);
 get_confirm(handles);


function edit_YDeg_Callback(hObject, eventdata, handles)
% hObject    handle to edit_YDeg (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_YDeg as text
%        str2double(get(hObject,'String')) returns contents of edit_YDeg as a double

OffsetDeg = round(str2double(get(hObject,'String')),1);
OffsetPWM = 10*OffsetDeg;

handles.edit_Yus.String  = OffsetPWM;
handles.popup_status.String =(['Y Offset: ' num2str(OffsetDeg) '°']);
temp = OffsetDeg/10;
if(temp>=-1 &&temp<=1) 
  handles.Y_Offset_Slide.Value = temp;
elseif (temp<-1) 
  handles.Y_Offset_Slide.Value = -1;
else
  handles.Y_Offset_Slide.Value = 1;
end

%   send to board
formatSpec = 'SO,Y,%d';
frame = sprintf(formatSpec,OffsetPWM);
fprintf(handles.s,frame);
 get_confirm(handles);




function UART_RX_Callback(hObject, eventdata, handles)
% hObject    handle to UART_RX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of UART_RX as text
%        str2double(get(hObject,'String')) returns contents of UART_RX as a double



% --- Executes on button press in RX_Read.
function RX_Read_Callback(hObject, eventdata, handles)
% hObject    handle to RX_Read (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of RX_Read
 % read from serial port 
LF = sprintf('\n'); 
CR = sprintf('\r');
 
while(1)
 if(get(hObject,'Value') && get(handles.s,'BytesAvailable') )
    BytesInput = get(handles.s,'BytesAvailable');
    if(BytesInput>=1)
        temp = fscanf(handles.s);   % read the first line
        ScanRead=[];
        for i = 1:length(temp)
            if(~(temp(i)==LF || temp(i)==CR))
                ScanRead = [ScanRead temp(i)];
            end
        end
        show = string(ScanRead);
        handles.UART_RX.String = show;
        guidata(hObject,handles);
        pause(0.1);
    end
 end
 pause();
end
 
function Serial_ISR(hObject, eventdata, handles)
fprintf(handles.s,'Here I am');
BytesInput = get(handles.s,'BytesAvailable');
    if(BytesInput>=1)
        temp = fscanf(handles.s);   % read the first line
        ScanRead=[];
        for i = 1:length(temp)
            if(~(temp(i)==LF || temp(i)==CR))
                ScanRead = [ScanRead temp(i)];
            end
        end
        show = string(ScanRead);
        handles.UART_RX.String = show;
        guidata(hObject,handles);
        pause(0.1);
    end




% --- Executes on button press in radiobutton_SettingMode.
function radiobutton_SettingMode_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_SettingMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_SettingMode
state = get(hObject,'Value');
if(state == 1) 
    handles.radiobutton_RunMode.Value = 0;
    handles.radiobutton_StandbyMode.Value = 0;
    handles.popup_status.String = 'Setting Mode';
    fprintf(handles.s,'S');
    
    
    % enable offset calibration feature
    en_CtrlSetting(handles);
    if(handles.radiobutton_CtrOder3rd.Value == 0)
        handles.edit_CtrPole3.Enable  = 'off';
        handles.edit_FBgain3.Enable  = 'off';
    end
    handles.radiobutton_Plot.Visible = 'off';

    % get confirm data
    get_confirm(handles);   

    
elseif (state == 0) 
    handles.radiobutton_SettingMode.Value = 1;
    fprintf(handles.s,'S');
    
    % get confirm data
    get_confirm(handles);   

end
guidata(hObject,handles);

% --- Executes on button press in radiobutton8_RunMode.
function radiobutton_RunMode_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton8_RunMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton8_RunMode
state = get(hObject,'Value');
if(state == 1) 
    handles.radiobutton_SettingMode.Value = 0;
    handles.radiobutton_StandbyMode.Value = 0;
    handles.popup_status.String = 'Running Mode';
    fprintf(handles.s,'R');
    
    % get confirm data
    get_confirm(handles);
    
elseif (state == 0) 
    handles.radiobutton_RunMode.Value = 1;
    fprintf(handles.s,'R');

    get_confirm(handles);

end
dis_CtrlSetting(handles)
handles.radiobutton_Plot.Visible = 'on';
handles.radiobutton_Plot.Value = 0;
handles.radiobutton_Plot.Enable = 'on';
guidata(hObject,handles);



function edit_TX_Callback(hObject, eventdata, handles)
% hObject    handle to edit_TX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_TX as text
%        str2double(get(hObject,'String')) returns contents of edit_TX as a double
dataSend = get(hObject,'String');
fprintf(handles.s,dataSend);
handles.popup_status.String = ['Sent: ' dataSend];




% --- Executes on button press in pushbutton_TXSend.
function pushbutton_TXSend_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_TXSend (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
dataSend = handles.edit_TX.String;
fprintf(handles.s,dataSend);
handles.popup_status.String = ['Sent: ' dataSend];



% --- Executes on button press in radiobutton_StandbyMode.
function radiobutton_StandbyMode_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_StandbyMode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_StandbyMode
state = get(hObject,'Value');
if(state == 1) 
    handles.radiobutton_SettingMode.Value = 0;
    handles.radiobutton_RunMode.Value = 0;
    handles.popup_status.String = 'Standby Mode';
    fprintf(handles.s,'D');
    
    % get confirm data
    get_confirm(handles);


elseif (state == 0) 
    handles.radiobutton_StandbyMode.Value = 1;
    handles.popup_status.String = 'Standby Mode';
    fprintf(handles.s,'D');
    
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
    else             % board response
        % process data got
        temp = fscanf(handles.s);   % read the first line
        handles.popup_status.String = temp;
    end
    
end
% offset calibration
dis_CtrlSetting(handles);
handles.radiobutton_Plot.Visible = 'off';

guidata(hObject,handles);


% --- Executes on button press in radiobutton_CtrOder2nd.
function radiobutton_CtrOder2nd_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_CtrOder2nd (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_CtrOder2nd


% --- Executes on button press in radiobutton_CtrOder3rd.
function radiobutton_CtrOder3rd_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_CtrOder3rd (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_CtrOder3rd



function edit_CtrPole1_Callback(hObject, eventdata, handles)
% hObject    handle to edit_CtrPole1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_CtrPole1 as text
%        str2double(get(hObject,'String')) returns contents of edit_CtrPole1 as a double



function edit_CtrPole2_Callback(hObject, eventdata, handles)
% hObject    handle to edit_CtrPole2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_CtrPole2 as text
%        str2double(get(hObject,'String')) returns contents of edit_CtrPole2 as a double



function edit_CtrPole3_Callback(hObject, eventdata, handles)
% hObject    handle to edit_CtrPole3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_CtrPole3 as text
%        str2double(get(hObject,'String')) returns contents of edit_CtrPole3 as a double



function edit_CtrFilter_Callback(hObject, eventdata, handles)
% hObject    handle to edit_CtrFilter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_CtrFilter as text
%        str2double(get(hObject,'String')) returns contents of edit_CtrFilter as a double
value = 1000*str2double(get(hObject,'String'));
formatSpec = 'SF,1,%d';
frame = sprintf(formatSpec,int32(value)); %x100 to get int number.
fprintf(handles.s,frame);
% wait for confirmation
get_confirm(handles)
BytesInput = get(handles.s,'BytesAvailable');
while(BytesInput)
    BytesInput = get(handles.s,'BytesAvailable');
    get_confirm(handles)
end


% --- Executes on button press in pushbutton_Save.
function pushbutton_Save_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_Save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% ask board to save setting to eeprom/flash permanently
fprintf(handles.s,'V');    % seting-> save
handles.popup_status.String = 'Saving...';
pause(0.1);

timeout = 30;   % set timeout for 3 seconds
BytesInput = get(handles.s,'BytesAvailable');
while(BytesInput == 0 && timeout ~= 0)
    pause(0.1); % wait for board response
    timeout = timeout - 1;
    BytesInput = get(handles.s,'BytesAvailable');
end

if(timeout == 0) % timeout break: no response
    handles.popup_status.String = 'No response from board';
else             % board response
    % process data got
    temp = fscanf(handles.s);   % read the first line
    handles.popup_status.String = temp;
end



function edit_FBgain1_Callback(hObject, eventdata, handles)
% hObject    handle to edit_FBgain1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_FBgain1 as text
%        str2double(get(hObject,'String')) returns contents of edit_FBgain1 as a double
value = 1000*str2double(get(hObject,'String'));
formatSpec = 'SG,1,%d';
frame = sprintf(formatSpec,int32(value)); %x100 to get int number.
fprintf(handles.s,frame);
% wait for confirmation
pause(0.01);
get_confirm(handles)
BytesInput = get(handles.s,'BytesAvailable');
while(BytesInput)
    BytesInput = get(handles.s,'BytesAvailable');
    get_confirm(handles)
end

function edit_FBgain2_Callback(hObject, eventdata, handles)
% hObject    handle to edit_FBgain2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_FBgain2 as text
%        str2double(get(hObject,'String')) returns contents of edit_FBgain2 as a double
value = 1000*str2double(get(hObject,'String'));
formatSpec = 'SG,2,%d';
frame = sprintf(formatSpec,int32(value)); %x100 to get int number.
fprintf(handles.s,frame);
% wait for confirmation
get_confirm(handles)
BytesInput = get(handles.s,'BytesAvailable');
while(BytesInput)
    BytesInput = get(handles.s,'BytesAvailable');
    get_confirm(handles)
end


function edit_FBgain3_Callback(hObject, eventdata, handles)
% hObject    handle to edit_FBgain3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_FBgain3 as text
%        str2double(get(hObject,'String')) returns contents of edit_FBgain3 as a double
value = 1000*str2double(get(hObject,'String'));
formatSpec = 'SG,3,%d';
frame = sprintf(formatSpec,int32(value)); %x100 to get int number.
fprintf(handles.s,frame);
% wait for confirmation
get_confirm(handles)
BytesInput = get(handles.s,'BytesAvailable');
while(BytesInput)
    BytesInput = get(handles.s,'BytesAvailable');
    get_confirm(handles)
end


% --- Executes on button press in pushbutton_PolePlace.
function pushbutton_PolePlace_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton_PolePlace (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% get poles then calculate feedback gain and filter
IsOrder2 = handles.radiobutton_CtrOder2nd.Value;
IsOrder3 = handles.radiobutton_CtrOder3rd.Value;
temp1 = (handles.edit_CtrPole1.String);
temp2 = (handles.edit_CtrPole2.String);
temp3 = (handles.edit_CtrPole3.String);

pole1 = str2double(temp1);
pole2 = str2double(temp2);
pole3 = str2double(temp3);

K_contents = cellstr(handles.popup_KModel.String);
K_index = handles.popup_KModel.Value;
K = cell2mat(K_contents(K_index));

if strcmp(K,'Rad-Met')
    Model_K = -0.8829;
elseif strcmp(K,'Rad-Pxl')
    Model_K = -1222.8;
else %strcmp(K,'Deg-Pxl')
    Model_K = -21.265;
end


if(isempty(temp1) || isempty(temp2) || pole1 == 0 || pole2 == 0)
    handles.popup_status.String = 'illegal poles';
else
    handles.popup_status.String = 'poles placement...';
    if(IsOrder2)
        A = [ 0 1; 0 0];
        b = [0; Model_K];
        cT = [1 0];
        poles = [pole1 pole2];
        rT = place(A,b,poles);
        v = 1/(cT*inv(b*rT-A)*b);

        handles.edit_FBgain1.String = round(rT(1),3);
        handles.edit_FBgain2.String = round(rT(2),3);
        handles.edit_CtrFilter.String = round(v,3);
        handles.popup_status.String = 'poles placement...done';

    elseif(IsOrder3)
        A = [ 0 1 0 ; 0 0 -21.2651; 0 0 -4];
        b = [0; 0; 4];
        cT = [1 0 0];
        poles = [pole1 pole2 pole3];
        rT = place(A,b,poles);
        v = 1/(cT*inv(b*rT-A)*b);

        handles.edit_FBgain1.String = round(rT(1),3);
        handles.edit_FBgain2.String = round(rT(2),3);
        handles.edit_FBgain3.String = round(rT(3),3);
        handles.edit_CtrFilter.String = round(v,3);
        handles.popup_status.String = 'poles placement...done';
        
    %     pole3 = str2double(handles.edit_CtrPole3.String);
%         handles.popup_status.String ='Not support 3rd order at the moment';

    else
        handles.popup_status.String = 'Please press Get button to know system order';
    end
end
    



function edit_Xus_Callback(hObject, eventdata, handles)
% hObject    handle to edit_Xus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_Xus as text
%        str2double(get(hObject,'String')) returns contents of edit_Xus as a double
OffsetPWM = round(str2double(get(hObject,'String')),1);
OffsetDeg = OffsetPWM/10;

handles.edit_XDeg.String  = OffsetDeg;
handles.popup_status.String =(['X Offset: ' num2str(OffsetDeg) '°']);
temp = OffsetDeg/10;
if(temp>=-1 &&temp<=1) 
  handles.X_Offset_Slide.Value = temp;
elseif (temp<-1) 
  handles.X_Offset_Slide.Value = -1;
else
  handles.X_Offset_Slide.Value = 1;
end

%   send to board
formatSpec = 'SO,X,%d';
frame = sprintf(formatSpec,OffsetPWM);
fprintf(handles.s,frame);
 get_confirm(handles);



function edit_Yus_Callback(hObject, eventdata, handles)
% hObject    handle to edit_Yus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_Yus as text
%        str2double(get(hObject,'String')) returns contents of edit_Yus as a double
OffsetPWM = round(str2double(get(hObject,'String')),1);
OffsetDeg = OffsetPWM/10;

handles.edit_YDeg.String  = OffsetDeg;
handles.popup_status.String =(['Y Offset: ' num2str(OffsetDeg) '°']);
temp = OffsetDeg/10;
if(temp>=-1 &&temp<=1) 
  handles.Y_Offset_Slide.Value = temp;
elseif (temp<-1) 
  handles.Y_Offset_Slide.Value = -1;
else
  handles.Y_Offset_Slide.Value = 1;
end

%   send to board
formatSpec = 'SO,Y,%d';
frame = sprintf(formatSpec,OffsetPWM);
fprintf(handles.s,frame);
 get_confirm(handles);


% --- Executes on button press in radiobutton_Plot.
function radiobutton_Plot_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton_Plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton_Plot

Time_ms = 0;
X_u_PWM = 0; X_est1_Pxl = 0; X_est2 = 0; X_est3 = 0; X_P1 = 0; X_P2 = 0; X_meas_Pxl = 0; X_diff = 0;
Y_u_PWM = 0; Y_est11_Pxl = 0; Y_est2 = 0; Y_est3 = 0; Y_P1 = 0; PX2 = 0; Y_meas_Pxl = 0; Y_diff = 0;

T = table(Time_ms,X_u_PWM,X_est1_Pxl,X_est2,X_est3,X_P1,X_P2,X_meas_Pxl,X_diff,...
  Y_u_PWM,Y_est11_Pxl,Y_est2,Y_est3,Y_P1,PX2,Y_meas_Pxl,Y_diff);

filename = char(datetime('now','Format','yyMMdd_HHmmss'));
currentDir = pwd;
if (~exist('Log','dir'))
mkdir('Log');
end
fileID = [currentDir '\Log\' filename '.csv'];
writetable(T,fileID);

% to read only first column, from the third line to line number 580.
% M = dlmread('170407_184050.csv',',',[2 0 580 0]);
% to read all column, from the second line to the end.
% M = dlmread('170407_184050.csv',',',1, 0);

frame_cnt = 0; index_cnt = 0;
hX_Pos = subplot(handles.X_Pos);
hX_Speed = subplot(handles.X_Speed);
hX_KF = subplot(handles.X_KF);

hY_Pos = subplot(handles.Y_Pos);
hY_Speed = subplot(handles.Y_Speed);
hY_KF = subplot(handles.Y_KF);

data_read_plot = zeros(10,18);



while(get(hObject,'Value') && handles.radiobutton_RunMode.Value && ...
      ~handles.radiobutton_SettingMode.Value &&...
      ~handles.radiobutton_StandbyMode.Value)
    
  
  fprintf(handles.s,'P');     % request data for plotting
  pause(0.01);
  data = get_confirm(handles);
  
  if ~isempty(data)
    index_cnt = index_cnt + 1;
    if(index_cnt == 1) 
        data_read_plot(1,:) = data_read_plot(10,:);
        index_cnt = 2;
    end
%     frame_cnt = frame_cnt + index_cnt;

    data_read = str2num(data(5:end));
    dlmwrite(fileID,data_read(2:end),'-append') % don't save ORDER

    data_read_plot(index_cnt,:) = data_read;
    if(index_cnt == 10)  % plot every 0.5 second
        index_cnt = 0;
        
        %   PlotData(data,handles)

        %% process frame input and then plot them
        % Frame("PXY,%u,%u,%d,%d,%d,%d,%d,%d,%d,%d",
        %       (",%u,%u,%d,%d,%d,%d,%d,%d,%d,%d\n"
        % |PXY,ORDER,Ts_ms,Xu_PWM,X1_Pxl,X2,X3,PX1,PX2,Xref_Pxl,Xdiff|
        % |Yu_PWM,Y1_Pxl,Y2,Y3,PX1,PX2,Yref_Pxl,Ydiff,LF|

        %% extract data
%         data_read = str2num(data(5:end));
%         ORDER = data_read(1);
%         time_ms = data_read(2);

%         Xu_pwm = data_read(3);
%         XX1 = data_read(4);
%         XX2 = data_read(5);
%         XX3 = data_read(6);
%         XP1 = data_read(7);
%         XP2 = data_read(8);
%         Xref = data_read(9);
%         Xdiff = data_read(10);
% 
%         Yu_pwm = data_read(11);
%         YX1 = data_read(12);
%         YX2 = data_read(13);
%         YX3 = data_read(14);
%         YP1 = data_read(15);
%         YP2 = data_read(16);
%         Yref = data_read(17);
%         Ydiff = data_read(18);

        %% Plot data
        % plot data with time variance

        %%
        % time axis
        time = data_read_plot(:,2)/1000;        %time_ms/1000
        if(max(time) < 48) 
            X_Min = 0; 
            X_Max = 50;
        else 
            X_Min = max(time) -48; 
            X_Max = max(time) + 2;
        end

        %%
        % plot X position
            xlim(hX_Pos,[X_Min X_Max]);
            plot(hX_Pos,time,data_read_plot(:,9),'b-',time,data_read_plot(:,4),'r-');
            
        % plot X speed
            xlim(hX_Speed,[X_Min X_Max]);
            plot(hX_Speed,time,data_read_plot(:,10),'b-',time,data_read_plot(:,5),'r-');

        % plot X kalman

%             xlim(hX_KF,[X_Min X_Max]);
%             plot(hX_KF,time,data_read_plot(:,7),'b-',time,data_read_plot(:,8),'r-');
            
        %%
        % % plot Y position
            xlim(hY_Pos,[X_Min X_Max]);
            plot(hY_Pos,time,data_read_plot(:,17),'b-',time,data_read_plot(:,12),'r-');

        % plot Y speed
            xlim(hY_Speed,[X_Min X_Max]);
            plot(hY_Speed,time,data_read_plot(:,18),'b-',time,data_read_plot(:,13),'r-');


        % % plot Y kalman
% 
%             xlim(hY_KF,[X_Min X_Max]);
%             plot(hY_KF,time,data_read_plot(:,15),'b-',time,data_read_plot(:,16),'r-');
            
            drawnow;
    end
  end
  
  
end
handles.popup_status.String = ['Stop plotting'];


% --- Executes on selection change in popup_KModel.
function popup_KModel_Callback(hObject, eventdata, handles)
% hObject    handle to popup_KModel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popup_KModel contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popup_KModel

K_contents = cellstr(handles.popup_KModel.String);
K_index = handles.popup_KModel.Value;
K = cell2mat(K_contents(K_index));
if strcmp(K,'Rad-Met')
    handles.Model_K = -0.8829;
elseif strcmp(K,'Rad-Pxl')
    handles.Model_K = -1222.8;
elseif strcmp(K,'Deg-Pxl')
    handles.Model_K = -21.265;
end


% --- Executes during object creation, after setting all properties.
function COM_Connect_CreateFcn(hObject, eventdata, handles)
% hObject    handle to COM_Connect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
