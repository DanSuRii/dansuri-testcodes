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
End Module
