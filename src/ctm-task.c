/* ctm-task.c
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

#include "ctm-task.h"

struct _CtmTask
{
  GomResource  parent_instance;
  guint        id;
  guint        person_id;
  guint        project_id;
  char        *description;
  char        *notes;
  GDateTime   *begin;
  GDateTime   *end;
  GDateTime   *due;
  guint        status;
  guint        priority;
  GDateTime   *created;
  GDateTime   *updated;
};

G_DEFINE_TYPE (CtmTask, ctm_task, GOM_TYPE_RESOURCE)

enum {
  PROP_0,
  PROP_ID,
  PROP_PERSON_ID,
  PROP_PROJECT_ID,
  PROP_DESCRIPTION,
  PROP_NOTES,
  PROP_BEGIN,
  PROP_END,
  PROP_DUE,
  PROP_STATUS,
  PROP_PRIORITY,
  PROP_CREATED,
  PROP_UPDATED,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
ctm_task_finalize (GObject *object)
{
  CtmTask *self = (CtmTask *)object;

  g_clear_pointer (&self->description, g_free);
  g_clear_pointer (&self->notes, g_free);
  g_clear_pointer (&self->begin, g_date_time_unref);
  g_clear_pointer (&self->end, g_date_time_unref);
  g_clear_pointer (&self->due, g_date_time_unref);
  g_clear_pointer (&self->created, g_date_time_unref);
  g_clear_pointer (&self->updated, g_date_time_unref);

  G_OBJECT_CLASS (ctm_task_parent_class)->finalize (object);
}

static void
ctm_task_get_property (GObject    *object,
                       guint       prop_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
  CtmTask *self = (CtmTask *)object;

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_uint (value, ctm_task_get_id (self));
      break;
    case PROP_PERSON_ID:
      g_value_set_uint (value, ctm_task_get_person_id (self));
      break;
    case PROP_PROJECT_ID:
      g_value_set_uint (value, ctm_task_get_project_id (self));
      break;
    case PROP_DESCRIPTION:
      g_value_set_string (value, ctm_task_get_description (self));
      break;
    case PROP_NOTES:
      g_value_set_string (value, ctm_task_get_notes (self));
      break;
    case PROP_BEGIN:
      g_value_set_boxed (value, ctm_task_get_begin (self));
      break;
    case PROP_END:
      g_value_set_boxed (value, ctm_task_get_end (self));
      break;
    case PROP_DUE:
      g_value_set_boxed (value, ctm_task_get_due (self));
      break;
    case PROP_STATUS:
      g_value_set_uint (value, ctm_task_get_status (self));
      break;
    case PROP_PRIORITY:
      g_value_set_uint (value, ctm_task_get_priority (self));
      break;
    case PROP_CREATED:
      g_value_set_boxed (value, ctm_task_get_created (self));
      break;
    case PROP_UPDATED:
      g_value_set_boxed (value, ctm_task_get_updated (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_task_set_property (GObject      *object,
                       guint         prop_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
  CtmTask *self = (CtmTask *)object;

  switch (prop_id)
    {
    case PROP_ID:
      ctm_task_set_id (self, g_value_get_uint (value));
      break;
    case PROP_PERSON_ID:
      ctm_task_set_person_id (self, g_value_get_uint (value));
      break;
    case PROP_PROJECT_ID:
      ctm_task_set_project_id (self, g_value_get_uint (value));
      break;
    case PROP_DESCRIPTION:
      ctm_task_set_description (self, g_value_get_string (value));
      break;
    case PROP_NOTES:
      ctm_task_set_notes (self, g_value_get_string (value));
      break;
    case PROP_BEGIN:
      ctm_task_set_begin (self, g_value_get_boxed (value));
      break;
    case PROP_END:
      ctm_task_set_end (self, g_value_get_boxed (value));
      break;
    case PROP_DUE:
      ctm_task_set_due (self, g_value_get_boxed (value));
      break;
    case PROP_STATUS:
      ctm_task_set_status (self, g_value_get_uint (value));
      break;
    case PROP_PRIORITY:
      ctm_task_set_priority (self, g_value_get_uint (value));
      break;
    case PROP_CREATED:
      ctm_task_set_created (self, g_value_get_boxed (value));
      break;
    case PROP_UPDATED:
      ctm_task_set_updated (self, g_value_get_boxed (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_task_class_init (CtmTaskClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);

  object_class->finalize = ctm_task_finalize;
  object_class->get_property = ctm_task_get_property;
  object_class->set_property = ctm_task_set_property;

  gom_resource_class_set_table(resource_class, "task");

  properties [PROP_ID] =
    g_param_spec_uint ("id",
                       "ID",
                       "The internal id of the task",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_PERSON_ID] =
    g_param_spec_uint ("person_id",
                       "Person ID",
                       "The internal id of this task's person",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_PROJECT_ID] =
    g_param_spec_uint ("project_id",
                       "Project ID",
                       "The internal id of this task's project",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_DESCRIPTION] =
    g_param_spec_string ("description",
                         "Description",
                         "The description of the task",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  properties [PROP_NOTES] =
    g_param_spec_string ("notes",
                         "Notes",
                         "Notes of the task",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  properties [PROP_BEGIN] =
    g_param_spec_boxed ("begin",
                        "Begin",
                        "The begin date of the task",
                        G_TYPE_DATE_TIME,
                        G_PARAM_READWRITE);

  properties [PROP_END] =
    g_param_spec_boxed ("end",
                        "End",
                        "The end date of the task",
                        G_TYPE_DATE_TIME,
                        G_PARAM_READWRITE);

  properties [PROP_DUE] =
    g_param_spec_boxed ("due",
                        "Due",
                        "The due date of the task",
                        G_TYPE_DATE_TIME,
                        G_PARAM_READWRITE);

  properties [PROP_STATUS] =
    g_param_spec_uint ("status",
                       "Status",
                       "The status of the task",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_PRIORITY] =
    g_param_spec_uint ("priority",
                       "Priority",
                       "The priority of the task",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_CREATED] =
    g_param_spec_boxed ("created",
                        "Created",
                        "The created date of the task",
                        G_TYPE_DATE_TIME,
                        G_PARAM_READWRITE);

  properties [PROP_UPDATED] =
    g_param_spec_boxed ("updated",
                        "Updated",
                        "The updated date of the task",
                        G_TYPE_DATE_TIME,
                        G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, LAST_PROP, properties);

  gom_resource_class_set_primary_key(resource_class, "id");
}

static void
ctm_task_init (CtmTask *self)
{
  ctm_task_set_created (self, ctm_util_get_today ());
}

CtmTask *
ctm_task_new (void)
{
  return g_object_new (CTM_TYPE_TASK, NULL);
}

guint
ctm_task_get_id (CtmTask *self)
{
  return self->id;
}

void
ctm_task_set_id (CtmTask     *self,
                 const guint  id)
{
  self->id = id;
}

guint
ctm_task_get_person_id (CtmTask *self)
{
  return self->person_id;
}

void
ctm_task_set_person_id (CtmTask     *self,
                        const guint  person_id)
{
  self->person_id = person_id;
}

guint
ctm_task_get_project_id (CtmTask *self)
{
  return self->project_id;
}

void
ctm_task_set_project_id (CtmTask    *self,
                         const guint  project_id)
{
  self->project_id = project_id;
}

const char *
ctm_task_get_description (CtmTask *self)
{
  g_return_val_if_fail (self, NULL);

  return self->description;
}

void
ctm_task_set_description (CtmTask    *self,
                          const char *description)
{
  if (g_strcmp0 (description, self->description))
    {
      g_clear_pointer (&self->description, g_free);
      self->description = g_strdup (description);
    }
}

const char *
ctm_task_get_notes (CtmTask *self)
{
  return self->notes;
}

void
ctm_task_set_notes (CtmTask    *self,
                    const char *notes)
{
  if (g_strcmp0 (notes, self->notes))
    {
      g_clear_pointer (&self->notes, g_free);
      self->notes = g_strdup (notes);
    }
}

GDateTime *
ctm_task_get_begin (CtmTask *self)
{
  return self->begin;
}

void
ctm_task_set_begin (CtmTask   *self,
                    GDateTime *begin)
{
  if (begin &&
      ((self->begin && !g_date_time_equal (begin, self->begin)) ||
       !self->begin))
    {
      g_clear_pointer (&self->begin, g_date_time_unref);
      self->begin = ctm_util_copy_date (begin);
    }
}

GDateTime *
ctm_task_get_end (CtmTask *self)
{
  return self->end;
}

void
ctm_task_set_end (CtmTask   *self,
                  GDateTime *end)
{
  if (end &&
      ((self->end && !g_date_time_equal (end, self->end)) ||
       !self->end))
    {
      g_clear_pointer (&self->end, g_date_time_unref);
      self->end = ctm_util_copy_date (end);
    }
}

GDateTime *
ctm_task_get_due (CtmTask *self)
{
  return self->due;
}

void
ctm_task_set_due (CtmTask   *self,
                  GDateTime *due)
{
  if (due &&
      ((self->due && !g_date_time_equal (due, self->due)) ||
       !self->due))
    {
      g_clear_pointer (&self->due, g_date_time_unref);
      self->due = ctm_util_copy_date (due);
    }
}

CtmStatusType
ctm_task_get_status (CtmTask *self)
{
  return self->status;
}

void
ctm_task_set_status (CtmTask       *self,
                     CtmStatusType  status)
{
  self->status = status;
}

CtmPriorityType
ctm_task_get_priority (CtmTask *self)
{
  return self->priority;
}

void
ctm_task_set_priority (CtmTask             *self,
                       CtmPriorityType  priority)
{
  self->priority = priority;
}

GDateTime *
ctm_task_get_created (CtmTask *self)
{
  return self->created;
}

void
ctm_task_set_created (CtmTask   *self,
                      GDateTime *created)
{
  if (created &&
      ((self->created && !g_date_time_equal (created, self->created)) ||
       !self->created))
    {
      g_clear_pointer (&self->created, g_date_time_unref);
      self->created = ctm_util_copy_date (created);

      ctm_task_set_updated (self, created);
    }
}

GDateTime *
ctm_task_get_updated (CtmTask *self)
{
  return self->updated;
}

void
ctm_task_set_updated (CtmTask   *self,
                      GDateTime *updated)
{
  if (updated &&
      ((self->updated && !g_date_time_equal (updated, self->updated)) ||
       !self->updated))
    {
      g_clear_pointer (&self->updated, g_date_time_unref);
      self->updated = ctm_util_copy_date (updated);
    }
}

