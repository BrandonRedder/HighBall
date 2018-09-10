function [] = PlotGridData()
load('WindData.mat');

x = 17:49;
y = -100:-68;
p = [1000:-25:900, 850:-50:100, 70, 50, 30, 20, 10, 7, 5, 3, 2, 1];
t = 0:3:15;
p0 = 1;
t0 = 1;
u = reshape(ugrd(:,:,1,1),length(x),length(y));
v = reshape(vgrd(:,:,1,1),length(x),length(y));

[X,Y] = meshgrid(x,y);
% Create a figure and axes
f = figure('Units', 'pixels', 'Visible','off', 'Position', [200 100 900 550]);
ax = axes('Units','pixels', 'OuterPosition', [0 50 900 500]);
h = quiver(X,Y,u,v);

% Create slider
sld1 = uicontrol('Style', 'slider',...
    'Min',1,'Max',5,'Value',1,...
    'SliderStep',[1/5 1/5],...
    'Position', [500 20 300 20],...
    'Callback', @timeStep);

% Add a text uicontrol to label the slider.
txt1 = uicontrol('Style','text',...
    'Position',[500 45 300 20],...
    'String','Time 0');

% Create slider
sld2 = uicontrol('Style', 'slider',...
    'Min',1,'Max',31,'Value',1,...
    'SliderStep',[1/31 1/31],...
    'Position', [100 20 300 20],...
    'Callback', @presStep);

% Add a text uicontrol to label the slider.
txt2 = uicontrol('Style','text',...
    'Position',[100 45 300 20],...
    'String','Pressure 1000');

% Make figure visble after adding all components
f.Visible = 'on';
% This code uses dot notation to set properties.
% Dot notation runs in R2014b and later.
% For R2014a and earlier: set(f,'Visible','on');

    function timeStep(source,event)
        t0 = round(source.Value);
        u = reshape(ugrd(:,:,p0,t0),length(x),length(y));
        v = reshape(vgrd(:,:,p0,t0),length(x),length(y));
        set(h,'udata',u,'vdata',v)
        txt1.String = ['Time ', num2str(t(t0))];
    end

    function presStep(source,event)
        p0 = round(source.Value);
        u = reshape(ugrd(:,:,p0,t0),length(x),length(y));
        v = reshape(vgrd(:,:,p0,t0),length(x),length(y));
        set(h,'udata',u,'vdata',v)
        txt2.String = ['Pressure ', num2str(p(p0))];
    end
end


