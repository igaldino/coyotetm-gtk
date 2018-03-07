/* ctm-window.c
 *
 * Copyright (C) 2017 Isaque Galdino <igaldino@gmail.com>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ctm-window.h"

struct _CtmWindow
{
  GtkApplicationWindow  parent_instance;
};

G_DEFINE_TYPE (CtmWindow, ctm_window, GTK_TYPE_APPLICATION_WINDOW)

CtmWindow *
ctm_window_new (CtmApp *app)
{
  return g_object_new (CTM_TYPE_WINDOW, "application", app, NULL);
}

static void
ctm_window_finalize (GObject *object)
{
  CtmWindow *self = (CtmWindow *)object;

  G_OBJECT_CLASS (ctm_window_parent_class)->finalize (object);
}

static void
ctm_window_class_init (CtmWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = ctm_window_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/CoyoteTM/ctm-window.ui");
}

static void
ctm_window_init (CtmWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

