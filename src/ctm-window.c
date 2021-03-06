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
#include "ctm-model.h"

struct _CtmWindow
{
  GtkApplicationWindow  parent_instance;
  GtkStack             *main_stack;
  GtkStack             *list_stack;
  GtkTreeView          *tasks_list;
  GtkTreeView          *projects_list;
  GtkTreeView          *people_list;
  GtkTreeView          *events_list;
  CtmModel             *model;
};

G_DEFINE_TYPE (CtmWindow, ctm_window, GTK_TYPE_APPLICATION_WINDOW)

static void show_tasks_list       (CtmWindow *self);
static void show_projects_list    (CtmWindow *self);
static void show_people_list      (CtmWindow *self);
static void show_events_list      (CtmWindow *self);

static void on_list_stack_changed (GtkWidget *widget,
                                   gpointer   data);

CtmWindow *
ctm_window_new (CtmApp *app)
{
  return g_object_new (CTM_TYPE_WINDOW, "application", app, NULL);
}

static void
ctm_window_finalize (GObject *object)
{
  CtmWindow *self = CTM_WINDOW (object);

  g_clear_object (&self->model);

  G_OBJECT_CLASS (ctm_window_parent_class)->finalize (object);
}

static void
ctm_window_class_init (CtmWindowClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = ctm_window_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/CoyoteTM/ctm-window.ui");
  gtk_widget_class_bind_template_child (widget_class, CtmWindow, main_stack);
  gtk_widget_class_bind_template_child (widget_class, CtmWindow, list_stack);
  gtk_widget_class_bind_template_child (widget_class, CtmWindow, tasks_list);
  gtk_widget_class_bind_template_child (widget_class, CtmWindow, projects_list);
  gtk_widget_class_bind_template_child (widget_class, CtmWindow, people_list);
  gtk_widget_class_bind_template_child (widget_class, CtmWindow, events_list);
  gtk_widget_class_bind_template_callback (widget_class, on_list_stack_changed);
}

static void
ctm_window_init (CtmWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->model = ctm_model_new ();
  gtk_tree_view_set_model (self->tasks_list, GTK_TREE_MODEL (ctm_model_task_new (self->model)));
  gtk_tree_view_set_model (self->people_list, GTK_TREE_MODEL (ctm_model_person_new (self->model)));
  gtk_tree_view_set_model (self->projects_list, GTK_TREE_MODEL (ctm_model_project_new (self->model)));
  gtk_tree_view_set_model (self->events_list, GTK_TREE_MODEL (ctm_model_event_new (self->model)));

  show_tasks_list (self);
}

static void
show_tasks_list (CtmWindow *self)
{
  static gboolean    done = FALSE;
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  ctm_model_task_get_all (self->model, GTK_LIST_STORE (gtk_tree_view_get_model (self->tasks_list)));

  if (done)
    return;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Person", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_PERSON_NAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_PERSON_NAME);
  gtk_tree_view_append_column (self->tasks_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Project Name", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_PROJECT_NAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_PROJECT_NAME);
  gtk_tree_view_append_column (self->tasks_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Description", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_DESCRIPTION,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_DESCRIPTION);
  gtk_tree_view_append_column (self->tasks_list, column);

  done = TRUE;
}

static void
show_projects_list (CtmWindow *self)
{
  static gboolean    done = FALSE;
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  ctm_model_project_get_all (self->model, GTK_LIST_STORE (gtk_tree_view_get_model (self->projects_list)));

  if (done)
    return;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Name", renderer,
                                                     "text", CTM_MODEL_PROJECT_COLUMN_NAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_PROJECT_COLUMN_NAME);
  gtk_tree_view_append_column (self->projects_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Description", renderer,
                                                     "text", CTM_MODEL_PROJECT_COLUMN_DESCRIPTION,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_PROJECT_COLUMN_DESCRIPTION);
  gtk_tree_view_append_column (self->projects_list, column);

  done = TRUE;
}

static void
show_people_list (CtmWindow *self)
{
  static gboolean    done = FALSE;
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  ctm_model_person_get_all (self->model, GTK_LIST_STORE (gtk_tree_view_get_model (self->people_list)));

  if (done)
    return;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Name", renderer,
                                                     "text", CTM_MODEL_PERSON_COLUMN_NAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_PERSON_COLUMN_NAME);
  gtk_tree_view_append_column (self->people_list, column);

  done = TRUE;
}

static void
show_events_list (CtmWindow *self)
{
  static gboolean    done = FALSE;
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  ctm_model_event_get_all (self->model, GTK_LIST_STORE (gtk_tree_view_get_model (self->events_list)));

  if (done)
    return;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Task", renderer,
                                                     "text", CTM_MODEL_EVENT_COLUMN_TASK_DESCRIPTION,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_EVENT_COLUMN_TASK_DESCRIPTION);
  gtk_tree_view_append_column (self->events_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Notes", renderer,
                                                     "text", CTM_MODEL_EVENT_COLUMN_NOTES,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_EVENT_COLUMN_NOTES);
  gtk_tree_view_append_column (self->events_list, column);

  done = TRUE;
}

static void
on_list_stack_changed (GtkWidget *widget,
                       gpointer   data)
{
  CtmWindow *self = CTM_WINDOW (widget);
  GtkWidget *child = gtk_stack_get_visible_child (self->list_stack);

  if (child == GTK_WIDGET (self->people_list))
    {
      show_people_list (self);
    }
  else if (child == GTK_WIDGET (self->projects_list))
    {
      show_projects_list (self);
    }
  else if (child == GTK_WIDGET (self->tasks_list))
    {
      show_tasks_list (self);
    }
  else if (child == GTK_WIDGET (self->events_list))
    {
      show_events_list (self);
    }
}
