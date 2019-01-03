/* ctm-model.c
 *
 * Copyright 2018 Isaque Galdino <igaldino@gmail.com>
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

#include "ctm-model.h"

struct _CtmModel
{
  GObject  parent_instance;
  CtmDB   *db;
};

G_DEFINE_TYPE (CtmModel, ctm_model, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_DB,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

CtmModel *
ctm_model_new (void)
{
  return g_object_new (CTM_TYPE_MODEL,
                       "db", ctm_app_get_db (CTM_APP_DEFAULT),
                       NULL);
}

/*static void
ctm_model_finalize (GObject *object)
{
  CtmModel *self = (CtmModel *)object;

  G_OBJECT_CLASS (ctm_model_parent_class)->finalize (object);
}*/

static void
ctm_model_get_property (GObject    *object,
                        guint       prop_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  CtmModel *self = CTM_MODEL (object);

  switch (prop_id)
    {
    case PROP_DB:
      g_value_set_object (value, self->db);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
ctm_model_set_property (GObject      *object,
                        guint         prop_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  CtmModel *self = CTM_MODEL (object);

  switch (prop_id)
    {
    case PROP_DB:
      self->db = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
ctm_model_class_init (CtmModelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  /*object_class->finalize = ctm_model_finalize;*/
  object_class->get_property = ctm_model_get_property;
  object_class->set_property = ctm_model_set_property;

  properties[PROP_DB] = g_param_spec_object ("db",
                                             "Database",
                                             "The Database",
                                             CTM_TYPE_DB,
                                             G_PARAM_READWRITE |
                                             G_PARAM_CONSTRUCT |
                                             G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
ctm_model_init (CtmModel *self)
{
}

GtkListStore *
ctm_model_person_new (CtmModel *self)
{
  return gtk_list_store_new (CTM_MODEL_PERSON_COLUMN_NOO,
                             G_TYPE_UINT,
                             G_TYPE_STRING);
}

void
ctm_model_person_get_all (CtmModel     *self,
                          GtkListStore *store)
{
  CtmPerson   *person = NULL;
  GPtrArray   *data   = NULL;
  GtkTreeIter  iter;

  gtk_list_store_clear (store);

  data = ctm_db_get_all_people (self->db);
  if (!data)
    return;

  for (int index = 0; index < data->len; index++)
    {
      person = CTM_PERSON (g_ptr_array_index (data, index));
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
                          CTM_MODEL_PERSON_COLUMN_ID,   ctm_person_get_id (person),
                          CTM_MODEL_PERSON_COLUMN_NAME, ctm_person_get_name (person),
                          -1);

    }
  g_ptr_array_free (data, TRUE);
}

GtkListStore *
ctm_model_project_new (CtmModel *self)
{
  return gtk_list_store_new (CTM_MODEL_PROJECT_COLUMN_NOO,
                             G_TYPE_UINT,
                             G_TYPE_STRING,
                             G_TYPE_STRING);

}

void
ctm_model_project_get_all (CtmModel     *self,
                           GtkListStore *store)
{
  CtmProject  *project = NULL;
  GPtrArray   *data    = NULL;
  GtkTreeIter  iter;

  gtk_list_store_clear (store);

  data = ctm_db_get_all_projects (self->db);
  if (!data)
    return;

  for (int index = 0; index < data->len; index++)
    {
      project = CTM_PROJECT (g_ptr_array_index (data, index));
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
                          CTM_MODEL_PROJECT_COLUMN_ID,          ctm_project_get_id (project),
                          CTM_MODEL_PROJECT_COLUMN_NAME,        ctm_project_get_name (project),
                          CTM_MODEL_PROJECT_COLUMN_DESCRIPTION, ctm_project_get_description (project),
                          -1);

    }
  g_ptr_array_free (data, TRUE);
}

GtkListStore *
ctm_model_task_new (CtmModel *self)
{
  return gtk_list_store_new (CTM_MODEL_TASK_COLUMN_NOO,
                             G_TYPE_UINT,
                             G_TYPE_UINT,
                             G_TYPE_STRING,
                             G_TYPE_UINT,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_UINT,
                             G_TYPE_UINT,
                             G_TYPE_UINT,
                             G_TYPE_UINT,
                             G_TYPE_UINT);
}

void
ctm_model_task_get_all (CtmModel     *self,
                        GtkListStore *store)
{
  CtmTask      *task    = NULL;
  CtmPerson    *person  = NULL;
  CtmProject   *project = NULL;
  GPtrArray    *data    = NULL;
  GtkTreeIter   iter;

  gtk_list_store_clear (store);

  data = ctm_db_get_all_tasks (self->db);
  if (!data)
    return;

  for (int index = 0; index < data->len; index++)
    {
      task = CTM_TASK (g_ptr_array_index (data, index));
      person = ctm_db_get_person_by_id (self->db, ctm_task_get_person_id (task));
      project = ctm_db_get_project_by_id (self->db, ctm_task_get_project_id (task));

      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
                          CTM_MODEL_TASK_COLUMN_ID,           ctm_task_get_id (task),
                          CTM_MODEL_TASK_COLUMN_PERSON_ID,    ctm_task_get_person_id (task),
                          CTM_MODEL_TASK_COLUMN_PERSON_NAME,  ctm_person_get_name (person),
                          CTM_MODEL_TASK_COLUMN_PROJECT_ID,   ctm_task_get_project_id (task),
                          CTM_MODEL_TASK_COLUMN_PROJECT_NAME, ctm_project_get_name (project),
                          CTM_MODEL_TASK_COLUMN_DESCRIPTION,  ctm_task_get_description (task),
                          CTM_MODEL_TASK_COLUMN_NOTES,        ctm_task_get_notes (task),
                          CTM_MODEL_TASK_COLUMN_BEGIN,        ctm_task_get_begin (task),
                          CTM_MODEL_TASK_COLUMN_END,          ctm_task_get_end (task),
                          CTM_MODEL_TASK_COLUMN_DUE,          ctm_task_get_due (task),
                          CTM_MODEL_TASK_COLUMN_STATUS,       ctm_task_get_status (task),
                          CTM_MODEL_TASK_COLUMN_PRIORITY,     ctm_task_get_priority (task),
                          -1);
      g_clear_object (&person);
      g_clear_object (&project);
    }
  g_ptr_array_free (data, TRUE);
}

GtkListStore *
ctm_model_event_new (CtmModel *self)
{
  return gtk_list_store_new (CTM_MODEL_EVENT_COLUMN_NOO,
                             G_TYPE_UINT,
                             G_TYPE_UINT,
                             G_TYPE_STRING,
                             G_TYPE_UINT,
                             G_TYPE_FLOAT,
                             G_TYPE_STRING);
}

void
ctm_model_event_get_all (CtmModel     *self,
                         GtkListStore *store)
{
  CtmEvent    *event = NULL;
  CtmTask     *task  = NULL;
  GPtrArray   *data  = NULL;
  GtkTreeIter  iter;

  gtk_list_store_clear (store);

  data = ctm_db_get_all_events (self->db);
  if (!data)
    return;

  for (int index = 0; index < data->len; index++)
    {
      event = CTM_EVENT (g_ptr_array_index (data, index));
      task = ctm_db_get_task_by_id (self->db, ctm_event_get_task_id (event));

      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
                          CTM_MODEL_EVENT_COLUMN_ID,               ctm_event_get_id (event),
                          CTM_MODEL_EVENT_COLUMN_TASK_ID,          ctm_event_get_task_id (event),
                          CTM_MODEL_EVENT_COLUMN_TASK_DESCRIPTION, ctm_task_get_description (task),
                          CTM_MODEL_EVENT_COLUMN_WHEN,             ctm_event_get_when (event),
                          CTM_MODEL_EVENT_COLUMN_TIME,             ctm_event_get_time (event),
                          CTM_MODEL_EVENT_COLUMN_NOTES,            ctm_event_get_notes (event),
                          -1);
      g_clear_object (&task);
    }
  g_ptr_array_free (data, TRUE);
}

