#pragma once

#include <windows.h>
#include <stdio.h>
#include <commdlg.h>

static OPENFILENAME ofn ;
void PopFileInitialize (HWND hwnd);
BOOL PopFileOpenDlg (HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
