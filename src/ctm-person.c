/* ctm-person.c
 *
 * Copyright (C) 2015 Isaque Galdino <igaldino@gmail.com>
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

#include "ctm-person.h"

struct _CtmPerson
{
  GomResource  parent_instance;
  guint        id;
  gchar       *name;
};

G_DEFINE_TYPE (CtmPerson, ctm_person, GOM_TYPE_RESOURCE)

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
ctm_person_finalize (GObject *object)
{
  CtmPerson *self = (CtmPerson *)object;

  g_clear_pointer (&self->name, g_free);

  G_OBJECT_CLASS (ctm_person_parent_class)->finalize (object);
}

static void
ctm_person_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
  CtmPerson *self = (CtmPerson *)object;

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_uint (value, ctm_person_get_id (self));
      break;
    case PROP_NAME:
      g_value_set_string (value, ctm_person_get_name (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_person_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
  CtmPerson *self = (CtmPerson *)object;

  switch (prop_id)
    {
    case PROP_ID:
      ctm_person_set_id (self, g_value_get_uint (value));
      break;
    case PROP_NAME:
      ctm_person_set_name (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_person_class_init (CtmPersonClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);

  object_class->finalize = ctm_person_finalize;
  object_class->get_property = ctm_person_get_property;
  object_class->set_property = ctm_person_set_property;

  gom_resource_class_set_table(resource_class, "person");

  properties [PROP_ID] =
    g_param_spec_uint ("id",
                       "ID",
                       "The internal id of the person",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_NAME] =
    g_param_spec_string ("name",
                         "Name",
                         "The name of the person",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, LAST_PROP, properties);

  gom_resource_class_set_primary_key(resource_class, "id");
}

static void
ctm_person_init (CtmPerson *self)
{
  self->id = 0;
  self->name = NULL;
}

CtmPerson *
ctm_person_new (void)
{
  return g_object_new (CTM_TYPE_PERSON, NULL);
}

guint
ctm_person_get_id (CtmPerson *self)
{
  return self->id;
}

void
ctm_person_set_id (CtmPerson   *self,
                   const guint  id)
{
  self->id = id;
}

const gchar *
ctm_person_get_name (CtmPerson *self)
{
  return self->name;
}

void
ctm_person_set_name (CtmPerson   *self,
                     const gchar *name)
{
  if (g_strcmp0 (name, self->name))
    {
      g_clear_pointer (&self->name, g_free);
      self->name = g_strdup (name);
    }
}

