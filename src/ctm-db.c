/* ctm-db.c
 *
 * Copyright (C) 2016 Isaque Galdino <igaldino@gmail.com>
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

#include "ctm-db.h"

struct _CtmDB
{
  GObject        parent_instance;
  char          *filename;
  GomAdapter    *adapter;
  GomRepository *repository;
};

G_DEFINE_TYPE (CtmDB, ctm_db, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_FILENAME,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
ctm_db_finalize (GObject *object)
{
  CtmDB *self = (CtmDB *)object;

  g_clear_pointer (&self->filename, g_free);
  g_clear_object (&self->adapter);

  G_OBJECT_CLASS (ctm_db_parent_class)->finalize (object);
}

static void
ctm_db_get_property (GObject    *object,
                     guint       prop_id,
                     GValue     *value,
                     GParamSpec *pspec)
{
  CtmDB *self = (CtmDB *)object;

  switch (prop_id)
    {
    case PROP_FILENAME:
      g_value_set_string (value, ctm_db_get_filename (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_db_set_property (GObject      *object,
                     guint         prop_id,
                     const GValue *value,
                     GParamSpec   *pspec)
{
  CtmDB *self = (CtmDB *)object;

  switch (prop_id)
    {
    case PROP_FILENAME:
      ctm_db_set_filename (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_db_class_init (CtmDBClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = ctm_db_finalize;
  object_class->get_property = ctm_db_get_property;
  object_class->set_property = ctm_db_set_property;

  properties [PROP_FILENAME] =
    g_param_spec_string ("filename",
                         "Filename",
                         "The filename of the DB",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, LAST_PROP, properties);
}

static void
ctm_db_init (CtmDB *self)
{
  self->filename = NULL;
  self->adapter = NULL;
}

CtmDB *
ctm_db_new (void)
{
  return g_object_new (CTM_TYPE_DB, NULL);
}

const char *
ctm_db_get_filename (CtmDB *self)
{
  return self->filename;
}

void
ctm_db_set_filename (CtmDB      *self,
                     const char *filename)
{
  if (g_strcmp0 (filename, self->filename))
    {
      g_clear_pointer (&self->filename, g_free);
      self->filename = g_strdup (filename);
    }
}

void
ctm_db_open (CtmDB *self)
{
  GList *object_types = NULL;
  g_autoptr(GError) error = NULL;

  self->adapter = gom_adapter_new ();
  gom_adapter_open_sync (self->adapter, self->filename, &error);
  if (error != NULL)
    {
      g_warning ("Error opening database file %s: (%d) %s\n", self->filename, error->code, error->message);
      return;
    }

  self->repository = gom_repository_new (self->adapter);
  object_types = g_list_prepend (object_types, GINT_TO_POINTER (CTM_TYPE_PERSON));
  object_types = g_list_prepend (object_types, GINT_TO_POINTER (CTM_TYPE_PROJECT));
  object_types = g_list_prepend (object_types, GINT_TO_POINTER (CTM_TYPE_TASK));
  object_types = g_list_prepend (object_types, GINT_TO_POINTER (CTM_TYPE_EVENT));
  gom_repository_automatic_migrate_sync (self->repository, 1, object_types, &error);
  if (error != NULL)
    {
      g_warning ("Error accessing database file %s structure: (%d) %s\n", self->filename, error->code, error->message);
      return;
    }

}

void
ctm_db_close (CtmDB *self)
{
  g_autoptr(GError) error = NULL;

  if (!self)
    return;

  gom_adapter_close_sync (self->adapter, &error);
  if (error != NULL)
    {
      g_warning ("Error closing database file %s: (%d) %s\n", self->filename, error->code, error->message);
      return;
    }

  g_clear_object (&self->repository);
  g_clear_object (&self->adapter);
}

int
ctm_db_save (CtmDB       *self,
             GomResource *resource)
{
  g_autoptr(GError) error = NULL;

  g_object_set (resource, "repository", self->repository, NULL);

  gom_resource_save_sync(resource, &error);
  if (error != NULL)
    {
      g_warning ("Error saving database table in %s: (%d) %s\n", self->filename, error->code, error->message);
      return FALSE;
    }
  return TRUE;
}

int
ctm_db_delete (CtmDB       *self,
               GomResource *resource)
{
  g_autoptr(GError) error = NULL;

  g_object_set (resource, "repository", self->repository, NULL);

  gom_resource_delete_sync(resource, &error);
  if (error != NULL)
    {
      g_warning ("Error deleting database table in %s: (%d) %s\n", self->filename, error->code, error->message);
      return FALSE;
    }
  return TRUE;
}

void
ctm_db_test (CtmDB *self)
{
  GPtrArray *data = NULL;
  CtmPerson *person = NULL;
  CtmProject *project = NULL;
  CtmTask *task = NULL;
  CtmEvent *event = NULL;
  char *description = NULL;
  int index, index2, index_person, index_project, index_task;

  /* create data */
  index_person = 1;
  person = ctm_person_new ();
  ctm_person_set_id (person, index_person++);
  ctm_person_set_name (person, "One Person");
  ctm_db_save (self, GOM_RESOURCE(person));
  g_clear_object (&person);

  index_project = 1;
  project = ctm_project_new ();
  ctm_project_set_id (project, index_project++);
  ctm_project_set_name (project, "project1");
  ctm_project_set_description (project, "Project One");
  ctm_db_save (self, GOM_RESOURCE(project));
  g_clear_object (&project);

  project = ctm_project_new ();
  ctm_project_set_id (project, index_project++);
  ctm_project_set_name (project, "project2");
  ctm_project_set_description (project, "Project Two");
  ctm_db_save (self, GOM_RESOURCE(project));
  g_clear_object (&project);

  for (index_task = 1, index = 1; index < index_person; index++)
    {
      for (index2 = 1; index2 < index_project; index2++, index_task++)
        {
          task = ctm_task_new ();
          description = g_strdup_printf ("T%02dP%02dP%02d", index_task, index, index2);
          ctm_task_set_id (task, index_task);
          ctm_task_set_person_id (task, index);
          ctm_task_set_project_id (task, index2);
          ctm_task_set_description (task, description);
          ctm_db_save (self, GOM_RESOURCE(task));
          g_clear_pointer (&description, g_free);
          g_clear_object (&task);
        }
    }

  for (index = 1; index < index_task; index++)
    {
      event = ctm_event_new ();
      description = g_strdup_printf ("E%02dT%02d", index, index);
      ctm_event_set_id (event, index);
      ctm_event_set_task_id (event, index);
      ctm_event_set_notes (event, description);
      ctm_db_save (self, GOM_RESOURCE(event));
      g_clear_pointer (&description, g_free);
      g_clear_object (&event);
    }

  /* list data */
  data = ctm_db_get_all_people (self);
  if (data)
    {
      for (index = 0; index < data->len; index++)
        {
          person = CTM_PERSON (g_ptr_array_index (data, index));
          g_print ("%d\t%s\n",
                   ctm_person_get_id (person),
                   ctm_person_get_name (person));
        }
      g_ptr_array_free (data, TRUE);
    }

  data = ctm_db_get_all_projects (self);
  if (data)
    {
      for (index = 0; index < data->len; index++)
        {
          project = CTM_PROJECT (g_ptr_array_index (data, index));
          g_print ("%d\t%s\t%s\n",
                   ctm_project_get_id (project),
                   ctm_project_get_name (project),
                   ctm_project_get_description (project));
        }
      g_ptr_array_free (data, TRUE);
    }

  data = ctm_db_get_all_tasks (self);
  if (data)
    {
      for (index = 0; index < data->len; index++)
        {
          task = CTM_TASK (g_ptr_array_index (data, index));
          g_print ("%d\t%s\n",
                   ctm_task_get_id (task),
                   ctm_task_get_description (task));
        }
      g_ptr_array_free (data, TRUE);
    }

  data = ctm_db_get_all_events (self);
  if (data)
    {
      for (index = 0; index < data->len; index++)
        {
          event = CTM_EVENT (g_ptr_array_index (data, index));
          g_print ("%d\t%s\n",
                   ctm_event_get_id (event),
                   ctm_event_get_notes (event));
        }
      g_ptr_array_free (data, TRUE);
    }
}

