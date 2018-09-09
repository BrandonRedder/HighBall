function [] = Send_Email(file_name)
% Define these variables appropriately:
mail = 'teamHighBall@yahoo.com'; %Your Yahoo email address
password = 'MITLLHighBall2018';  %Your Yahoo password
setpref('Internet','SMTP_Server','smtp.mail.yahoo.com');
setpref('Internet','E_mail',mail);
setpref('Internet','SMTP_Username',mail);
setpref('Internet','SMTP_Password',password);
props = java.lang.System.getProperties;
props.setProperty('mail.smtp.auth','true');
props.setProperty('mail.smtp.socketFactory.class', 'javax.net.ssl.SSLSocketFactory');
props.setProperty('mail.smtp.socketFactory.port','465');

to_mail =  'data@sbd.iridium.com'; % use this email for messages
to_mail = 'kwatters6@gatech.edu'; % change to my email for testing
ISU = '304050607080903'; % ISU number here

sendmail(to_mail, ISU, 'Message is Unrelated', file_name)
end