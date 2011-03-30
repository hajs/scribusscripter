/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


//Syntax: ATTRDEF( datatype, gettername, name, defaultvalue)

// NOTE: These attributes are preliminary.
ATTRDEF(double, width, Width, 0)
ATTRDEF(double, height, Height, 0)
ATTRDEF(double, margin, Margin, 0)
ATTRDEF(double, leftMargin, LeftMargin, 0)
ATTRDEF(double, rightMargin, RightMargin, 0)
ATTRDEF(double, topMargin, TopMargin, 0)
ATTRDEF(double, bottomMargin, BottomMargin, 0)
ATTRDEF(QString, backgroundColor, BackgroundColor, "None")
