@echo off
start tortoiseproc /path:C:\vector\programtrunk\src\VectorDB /command:log
SET /P SvnRev="SvnRevision? : "
md "%date% DBÀÛ¾÷ - %SvnRev%"