function [] = TestMailing(Data)
% Iridium messages can be sent and recieved through emails
% This uses matlab code to create *.sbd binary data files and sends them
% using the Iridium formatting from a team email. The files are kept in the
% Sent folder.

% The program sends the message directly to a gatech email address for now,
% but the correct email address is in the Send_Email function. The message
% will contain the ISU for our device, but currently holds a random device ISU. 

% The emails are directed to a folder in outlook which the program checks
% for new messages. Attachments are stored in the Incoming folder until the
% porgram reads them, then they are moved to the Read folder. For the
% program to work Outlook must be open on the computer.

% This function can be modified to take in an outgoing message and then
% begin checking for incoming messages until one is recieved. The recieved
% data can then be decoded and input to the simulation.

% Create Message
A = EncodeMessage(Data);

% Write data to binary .sbd filed
temp = datetime('now','TimeZone','local');
date = datestr(temp, 'yy-MM-dd_HH-mm-ss');
FID = fopen([date,'data.sbd'], 'w+');
fwrite(FID, A);
fclose('all');

% Send email with sbd Attachment
Send_Email([date,'data.sbd']);

% Move sent data to another folder
status = movefile([date,'data.sbd'], 'Sent');

% Read Unread Emails in the folder every second for 10 seconds
tries = 0;
recieved = false;
while tries < 10 && ~recieved  
    pause(1)
    mails = ReadOutlook(...
    'Folder', 'Senior Design SBD',...
    'Savepath', 'C:\Users\Kyle\Documents\Classes\Senior Design\Incoming',...
    'Read',1 , ...
    'Mark',1);
    tries = tries + 1;
    [r,c] = size(mails);
    if (r ~= 0)
        received = true;
    end
end

% Read data from incoming file and move it to another folder
cd Incoming
list = dir( '*data*.sbd');
cd ..
num = length(list);
while(num > 0)
    name = ['Incoming/',list(num).name;];
    FID = fopen(name, 'r');
    C{1,num} = fread(FID);
    C{2,num} = list(num).name;
    fclose('all');
    status = movefile(['Incoming/',list(num).name;], 'Read');
    num = num - 1;
end
end