GPtrArray *
ctm_db_get_all (CtmDB      *self,
                GType       type,
                const char *description)
{
  g_autoptr(GomResourceGroup) group = NULL;
  GPtrArray *array = NULL;
  GomResource *resource = NULL;
  g_autoptr(GError) error = NULL;
  guint count;

  group = gom_repository_find_sync (self->repository, type, NULL, &error);
  if (error != NULL)
    {
      g_warning ("Error finding %s: (%d) %s\n",
                 description, error->code, error->message);
      return NULL;
    }

  count = gom_resource_group_get_count (group);
  if (count > 0)
    {
      gom_resource_group_fetch_sync (group, 0, count, &error);
      if (error != NULL)
        {
          g_warning ("Error fetching all %s from db: %s\n",
                     description, error->message);
          return NULL;
        }

      array = g_ptr_array_new_with_free_func (g_object_unref);
      for (int index = 0; index < count; index++)
        {
          resource = gom_resource_group_get_index (group, index);

          /* create a new reference to the object, so it won't get lost when
           * resource group is cleared */
          g_ptr_array_add (array, g_object_ref (resource));
        }
      }

  return array;
}

GomResource *
ctm_db_get_by_id (CtmDB      *self,
                  GType       type,
                  const char *description,
                  const char *which,
                  guint       id)
{
  GValue value = { 0, };
  g_autoptr(GomFilter) filter = NULL;
  GomResource *data = NULL;
  g_autoptr(GError) error = NULL;

  g_value_init (&value, G_TYPE_UINT);
  g_value_set_uint (&value, id);
  filter = gom_filter_new_eq (type, which, &value);
  g_value_unset(&value);

  data = gom_repository_find_one_sync(self->repository, type, filter, &error);
  if (error != NULL && error->code != 3)
    {
      g_warning ("Error finding %s #%d: (%d) %s\n", description, id, error->code, error->message);
      return NULL;
    }

  return data;
}

