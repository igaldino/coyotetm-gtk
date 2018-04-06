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
  gchar         *filename;
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

const gchar *
ctm_db_get_filename (CtmDB *self)
{
  return self->filename;
}

void
ctm_db_set_filename (CtmDB       *self,
                     const gchar *filename)
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

void
ctm_db_save (CtmDB       *self,
             GomResource *resource)
{
  g_autoptr(GError) error = NULL;

  g_object_set (resource, "repository", self->repository, NULL);

  gom_resource_save_sync(resource, &error);
  if (error != NULL)
    {
      g_warning ("Error saving database table in %s: (%d) %s\n", self->filename, error->code, error->message);
      return;
    }
}

void
ctm_db_test (CtmDB *self)
{
  g_autoptr(GPtrArray) people = NULL;
  g_autoptr(GPtrArray) projects = NULL;
  CtmPerson *person = NULL;
  CtmProject *project = NULL;

  /* create data */
  person = ctm_person_new ();
  ctm_person_set_name (person, "One Person");
  ctm_db_save (self, GOM_RESOURCE(person));
  g_clear_object (&person);

  project = ctm_project_new ();
  ctm_project_set_name (project, "project1");
  ctm_project_set_description (project, "Project One");
  ctm_db_save (self, GOM_RESOURCE(project));
  g_clear_object (&project);

  project = ctm_project_new ();
  ctm_project_set_name (project, "project2");
  ctm_project_set_description (project, "Project Two");
  ctm_db_save (self, GOM_RESOURCE(project));
  g_clear_object (&project);

  /* list data */
  people = ctm_db_get_all_people (self);
  if (people)
    {
      for (int index = 0; index < people->len; index++)
        {
          person = g_ptr_array_index (people, index);
          g_print ("%d\t%s\n", ctm_person_get_id (person), ctm_person_get_name (person));
        }
      g_ptr_array_free (people, TRUE);
    }

  projects = ctm_db_get_all_projects (self);
  if (projects)
    {
      for (int index = 0; index < projects->len; index++)
        {
          project = g_ptr_array_index (projects, index);
          g_print ("%d\t%s\t%s\n", ctm_project_get_id (project), ctm_project_get_name (project), ctm_project_get_description (project));
        }
      g_ptr_array_free (projects, TRUE);
    }
}

/* Person functions */

GPtrArray *
ctm_db_get_all_people (CtmDB *self)
{
  g_autoptr(GomResourceGroup) resource_group = NULL;
  GPtrArray *people = NULL;
  CtmPerson *person = NULL;
  g_autoptr(GError) error = NULL;
  guint count;

  resource_group = gom_repository_find_sync (self->repository, CTM_TYPE_PERSON, NULL, &error);
  if (error != NULL)
    {
      g_warning ("Error finding people: (%d) %s\n", error->code, error->message);
      return NULL;
    }

  count = gom_resource_group_get_count (resource_group);
  if (count > 0)
    {
      gom_resource_group_fetch_sync (resource_group, 0, count, &error);
      if (error != NULL)
        {
          g_warning ("Error fetching all people from db: %s\n", error->message);
          return NULL;
        }

      people = g_ptr_array_new_with_free_func (g_object_unref);
      for (int index = 0; index < count; index++)
        {
          person = CTM_PERSON (gom_resource_group_get_index (resource_group, index));

          /* create a new reference to the person object,
             so it won't get lost when resource group is cleared */
          g_ptr_array_add (people, g_object_ref (person));
        }
      }

  return people;
}

CtmPerson *
ctm_db_get_person_by_id (CtmDB *self,
                         guint  id)
{
  GValue value = { 0, };
  g_autoptr(GomFilter) filter = NULL;
  CtmPerson *person = NULL;
  g_autoptr(GError) error = NULL;

  g_value_init (&value, G_TYPE_UINT);
  g_value_set_uint (&value, id);
  filter = gom_filter_new_eq (CTM_TYPE_PERSON, "id", &value);
  g_value_unset(&value);

  person = CTM_PERSON (gom_repository_find_one_sync(self->repository, CTM_TYPE_PERSON, filter, &error));
  if (error != NULL && error->code != 3)
    {
      g_warning ("Error finding person #%d: (%d) %s\n", id, error->code, error->message);
      return NULL;
    }

  return person;
}

/* Project functions */

GPtrArray *
ctm_db_get_all_projects (CtmDB *self)
{
  g_autoptr(GomResourceGroup) resource_group = NULL;
  GPtrArray *projects = NULL;
  CtmProject *project = NULL;
  g_autoptr(GError) error = NULL;
  guint count;

  resource_group = gom_repository_find_sync (self->repository, CTM_TYPE_PROJECT, NULL, &error);
  if (error != NULL)
    {
      g_warning ("Error finding projects: (%d) %s\n", error->code, error->message);
      return NULL;
    }

  count = gom_resource_group_get_count (resource_group);
  if (count > 0)
    {
      gom_resource_group_fetch_sync (resource_group, 0, count, &error);
      if (error != NULL)
        {
          g_warning ("Error fetching all projects from db: %s\n", error->message);
          return NULL;
        }

      projects = g_ptr_array_new_with_free_func (g_object_unref);
      for (int index = 0; index < count; index++)
        {
          project = CTM_PROJECT (gom_resource_group_get_index (resource_group, index));

          /* create a new reference to the project object,
             so it won't get lost when resource group is cleared */
          g_ptr_array_add (projects, g_object_ref (project));
        }
      }

  return projects;
}

