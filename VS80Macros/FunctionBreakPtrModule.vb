Imports System
Imports EnvDTE
Imports EnvDTE80
Imports System.Diagnostics

Imports System.IO
Imports System.Text.RegularExpressions



Public Module FunctionBreakPtrModule

    Function GetOutputWindowPane(ByVal Name As String, Optional ByVal show As Boolean = True) As OutputWindowPane
        Dim window As Window
        Dim outputWindow As OutputWindow
        Dim outputWindowPane As OutputWindowPane

        window = DTE.Windows.Item(EnvDTE.Constants.vsWindowKindOutput)
        If show Then window.Visible = True
        outputWindow = window.Object
        Try
            outputWindowPane = outputWindow.OutputWindowPanes.Item(Name)
        Catch e As System.Exception
            outputWindowPane = outputWindow.OutputWindowPanes.Add(Name)
        End Try
        outputWindowPane.Activate()
        Return outputWindowPane
    End Function


    Sub ClipboardMacro()
        'DTE.Commands.Raise("{AA61C329-D559-468F-8F0F-4F03896F704D}", 2, Customin, Customout)
        'DTE.Commands.Raise("{AA61C329-D559-468F-8F0F-4F03896F704D}", 5, Customin, Customout)
        DTE.Windows.Item("GameCommClient.cpp").Activate()
        DTE.Find.FindWhat = "printf"
        DTE.ExecuteCommand("Edit.FindNext")
        DTE.ActiveDocument.Selection.EndOfLine(True)
        DTE.ActiveDocument.Selection.Copy()
        DTE.ActiveDocument.Selection.EndOfLine()
        DTE.ActiveDocument.Selection.NewLine()
        DTE.ActiveDocument.Selection.Paste()
        DTE.ExecuteCommand("Edit.FindPrevious")
        DTE.ActiveDocument.Selection.Text = "TraceError"
        DTE.Find.FindWhat = "\n"
        DTE.ExecuteCommand("Edit.FindNext")

        DTE.ActiveDocument.Selection.Text = ""




        '       DTE.Find.MatchCase = False
        'DTE.Find.MatchWholeWord = False
        'DTE.Find.Backwards = False
        'DTE.Find.MatchInHiddenText = True
        'DTE.Find.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxLiteral
        '       DTE.Find.Action = vsFindAction.vsFindActionFind


        '	If(DTE.Find.Execute() = vsFindResult.vsFindResultNotFound) Then
        'Throw New System.Exception("vsFindResultNotFound")
        'End If
        '	DTE.Windows.Item("{CF2DDC32-8CAD-11D2-9302-005345000000}").Close
        '	DTE.ActiveDocument.Selection.Delete
    End Sub

    Sub Denny()
        Dim textSelection As EnvDTE.TextSelection
        Dim codeElement As EnvDTE.CodeElement
        Dim documentCur As EnvDTE.Document

        DTE.ExecuteCommand("Debug.ToggleBreakpoint")

        textSelection = DTE.ActiveWindow.Selection
        documentCur = DTE.ActiveWindow.Document

        codeElement = textSelection.ActivePoint.CodeElement(vsCMElement.vsCMElementFunction)

        Dim outputWinPane As EnvDTE.OutputWindowPane

        outputWinPane = GetOutputWindowPane("Debugger")
        outputWinPane.OutputString(codeElement.Name() + codeElement.GetStartPoint.Line.ToString() + vbCrLf)
        outputWinPane.OutputString(documentCur.Name() + textSelection.ActivePoint.Line.ToString() + vbCrLf)

        Dim breakPoints As EnvDTE.Breakpoints
        Dim breakPoint As EnvDTE.Breakpoint
        'DTE.Debugger.Breakpoints.Add(codeElement.Name(), documentCur.Name(), textSelection.ActivePoint.Line())
        'breakPoints = DTE.Debugger.Breakpoints.Add(codeElement.Name(), "", textSelection.ActivePoint.Line())
        'breakPoints = DTE.Debugger.Breakpoints.Add("", documentCur.Name(), textSelection.ActivePoint.Line())
        'breakPoint = breakPoints.Item(1)

        breakPoints = DTE.Debugger.Breakpoints
        If (breakPoints.Count > 0) Then

            Dim bp As EnvDTE.Breakpoint
            For Each bp In breakPoints
                If (bp.FileLine() = textSelection.ActivePoint.Line()) Then
                    breakPoint = bp
                End If

            Next
        Else
            outputWinPane.OutputString("No breakpoints are set" + vbCrLf)
        End If


        Dim functionLineOffset As String
        functionLineOffset = breakPoint.FunctionLineOffset.ToString()

        outputWinPane.OutputString("{" + breakPoint.FunctionName() + "}@" + functionLineOffset + vbCrLf)

        breakPoint.Delete()

        Dim funcName As String
        'outputWinPane.OutputString(codeElement.FullName() + vbCrLf)
        'outputWinPane.OutputString("{" + codeElement.FullName() + "}@" + functionLineOffset + vbCrLf)
        breakPoints = DTE.Debugger.Breakpoints.Add(codeElement.FullName())
        'breakPoint = breakPoints.Item(1)

        'breakPoint.FunctionLineOffset = functionLineOffset
        'breakPoints = DTE.Debugger.Breakpoints.Add(codeElement.FullName() + functionLineOffset)

        'DTE.ExecuteCommand("편집기상황에맞는메뉴.코드창.중단점.중단점위치", breakPoint)


    End Sub

    Sub FuncIter()
        Dim outputWinPane As EnvDTE.OutputWindowPane

        outputWinPane = GetOutputWindowPane("Debugger")

        Dim solution As Solution = DTE.Solution

        For Each project As Project In solution.Projects
            outputWinPane.OutputString(project.Name + vbCrLf)
        Next

        For Each codeElement As CodeElement In DTE.ActiveDocument.ProjectItem.FileCodeModel.CodeElements
            If TypeOf codeElement Is CodeFunction Then
                outputWinPane.OutputString(codeElement.FullName + vbCrLf)
            End If
        Next


    End Sub


    Sub Macro1()
        Dim textSelection As EnvDTE.TextSelection
        Dim codeElement As EnvDTE.CodeElement

        textSelection = DTE.ActiveWindow.Selection

        codeElement = textSelection.ActivePoint.CodeElement(vsCMElement.vsCMElementFunction)

        Dim outputWinPane As EnvDTE.OutputWindowPane

        outputWinPane = GetOutputWindowPane("Debugger")
        outputWinPane.OutputString(codeElement.Name() + codeElement.GetStartPoint.Line.ToString())
        outputWinPane.OutputString(vbCrLf)

        'Dim clipBoard As System.Windows.Forms.Clipboard
        'clipBoard.SetText(codeElement.Name())
    End Sub

    Sub BreakpointFindResults()
        Dim findResultsWindow As Window = DTE.Windows.Item(Constants.vsWindowKindFindResults1)

        Dim selection As TextSelection
        selection = findResultsWindow.Selection
        selection.SelectAll()

        Dim findResultsReader As New StringReader(selection.Text)
        Dim findResult As String = findResultsReader.ReadLine()

        Dim findResultRegex As New Regex("(?<Path>.*?)\((?<LineNumber>\d+)\):")

        While Not findResult Is Nothing
            Dim findResultMatch As Match = findResultRegex.Match(findResult)

            If findResultMatch.Success Then
                Dim path As String = findResultMatch.Groups.Item("Path").Value
                Dim lineNumber As Integer = Integer.Parse(findResultMatch.Groups.Item("LineNumber").Value)

                Try
                    DTE.Debugger.Breakpoints.Add("", path, lineNumber)
                Catch ex As Exception
                    ' breakpoints can't be added everywhere
                End Try
            End If

            findResult = findResultsReader.ReadLine()
        End While
    End Sub
End Module

