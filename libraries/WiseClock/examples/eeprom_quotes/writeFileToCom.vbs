Const ForReading = 1
Const ForWriting = 2

'------------------------------------------------------------------------------------------------
' open USB serial port (COMx);
'
' If the serial monitor in Arduino IDE is open, you will get an "access denied" error.
' Just make sure that the serial monitor is closed (so bytes are not sent by the arduino board).
'------------------------------------------------------------------------------------------------

Set fso = CreateObject("Scripting.FileSystemObject")
Set com = fso.OpenTextFile("COM7:9600,N,8,1", ForWriting)


'---------------------------------------------
' read content of text file line by line;
' write line to COMx;
'---------------------------------------------

Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("c:\docs\quotes.txt", ForReading)

MsgBox("Ready to write file content to COM")

   Do While objFile.AtEndOfStream <> True
      '---------------------------------------------------------------------------------------------------------
      ' read 10 characters at a time; arduino serial buffer cannot take more than 32 characters;
      ' writing a character to eeprom takes about 11 ms (assuming that there is no serial.prints in the loop);
      ' therefore, after each batch of 10 chars sent to COM, we should wait no less than 110 ms;
      ' we use 200 to have a margin of safety;
      '---------------------------------------------------------------------------------------------------------
      strChars = objFile.Read(10)
      com.Write(strChars)
      WScript.Sleep(200)
   Loop


objFile.Close
com.Close()

MsgBox("Finished writing to COM")

 