CtmProject *
ctm_db_get_project_by_id (CtmDB *self,
                          guint  id)
{
  GValue value = { 0, };
  g_autoptr(GomFilter) filter = NULL;
  CtmProject *project = NULL;
  g_autoptr(GError) error = NULL;

  g_value_init (&value, G_TYPE_UINT);
  g_value_set_uint (&value, id);
  filter = gom_filter_new_eq (CTM_TYPE_PROJECT, "id", &value);
  g_value_unset(&value);

  project = CTM_PROJECT (gom_repository_find_one_sync(self->repository, CTM_TYPE_PROJECT, filter, &error));
  if (error != NULL && error->code != 3)
    {
      g_warning ("Error finding project #%d: (%d) %s\n", id, error->code, error->message);
      return NULL;
    }

  return project;
}

/* Task functions */

GPtrArray *
ctm_db_get_all_tasks (CtmDB *self)
{
  g_autoptr(GomResourceGroup) resource_group = NULL;
  GPtrArray *tasks = NULL;
  CtmTask *task = NULL;
  g_autoptr(GError) error = NULL;
  guint count;

  resource_group = gom_repository_find_sync (self->repository, CTM_TYPE_TASK, NULL, &error);
  if (error != NULL)
    {
      g_warning ("Error finding tasks: (%d) %s\n", error->code, error->message);
      return NULL;
    }

  count = gom_resource_group_get_count (resource_group);
  if (count > 0)
    {
      gom_resource_group_fetch_sync (resource_group, 0, count, &error);
      if (error != NULL)
        {
          g_warning ("Error fetching all tasks from db: %s\n", error->message);
          return NULL;
        }

      tasks = g_ptr_array_new_with_free_func (g_object_unref);
      for (int index = 0; index < count; index++)
        {
          task = CTM_TASK (gom_resource_group_get_index (resource_group, index));

          /* create a new reference to the task object,
             so it won't get lost when resource group is cleared */
          g_ptr_array_add (tasks, g_object_ref (task));
        }
      }

  return tasks;
}

CtmTask *
ctm_db_get_task_by_id (CtmDB *self,
                       guint  id)
{
  GValue value = { 0, };
  g_autoptr(GomFilter) filter = NULL;
  CtmTask *task = NULL;
  g_autoptr(GError) error = NULL;

  g_value_init (&value, G_TYPE_UINT);
  g_value_set_uint (&value, id);
  filter = gom_filter_new_eq (CTM_TYPE_TASK, "id", &value);
  g_value_unset(&value);

  task = CTM_TASK (gom_repository_find_one_sync(self->repository, CTM_TYPE_TASK, filter, &error));
  if (error != NULL && error->code != 3)
    {
      g_warning ("Error finding task #%d: (%d) %s\n", id, error->code, error->message);
      return NULL;
    }

  return task;
}

/* Event functions */

GPtrArray *
ctm_db_get_all_events (CtmDB *self)
{
  g_autoptr(GomResourceGroup) resource_group = NULL;
  GPtrArray *events = NULL;
  CtmEvent *event = NULL;
  g_autoptr(GError) error = NULL;
  guint count;

  resource_group = gom_repository_find_sync (self->repository, CTM_TYPE_EVENT, NULL, &error);
  if (error != NULL)
    {
      g_warning ("Error finding events: (%d) %s\n", error->code, error->message);
      return NULL;
    }

  count = gom_resource_group_get_count (resource_group);
  if (count > 0)
    {
      gom_resource_group_fetch_sync (resource_group, 0, count, &error);
      if (error != NULL)
        {
          g_warning ("Error fetching all events from db: %s\n", error->message);
          return NULL;
        }

      events = g_ptr_array_new_with_free_func (g_object_unref);
      for (int index = 0; index < count; index++)
        {
          event = CTM_EVENT (gom_resource_group_get_index (resource_group, index));

          /* create a new reference to the event object,
             so it won't get lost when resource group is cleared */
          g_ptr_array_add (events, g_object_ref (event));
        }
      }

  return events;
}

CtmEvent *
ctm_db_get_event_by_id (CtmDB *self,
                        guint  id)
{
  GValue value = { 0, };
  g_autoptr(GomFilter) filter = NULL;
  CtmEvent *event = NULL;
  g_autoptr(GError) error = NULL;

  g_value_init (&value, G_TYPE_UINT);
  g_value_set_uint (&value, id);
  filter = gom_filter_new_eq (CTM_TYPE_EVENT, "id", &value);
  g_value_unset(&value);

  event = CTM_EVENT (gom_repository_find_one_sync(self->repository, CTM_TYPE_EVENT, filter, &error));
  if (error != NULL && error->code != 3)
    {
      g_warning ("Error finding event #%d: (%d) %s\n", id, error->code, error->message);
      return NULL;
    }

  return event;
}

