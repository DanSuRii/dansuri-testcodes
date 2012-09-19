IF EXIST boost-trunk goto update
goto checkout


:update
REM @echo update
svn update boost-trunk
goto end

:checkout
svn co http://svn.boost.org/svn/boost/trunk boost-trunk
@echo checkout

:end
pause