/* Person functions */

GPtrArray *
ctm_db_get_all_people (CtmDB *self)
{
  return ctm_db_get_all (self, CTM_TYPE_PERSON, "people");
}

CtmPerson *
ctm_db_get_person_by_id (CtmDB *self,
                         guint  id)
{
  return CTM_PERSON (ctm_db_get_by_id(self, CTM_TYPE_PERSON, "person", "id", id));
}

/* Project functions */

GPtrArray *
ctm_db_get_all_projects (CtmDB *self)
{
  return ctm_db_get_all (self, CTM_TYPE_PROJECT, "projects");
}

CtmProject *
ctm_db_get_project_by_id (CtmDB *self,
                          guint  id)
{
  return CTM_PROJECT (ctm_db_get_by_id(self, CTM_TYPE_PROJECT, "project", "id", id));
}

/* Task functions */

GPtrArray *
ctm_db_get_all_tasks (CtmDB *self)
{
  return ctm_db_get_all (self, CTM_TYPE_TASK, "tasks");
}

CtmTask *
ctm_db_get_task_by_id (CtmDB *self,
                       guint  id)
{
  return CTM_TASK (ctm_db_get_by_id(self, CTM_TYPE_TASK, "task", "id", id));
}

/* Event functions */

GPtrArray *
ctm_db_get_all_events (CtmDB *self)
{
  return ctm_db_get_all (self, CTM_TYPE_EVENT, "events");
}

CtmEvent *
ctm_db_get_event_by_id (CtmDB *self,
                        guint  id)
{
  return CTM_EVENT (ctm_db_get_by_id(self, CTM_TYPE_EVENT, "event", "id", id));
}

