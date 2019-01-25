/* ctm-main-window.c
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

#include "ctm-main-window.h"
#include "ctm-model.h"

struct _CtmMainWindow
{
  GtkApplicationWindow  parent_instance;
  GtkHeaderBar         *header;
  GtkTreeView          *task_list;
  GtkStack             *filter_stack;
  GtkComboBox          *project_combo;
  GtkComboBox          *person_combo;
  CtmModel             *model;
};

G_DEFINE_TYPE (CtmMainWindow, ctm_main_window, GTK_TYPE_APPLICATION_WINDOW)

static void            clear_filter_popover           (CtmMainWindow     *self);

static void            edit_task_panel                (CtmMainWindow     *self,
                                                       GtkTreePath       *path);

static gboolean        filter_task_list               (GtkTreeModel      *model,
                                                       GtkTreeIter       *iter,
                                                       gpointer           data);

static void            on_clear_filter_button_clicked (GtkWidget         *widget,
                                                       gpointer           data);

static void            on_filter_button_clicked       (GtkWidget         *widget,
                                                       gpointer           data);

static void            on_list_row_activated          (GtkTreeView       *view,
                                                       GtkTreePath       *path,
                                                       GtkTreeViewColumn *column,
                                                       gpointer           data);

static void            on_new_button_clicked          (GtkWidget         *widget,
                                                       gpointer           data);

static void            on_set_filter_button_clicked   (GtkWidget         *widget,
                                                       gpointer           data);

static void            show_clear_filter_button       (CtmMainWindow     *self);

static void            show_filter_popover            (CtmMainWindow     *self);

static void            show_task_list                 (CtmMainWindow     *self);

static void            show_task_panel                (CtmMainWindow     *self);

CtmMainWindow *
ctm_main_window_new (CtmApp *app)
{
  return g_object_new (CTM_TYPE_MAIN_WINDOW, "application", app, NULL);
}

static void
ctm_main_window_finalize (GObject *object)
{
  CtmMainWindow *self = CTM_MAIN_WINDOW (object);

  g_clear_object (&self->model);

  G_OBJECT_CLASS (ctm_main_window_parent_class)->finalize (object);
}

static void
ctm_main_window_class_init (CtmMainWindowClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = ctm_main_window_finalize;

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/CoyoteTM/ctm-main-window.ui");
  gtk_widget_class_bind_template_child (widget_class, CtmMainWindow, header);
  gtk_widget_class_bind_template_child (widget_class, CtmMainWindow, task_list);
  gtk_widget_class_bind_template_child (widget_class, CtmMainWindow, filter_stack);
  gtk_widget_class_bind_template_child (widget_class, CtmMainWindow, project_combo);
  gtk_widget_class_bind_template_child (widget_class, CtmMainWindow, person_combo);
  gtk_widget_class_bind_template_callback (widget_class, on_new_button_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_filter_button_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_clear_filter_button_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_set_filter_button_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_list_row_activated);
}

static void
ctm_main_window_init (CtmMainWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->model = ctm_model_new ();

  show_task_list (self);
}

static void
show_task_list (CtmMainWindow *self)
{
  static gboolean           done     = FALSE;
  g_autoptr (GtkListStore)  store    = NULL;
  g_autoptr (GtkTreeModel)  filter   = NULL;
  g_autoptr (GtkTreeModel)  model    = NULL;
  GtkCellRenderer          *renderer = NULL;
  GtkTreeViewColumn        *column   = NULL;

  if (done)
    return;

  store = ctm_model_task_get_all (self->model);

  filter = gtk_tree_model_filter_new (GTK_TREE_MODEL (store), NULL);
  gtk_tree_model_filter_set_visible_func (GTK_TREE_MODEL_FILTER (filter),
                                          filter_task_list, self, NULL);

  model = gtk_tree_model_sort_new_with_model (filter);
  gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (model), CTM_MODEL_TASK_COLUMN_PRIORITY, GTK_SORT_ASCENDING);
  gtk_tree_view_set_model (self->task_list, model);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Description", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_DESCRIPTION,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_DESCRIPTION);
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (column, 300);
  gtk_tree_view_column_set_expand (column, TRUE);
  gtk_tree_view_column_set_resizable (column, TRUE);
  gtk_tree_view_append_column (self->task_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Status", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_STATUS_STRING,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_STATUS);
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (column, 100);
  gtk_tree_view_column_set_resizable (column, TRUE);
  gtk_tree_view_append_column (self->task_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Priority", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_PRIORITY_STRING,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_PRIORITY);
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (column, 100);
  gtk_tree_view_column_set_resizable (column, TRUE);
  gtk_tree_view_append_column (self->task_list, column);

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Updated", renderer,
                                                     "text", CTM_MODEL_TASK_COLUMN_UPDATED_STRING,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, CTM_MODEL_TASK_COLUMN_UPDATED_SORT);
  gtk_tree_view_column_set_sort_order (column, GTK_SORT_DESCENDING);
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (column, 100);
  gtk_tree_view_column_set_resizable (column, TRUE);
  gtk_tree_view_append_column (self->task_list, column);

  gtk_tree_view_set_search_column (self->task_list, CTM_MODEL_TASK_COLUMN_DESCRIPTION);

  done = TRUE;
}

static gboolean
filter_task_list (GtkTreeModel *model,
                  GtkTreeIter  *iter,
                  gpointer      data)
{
  gboolean         visible               = TRUE;
  CtmMainWindow   *self                  = CTM_MAIN_WINDOW (data);
  const char      *selected_project_name = NULL;
  const char      *selected_person_name  = NULL;
  g_autofree char *project_name          = NULL;
  g_autofree char *person_name           = NULL;

  selected_project_name = gtk_combo_box_get_active_id (self->project_combo);
  if (selected_project_name)
    {
      gtk_tree_model_get (model, iter, CTM_MODEL_TASK_COLUMN_PROJECT_NAME, &project_name, -1);
      if (project_name && strcmp (selected_project_name, project_name) != 0)
        {
          visible = FALSE;
        }
    }

  selected_person_name = gtk_combo_box_get_active_id (self->person_combo);
  if (selected_person_name)
    {
      gtk_tree_model_get (model, iter, CTM_MODEL_TASK_COLUMN_PERSON_NAME, &person_name, -1);
      if (person_name && strcmp (selected_person_name, person_name) != 0)
        {
          visible = FALSE;
        }
    }

  return visible;
}

static void
show_filter_popover (CtmMainWindow *self)
{
  static gboolean           done          = FALSE;
  g_autoptr (GtkListStore)  project_store = NULL;
  g_autoptr (GtkListStore)  person_store  = NULL;
  GtkCellRenderer          *renderer      = NULL;

  if (done)
    return;

  project_store = ctm_model_project_get_all (self->model);
  gtk_combo_box_set_model (self->project_combo, GTK_TREE_MODEL (project_store));
  gtk_combo_box_set_id_column (self->project_combo, CTM_MODEL_PROJECT_COLUMN_NAME);

  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self->project_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (self->project_combo), renderer,
                                  "text", CTM_MODEL_PROJECT_COLUMN_NAME,
                                  NULL);

  person_store = ctm_model_person_get_all (self->model);
  gtk_combo_box_set_model (self->person_combo, GTK_TREE_MODEL (person_store));
  gtk_combo_box_set_id_column (self->person_combo, CTM_MODEL_PERSON_COLUMN_NAME);

  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self->person_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (self->person_combo), renderer,
                                  "text", CTM_MODEL_PERSON_COLUMN_NAME,
                                  NULL);

  done = TRUE;
}

static void
show_clear_filter_button (CtmMainWindow *self)
{
  GtkTreeModel *model  = NULL;
  GtkTreeModel *filter = NULL;

  if (gtk_combo_box_get_active (self->project_combo) < 0 &&
      gtk_combo_box_get_active (self->person_combo) < 0)
    {
      return;
    }

  model = gtk_tree_view_get_model (self->task_list);
  filter = gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (model));
  gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER (filter));

  gtk_stack_set_visible_child_name (self->filter_stack, "clear-filter-page");
}

static void
clear_filter_popover (CtmMainWindow *self)
{
  GtkTreeModel *model  = NULL;
  GtkTreeModel *filter = NULL;

  gtk_combo_box_set_active (self->project_combo, -1);
  gtk_combo_box_set_active (self->person_combo, -1);

  model = gtk_tree_view_get_model (self->task_list);
  filter = gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (model));
  gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER (filter));

  gtk_stack_set_visible_child_name (self->filter_stack, "filter-page");
}

static void
show_task_panel (CtmMainWindow *self)
{
}

static void
edit_task_panel (CtmMainWindow   *self,
                 GtkTreePath *path)
{
}

static void
on_new_button_clicked (GtkWidget *widget,
                       gpointer   data)
{
  CtmMainWindow *self = CTM_MAIN_WINDOW (widget);

  show_task_panel (self);
}

static void
on_filter_button_clicked (GtkWidget *widget,
                          gpointer   data)
{
  CtmMainWindow *self = CTM_MAIN_WINDOW (widget);

  show_filter_popover (self);
}

static void
on_clear_filter_button_clicked (GtkWidget *widget,
                                gpointer   data)
{
  CtmMainWindow *self = CTM_MAIN_WINDOW (widget);

  clear_filter_popover (self);
}

static void
on_set_filter_button_clicked (GtkWidget *widget,
                              gpointer   data)
{
  CtmMainWindow *self = CTM_MAIN_WINDOW (widget);

  show_clear_filter_button (self);
}

static void
on_list_row_activated (GtkTreeView       *view,
                       GtkTreePath       *path,
                       GtkTreeViewColumn *column,
                       gpointer           data)
{
  CtmMainWindow *self = CTM_MAIN_WINDOW (data);

  show_task_panel (self);
  edit_task_panel (self, path);
}


