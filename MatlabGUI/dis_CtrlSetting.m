function dis_CtrlSetting(handles)

% offset calibration
handles.X_Offset_Slide.Enable = 'off';
handles.Y_Offset_Slide.Enable = 'off';
handles.edit_XDeg.Enable  = 'off';
handles.edit_YDeg.Enable  = 'off';
handles.edit_Xus.Enable  = 'off';
handles.edit_Yus.Enable  = 'off';

% controller feature
handles.radiobutton_CtrOder2nd.Enable = 'off';
handles.radiobutton_CtrOder3rd.Enable = 'off';
handles.edit_CtrPole1.Enable  = 'off';
handles.edit_CtrPole2.Enable  = 'off';
handles.edit_CtrPole3.Enable  = 'off';
handles.pushbutton_PolePlace.Enable  = 'off';
handles.popup_KModel.Enable  = 'off';


handles.edit_FBgain1.Enable  = 'off';
handles.edit_FBgain2.Enable  = 'off';
handles.edit_FBgain3.Enable  = 'off';
handles.edit_CtrFilter.Enable  = 'off';


end