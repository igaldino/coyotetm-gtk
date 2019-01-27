/* ctm-task-window.c
 *
 * Copyright 2019 Isaque Galdino <igaldino@gmail.com>
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "ctm-task-window.h"

struct _CtmTaskWindow
{
  GtkWindow  parent_instance;
  CtmTask   *task;
};

G_DEFINE_TYPE (CtmTaskWindow, ctm_task_window, GTK_TYPE_WINDOW)

static void on_cancel_button_clicked (GtkWidget *widget,
                                      gpointer   data);

static void on_done_button_clicked   (GtkWidget *widget,
                                      gpointer   data);

CtmTaskWindow *
ctm_task_window_new (void)
{
  return g_object_new (CTM_TYPE_TASK_WINDOW, NULL);
}

static void
ctm_task_window_finalize (GObject *object)
{
  /*CtmTaskWindow *self = (CtmTaskWindow *)object;

  g_clear_object (&self->task);*/

  G_OBJECT_CLASS (ctm_task_window_parent_class)->finalize (object);
}

static void
ctm_task_window_class_init (CtmTaskWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = ctm_task_window_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/CoyoteTM/ctm-task-window.ui");
  gtk_widget_class_bind_template_callback (widget_class, on_cancel_button_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_done_button_clicked);
}

static void
ctm_task_window_init (CtmTaskWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  gtk_window_set_transient_for (GTK_WINDOW (self),
                                GTK_WINDOW (ctm_app_get_main_window (CTM_APP_DEFAULT)));
  gtk_window_set_modal (GTK_WINDOW (self), TRUE);
  gtk_window_present (GTK_WINDOW (self));
}

void
ctm_task_window_show_task (CtmTaskWindow *self,
                           CtmTask       *task)
{

}

static void
on_cancel_button_clicked (GtkWidget *widget,
                          gpointer   data)
{
  CtmTaskWindow *self = CTM_TASK_WINDOW (widget);

  gtk_window_close (GTK_WINDOW (self));
}

static void
on_done_button_clicked (GtkWidget *widget,
                        gpointer   data)
{
  CtmTaskWindow *self = CTM_TASK_WINDOW (widget);

  gtk_window_close (GTK_WINDOW (self));
}
