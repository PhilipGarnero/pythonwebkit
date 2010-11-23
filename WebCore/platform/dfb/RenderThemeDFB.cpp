/*
 * This file is part of the WebKit project.
 *
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 *               2006 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2007 Ryan Leavengood <leavengood@gmail.com>
 * Copyright (C) 2009 Maxime Simon <simon.maxime@gmail.com>
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include "config.h"
#include "RenderThemeDFB.h"

#include "GraphicsContext.h"
#include "NotImplemented.h"



namespace WebCore {

PassRefPtr<RenderTheme> RenderThemeDFB::create()
{
    return adoptRef(new RenderThemeDFB());
}

PassRefPtr<RenderTheme> RenderTheme::themeForPage(Page*)
{
    static RenderTheme* renderTheme = RenderThemeDFB::create().releaseRef();
    return renderTheme;
}

RenderThemeDFB::RenderThemeDFB()
{
}

RenderThemeDFB::~RenderThemeDFB()
{
}

static bool supportsFocus(ControlPart appearance)
{
    switch (appearance) {
    case PushButtonPart:
    case ButtonPart:
    case TextFieldPart:
    case TextAreaPart:
    case SearchFieldPart:
    case MenulistPart:
    case RadioPart:
    case CheckboxPart:
        return true;
    default:
        return false;
    }
}

bool RenderThemeDFB::supportsFocusRing(const RenderStyle* style) const
{
    return supportsFocus(style->appearance());
}

Color RenderThemeDFB::platformActiveSelectionBackgroundColor() const
{
     notImplemented();
     return Color();
//    return Color(ui_color(B_CONTROL_HIGHLIGHT_COLOR));
}

Color RenderThemeDFB::platformInactiveSelectionBackgroundColor() const
{
     notImplemented();
     return Color();
//    return Color(ui_color(B_CONTROL_HIGHLIGHT_COLOR));
}

Color RenderThemeDFB::platformActiveSelectionForegroundColor() const
{
     notImplemented();
     return Color();
//    return Color(ui_color(B_CONTROL_TEXT_COLOR));
}

Color RenderThemeDFB::platformInactiveSelectionForegroundColor() const
{
     notImplemented();
     return Color();
//    return Color(ui_color(B_CONTROL_TEXT_COLOR));
}

Color RenderThemeDFB::platformTextSearchHighlightColor() const
{
     notImplemented();
     return Color();
//    return Color(ui_color(B_MENU_SELECTED_BACKGROUND_COLOR));
}

void RenderThemeDFB::systemFont(int propId, FontDescription&) const
{
    notImplemented();
}

bool RenderThemeDFB::paintCheckbox(RenderObject*, const PaintInfo& info, const IntRect& intRect)
{
     notImplemented();
     return false;
/*
    if (info.context->paintingDisabled())
        return false;

    rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
    BRect rect = intRect;
    BView* view = info.context->platformContext();

    if (!be_control_look)
        return false;

    be_control_look->DrawCheckBox(view, rect, rect, base);
    return true;
*/
}

void RenderThemeDFB::setCheckboxSize(RenderStyle* style) const
{
     notImplemented();
/*
    int size = 10;

    // If the width and height are both specified, then we have nothing to do.
    if (!style->width().isIntrinsicOrAuto() && !style->height().isAuto())
        return;

    // FIXME: A hard-coded size of 'size' is used. This is wrong but necessary for now.
    if (style->width().isIntrinsicOrAuto())
        style->setWidth(Length(size, Fixed));

    if (style->height().isAuto())
        style->setHeight(Length(size, Fixed));
*/
}

bool RenderThemeDFB::paintRadio(RenderObject*, const PaintInfo& info, const IntRect& intRect)
{
     notImplemented();
     return false;
/*
    if (info.context->paintingDisabled())
        return false;

    rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
    BRect rect = intRect;
    BView* view = info.context->platformContext();

    if (!be_control_look)
        return false;

    be_control_look->DrawRadioButton(view, rect, rect, base);
    return true;
*/
}

void RenderThemeDFB::setRadioSize(RenderStyle* style) const
{
     notImplemented();
/*
    // This is the same as checkboxes.
    setCheckboxSize(style);
*/
}

void RenderThemeDFB::adjustMenuListStyle(CSSStyleSelector*, RenderStyle* style, Element*) const
{
     notImplemented();
/*
    // Leave some space for the arrow.
    style->setPaddingRight(Length(22, Fixed));
    const int minHeight = 20;
    style->setMinHeight(Length(minHeight, Fixed));
*/
}

bool RenderThemeDFB::paintMenuList(RenderObject*, const PaintInfo&, const IntRect&)
{
    notImplemented();
    return false;
}

} // namespace WebCore
