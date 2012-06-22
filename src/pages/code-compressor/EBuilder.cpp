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
 * @file EBuilder.cpp - Functions for pgCodeCompressor's E Builder.
**/

#include "EBuilder.h"

wxString buildECodeType(wxString input)
{
    wxArrayString lines = wxSplit(input, _T('\n'));

    // Generate an array with the last halves of each line.
    wxArrayString code;
//  if (lines.GetCount() > 1)
//  {
        // Multi-line format

        for (size_t i = 0; i < lines.GetCount(); ++i)
        {
            // Now is a good time as any to trim the lines
            wxString line = lines[i].Trim().Trim(false);
            if (!line.IsEmpty())
            {
                // Now grab everything after the space (it occurs at index 9)
                wxString end;
                for (size_t j = 9; j < line.Len(); ++j)
                    end += line[j];

                // Add it to code
                code.Add(end);
            }
        }
//  }
//  else
//  {
//      // :TODO: 2012-06-20 gbchaosmaster - Implement single line format
//  }

    // Build the first line of the output based on the number of input lines.
    long hexLength;
    // The first line needs data on the code that it's porting.
    // This includes the amount of lines, multiplied by 4, converted to hex:
    wxString::Format(_T("%i"), code.GetCount() * 4).ToLong(&hexLength, 16);
    // Convert that to a string:
    wxString hexLengthStr = wxString::Format(_T("%i"), hexLength);
    wxString primer = // Pad that with 0's to 8 characters long and set it up:
        _T("E2000000 ") + hexLengthStr.Pad(8 - hexLengthStr.Len(),
                                           _T('0'), false);

    // If the number of lines is odd, append a "00000000", we'll use it later
    if (code.GetCount() % 2 != 0)
        code.Add(_T("00000000"));

    // Build the output

    wxArrayString pairs = group(code);

    return primer + _T('\n') + wxJoin(pairs, _T('\n'));
}

/** Container Manipulation **/

wxArrayString group(wxArrayString arr, size_t n)
{
    wxString accumulator;
    wxArrayString output;

    size_t tracker = 0;
    for (size_t i = 0; i < arr.GetCount(); ++i)
    {
        accumulator += arr[i] + _T(' ');
        ++tracker;
        if (tracker >= n)
        {
            accumulator.Trim();
            output.Add(accumulator);
            accumulator.Clear();
            tracker = 0;
        }
    }

    return output;
}

/** Compatibility Functions **/

// wxJoin() and wxSplit() are not implemented in wxWidgets 2.8.11.
// Let's do it ourself.

wxString wxJoin(wxArrayString arr, wxChar sep)
{
    wxString output;

    for (size_t i = 0; i < arr.GetCount(); ++i)
    {
        output += arr[i];
        if (i < arr.GetCount() - 1)
            output += sep;
    }

    return output;
}
wxArrayString wxSplit(wxString str, wxChar sep)
{
    wxArrayString output;
    wxString accumulator = _T("");

    for (size_t i = 0; i < str.Len(); ++i)
    {
        if (str[i] == sep)
        {
            // If we encounter the separator (or if we reach the end of the
            // string), add the current state of the accumulator to the output
            // array and reset the accumulator.
            output.Add(accumulator);
            accumulator = _T("");
        }
        else if (i == str.Len() - 1)
        {
            // Last character of the string and it isn't the separator, so add
            // it to the accumulator and then add the accumulator to output.
            accumulator += str[i];
            output.Add(accumulator);
        }
        else // Non-separator character, add it to the accumulator.
            accumulator += str[i];
    }

    return output;
}
