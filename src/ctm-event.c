/* ctm-event.c
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

#include "ctm-event.h"

struct _CtmEvent
{
  GomResource  parent_instance;
  guint        id;
  guint        task_id;
  GDateTime   *when;
  gfloat       time;
  char        *notes;
};

G_DEFINE_TYPE (CtmEvent, ctm_event, GOM_TYPE_RESOURCE)

enum {
  PROP_0,
  PROP_ID,
  PROP_TASK_ID,
  PROP_WHEN,
  PROP_TIME,
  PROP_NOTES,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
ctm_event_finalize (GObject *object)
{
  CtmEvent *self = (CtmEvent *)object;

  g_clear_pointer (&self->when, g_date_time_unref);
  g_clear_pointer (&self->notes, g_free);

  G_OBJECT_CLASS (ctm_event_parent_class)->finalize (object);
}

static void
ctm_event_get_property (GObject    *object,
                        guint       prop_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  CtmEvent *self = (CtmEvent *)object;

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_uint (value, ctm_event_get_id (self));
      break;
    case PROP_TASK_ID:
      g_value_set_uint (value, ctm_event_get_task_id (self));
      break;
    case PROP_WHEN:
      g_value_set_boxed (value, ctm_event_get_when (self));
      break;
    case PROP_TIME:
      g_value_set_float (value, ctm_event_get_time (self));
      break;
    case PROP_NOTES:
      g_value_set_string (value, ctm_event_get_notes (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_event_set_property (GObject      *object,
                        guint         prop_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  CtmEvent *self = (CtmEvent *)object;

  switch (prop_id)
    {
    case PROP_ID:
      ctm_event_set_id (self, g_value_get_uint (value));
      break;
    case PROP_TASK_ID:
      ctm_event_set_task_id (self, g_value_get_uint (value));
      break;
    case PROP_WHEN:
      ctm_event_set_when (self, g_value_get_boxed (value));
      break;
    case PROP_TIME:
      ctm_event_set_time (self, g_value_get_float (value));
      break;
    case PROP_NOTES:
      ctm_event_set_notes (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_event_class_init (CtmEventClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);

  object_class->finalize = ctm_event_finalize;
  object_class->get_property = ctm_event_get_property;
  object_class->set_property = ctm_event_set_property;

  gom_resource_class_set_table(resource_class, "event");

  properties [PROP_ID] =
    g_param_spec_uint ("id",
                       "ID",
                       "The internal id of the event",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_TASK_ID] =
    g_param_spec_uint ("task_id",
                       "Task ID",
                       "The internal id of this event's task",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_WHEN] =
    g_param_spec_boxed ("when",
                        "When",
                        "When this event happened",
                        G_TYPE_DATE_TIME,
                        G_PARAM_READWRITE);

  properties [PROP_TIME] =
    g_param_spec_float ("time",
                        "Time",
                        "How long this event took",
                        0,
                        G_MAXFLOAT,
                        0,
                        G_PARAM_READWRITE);

  properties [PROP_NOTES] =
    g_param_spec_string ("notes",
                         "Notes",
                         "Notes of the event",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, LAST_PROP, properties);

  gom_resource_class_set_primary_key(resource_class, "id");
}

static void
ctm_event_init (CtmEvent *self)
{
  ctm_event_set_when (self, ctm_util_get_today ());
}

CtmEvent *
ctm_event_new (void)
{
  return g_object_new (CTM_TYPE_EVENT, NULL);
}

guint
ctm_event_get_id (CtmEvent *self)
{
  return self->id;
}

void
ctm_event_set_id (CtmEvent    *self,
                  const guint  id)
{
  self->id = id;
}

guint
ctm_event_get_task_id (CtmEvent *self)
{
  return self->task_id;
}

void
ctm_event_set_task_id (CtmEvent    *self,
                       const guint  task_id)
{
  self->task_id = task_id;
}

GDateTime *
ctm_event_get_when (CtmEvent *self)
{
  return self->when;
}

void
ctm_event_set_when (CtmEvent  *self,
                    GDateTime *when)
{
  if (when &&
      ((self->when && !g_date_time_equal (when, self->when)) ||
       !self->when))
    {
      g_clear_pointer (&self->when, g_date_time_unref);
      self->when = ctm_util_copy_date (when);
    }
}

gfloat
ctm_event_get_time (CtmEvent *self)
{
  return self->time;
}

void
ctm_event_set_time (CtmEvent *self,
                    gfloat    time)
{
  self->time = time;
}

const char *
ctm_event_get_notes (CtmEvent *self)
{
  return self->notes;
}

void
ctm_event_set_notes (CtmEvent   *self,
                     const char *notes)
{
  if (g_strcmp0 (notes, self->notes))
    {
      g_clear_pointer (&self->notes, g_free);
      self->notes = g_strdup (notes);
    }
}

