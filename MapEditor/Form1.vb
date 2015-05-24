Public Class Form1

    Private Sub Form1_Load(sender As Object, e As System.EventArgs) Handles Me.Load
        Dim dir As New IO.DirectoryInfo("C:\Users\LSF\Documents\Visual Studio 2010\Projects\hiho\Fight\Debug\Graphics\Tilesets")
        Dim file As IO.FileSystemInfo
        For Each file In dir.GetFileSystemInfos
            ListBox1.Items.Add(file.Name)
        Next
    End Sub
End Class
