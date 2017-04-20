function en_CtrlSetting(handles)

% enable offset calibration feature
handles.X_Offset_Slide.Enable = 'on';
handles.Y_Offset_Slide.Enable = 'on';
handles.edit_XDeg.Enable  = 'on';
handles.edit_YDeg.Enable  = 'on';
handles.edit_Xus.Enable  = 'on';
handles.edit_Yus.Enable  = 'on';

% enable controller feature
%     handles.radiobutton_CtrOder2nd.Enable = 'on';
%     handles.radiobutton_CtrOder3rd.Enable = 'on';
    
handles.edit_CtrPole1.Enable  = 'on';
handles.edit_CtrPole2.Enable  = 'on';
handles.edit_CtrPole3.Enable  = 'on';
handles.pushbutton_PolePlace.Enable  = 'on';
handles.popup_KModel.Enable  = 'on';
    
handles.edit_FBgain1.Enable  = 'on';
handles.edit_FBgain2.Enable  = 'on';
handles.edit_FBgain3.Enable  = 'on';
handles.edit_CtrFilter.Enable  = 'on';

end