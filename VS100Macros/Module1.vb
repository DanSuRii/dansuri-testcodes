Imports System
Imports EnvDTE
Imports EnvDTE80
Imports EnvDTE90
Imports EnvDTE90a
Imports EnvDTE100
Imports System.Diagnostics

Public Module Module1

    Sub ViewInObjectBrowser()

        DTE.ExecuteCommand("Edit.SelectCurrentWord")
        DTE.ExecuteCommand("View.ObjectBrowser")
        DTE.ExecuteCommand("View.ObjectBrowserSearch", DTE.ActiveDocument.Selection.Text)

    End Sub

    Sub SearchFromGoogle()

        DTE.ExecuteCommand("Edit.SelectCurrentWord")
        Dim procStart As System.Diagnostics.ProcessStartInfo
        Dim startArgument As String = "http://google.co.kr/#q=" & DTE.ActiveDocument.Selection.Text()
        procStart = New System.Diagnostics.ProcessStartInfo("Chrome", startArgument)
        System.Diagnostics.Process.Start(procStart)

        'procStart.Arguments = 


    End Sub
End Module
