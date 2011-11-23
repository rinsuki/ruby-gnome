/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Geoff Youngs, based on gtktextview.c by Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgtksourcemain.h"

/* Class: Gtk::SourceView
 * A view on a source.
 */

#define RG_TARGET_NAMESPACE cView
#define _SELF(self) (GTK_SOURCE_VIEW(RVAL2GOBJ(self)))

/*
 * Class method: new(buffer=nil)
 * buffer: a Gtk::SourceBuffer object.
 *
 * Creates a new Gtk::SourceView.  If buffer is not provided or nil, an empty
 * buffer will be created for you.  Note that one buffer can be shared among
 * many widgets.
 *
 * Returns: a newly created Gtk::SourceView object.
 */
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE buffer;
    GtkWidget *widget;

    rb_scan_args(argc, argv, "01", &buffer);

    if (NIL_P(buffer))
        widget = gtk_source_view_new();
    else
        widget = gtk_source_view_new_with_buffer(RVAL2GOBJ(buffer));

    RBGTK_INITIALIZE(self, widget);
    return self;
}

#ifdef HAVE_GTK_SOURCE_VIEW_GET_MARK_CATEGORY_BACKGROUND
/*
 * Method: get_mark_category_background(category)
 * category: a category (as a string).
 *
 * Gets the background color which is associated with the given category.
 *
 * Returns: a Gdk::Color object if found, or nil if not found.
 */
static VALUE
rg_get_mark_category_background(VALUE self, VALUE category)
{
    GdkColor color;
    gtk_source_view_get_mark_category_background(_SELF (self),
                         RVAL2CSTR(category),
                         &color);
    return GDKCOLOR2RVAL(&color);
}

/*
 * Method: set_mark_category_background(category, color)
 * category: a category (as a string).
 * color: a Gdk::Color.
 *
 * Sets given background color for mark category. If color is NULL,
 * the background color is unset.
 *
 * Returns: self.
 */
static VALUE
rg_set_mark_category_background(VALUE self, VALUE category, VALUE color)
{
    gtk_source_view_set_mark_category_background(_SELF (self),
                         RVAL2CSTR (category),
                         RVAL2GDKCOLOR((color)));
    return self;
}
# endif /* HAVE_GTK_SOURCE_VIEW_GET_MARK_CATEGORY_BACKGROUND */

void
Init_gtk_sourceview (VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS (GTK_SOURCE_TYPE_VIEW, "View", mGtkSource);

    RG_DEF_METHOD(initialize, -1);
#ifdef HAVE_GTK_SOURCE_VIEW_GET_MARK_CATEGORY_BACKGROUND
    RG_DEF_METHOD(get_mark_category_background, 1);
    RG_DEF_METHOD(set_mark_category_background, 2);
#endif
    G_DEF_SETTERS (RG_TARGET_NAMESPACE);

    G_DEF_CLASS(GTK_SOURCE_TYPE_SMART_HOME_END_TYPE, "SmartHomeEndType", RG_TARGET_NAMESPACE);
#ifdef HAVE_GTK_SOURCE_VIEW_GET_MARK_CATEGORY_BACKGROUND
    G_DEF_CLASS(GTK_SOURCE_TYPE_DRAW_SPACES_FLAGS, "DrawSpacesFlags", RG_TARGET_NAMESPACE);
#endif
}
