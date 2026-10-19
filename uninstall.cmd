/**/
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

TKFRAME = 'TKFrame'

say 'Quering (SYS_DLLS,LoadPerProcess) value...'
say ''

sysDLLs = SysIni('USER', 'SYS_DLLS', 'LoadPerProcess')
p = pos(TKFRAME, sysDLLs)

if (p > 0) then do
  say 'TKFrame dll has been already registered'
  say 'Removing...'
  SysIni('USER', 'SYS_DLLS', 'LoadPerProcess', delstr(sysDLLs, p, length(TKFRAME)))
  say 'Please reboot your system so changes will be active.'
  say ''
  say '***************************************************************'
  say '* After reboot delete TKFrame.dll from the OS2\DLL directory. *'
  say '***************************************************************'
end
else do
  say 'TKFrame not found'
end


