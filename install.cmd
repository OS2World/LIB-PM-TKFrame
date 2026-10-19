/**/
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

TKFRAME = 'TKFrame'

say 'Quering (SYS_DLLS,LoadPerProcess) value...'
sysDLLs = SysIni('USER', 'SYS_DLLS', 'LoadPerProcess')
p = pos(TKFRAME, sysDLLs)
if (p > 0) then
  say 'TKFrame dll has been already registered'
else do
  ret = insert(TKFRAME, sysDLLs, length(sysDLLs)-1)
  SysIni('USER', 'SYS_DLLS', 'LoadPerProcess', ret)
  say 'TKFrame dll has been registered'
  say 'Please reboot your system so changes will be active.'
end


