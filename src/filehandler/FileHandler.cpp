/**
 * NDS Hacking Kit - A suite of tools for hacking Nintendo video games.
 * Copyright (C) 2012 Vinny Diehl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/

/**
 * @file FileHandler.cpp - Contains methods for file input.
**/

#include "FileHandler.h"

wxString FileHandler::ReadFile(wxWindow *parent, wxString *tgt,
                               wxTextCtrl *display, wxString wildcard,
                               wxString title)
{
    wxFileDialog dlgOpenFile(parent, title, wxGetHomeDir(), wxEmptyString,
                             wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    // Show the dialog. Once it closes, if they pressed cancel, stop.
    if (dlgOpenFile.ShowModal() == wxID_CANCEL) return _T("");

    wxString path = dlgOpenFile.GetPath();

    if (tgt != NULL)
    {
        // If they have a target variable to send the contents of the file to,
        // do so now. Otherwise it'll just return the path.
        wxPuts(_T("\nReading file: ") + path);
        wxFFile file(path);
        file.ReadAll(tgt);
        file.Close();
        wxPuts(_T("Success!"));
    }

    // If they want to output the file path into a TextCtrl, do so.
    if (display != NULL)
        display->SetValue(path);

    // Return the path of the file in case it's needed.
    return path;
}
