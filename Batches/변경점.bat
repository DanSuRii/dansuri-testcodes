@start tortoiseproc /path:C:\vector\programtrunk /command:log

SET /P SvnRev="SvnRevision? : "
SET /A SvnPrevRev=%SvnRev%-1

echo %SvnRev% %SvnPrevRev%

echo.cmd /c tortoiseproc /command:showcompare /url1:"svn://10.29.50.214/vector/programtrunk" /revision1:%SvnPrevRev% /url2:"svn://10.29.50.214/vector/programtrunk" /revision2:%SvnRev% /unified > Diff%SvnRev%.txt
start .
