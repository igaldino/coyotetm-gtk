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
  GtkWindow    parent_instance;
  GtkEntry    *description_entry;
  GtkComboBox *status_combo;
  GtkComboBox *priority_combo;
  GtkEntry    *due_entry;
  GtkEntry    *project_entry;
  GtkEntry    *person_entry;
  GtkEntry    *begin_entry;
  GtkEntry    *end_entry;
  GtkTextView *notes_entry;
  CtmModel    *model;
  CtmTask     *task;
};

G_DEFINE_TYPE (CtmTaskWindow, ctm_task_window, GTK_TYPE_WINDOW)

static void on_cancel_button_clicked (GtkWidget *widget,
                                      gpointer   data);

static void on_done_button_clicked   (GtkWidget *widget,
                                      gpointer   data);

static void populate_combos          (CtmTaskWindow *self);

static void populate_window          (CtmTaskWindow *self);

CtmTaskWindow *
ctm_task_window_new (void)
{
  return g_object_new (CTM_TYPE_TASK_WINDOW, NULL);
}

static void
ctm_task_window_finalize (GObject *object)
{
  CtmTaskWindow *self = (CtmTaskWindow *)object;

  g_clear_object (&self->model);

  G_OBJECT_CLASS (ctm_task_window_parent_class)->finalize (object);
}

static void
ctm_task_window_class_init (CtmTaskWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = ctm_task_window_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/CoyoteTM/ctm-task-window.ui");
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, description_entry);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, status_combo);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, priority_combo);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, due_entry);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, project_entry);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, person_entry);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, begin_entry);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, end_entry);
  gtk_widget_class_bind_template_child (widget_class, CtmTaskWindow, notes_entry);
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

  self->model = ctm_model_new ();

  populate_combos (self);
}

void
ctm_task_window_show_task (CtmTaskWindow *self,
                           CtmTask       *task)
{
  if (task)
    {
      self->task = task;
      populate_window (self);
    }
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

static void
populate_combos (CtmTaskWindow *self)
{
  g_autoptr (GtkListStore)  status_store = NULL;
  g_autoptr (GtkListStore)  priority_store = NULL;
  GtkCellRenderer          *renderer     = NULL;

  status_store = ctm_model_status_get_all (self->model);
  gtk_combo_box_set_model (self->status_combo, GTK_TREE_MODEL (status_store));
  gtk_combo_box_set_id_column (self->status_combo, CTM_MODEL_STATUS_COLUMN_NAME);

  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self->status_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (self->status_combo), renderer,
                                  "text", CTM_MODEL_STATUS_COLUMN_NAME,
                                  NULL);

  priority_store = ctm_model_priority_get_all (self->model);
  gtk_combo_box_set_model (self->priority_combo, GTK_TREE_MODEL (priority_store));
  gtk_combo_box_set_id_column (self->priority_combo, CTM_MODEL_PRIORITY_COLUMN_NAME);

  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self->priority_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (self->priority_combo), renderer,
                                  "text", CTM_MODEL_PRIORITY_COLUMN_NAME,
                                  NULL);
}

static void
populate_window (CtmTaskWindow *self)
{
  CtmStatusType   status   = ctm_task_get_status (self->task);
  CtmPriorityType priority = ctm_task_get_priority (self->task);

  gtk_entry_set_text (self->description_entry, ctm_task_get_description (self->task));
  gtk_combo_box_set_active_id (self->status_combo,
                               ctm_model_status_get_description (self->model, status));
  gtk_combo_box_set_active_id (self->priority_combo,
                               ctm_model_priority_get_description (self->model, priority));
}


