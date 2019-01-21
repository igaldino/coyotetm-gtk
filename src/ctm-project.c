/* ctm-project.c
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

#include "ctm-project.h"

struct _CtmProject
{
  GomResource  parent_instance;
  guint        id;
  char        *name;
  char        *description;
};

G_DEFINE_TYPE (CtmProject, ctm_project, GOM_TYPE_RESOURCE)

enum {
  PROP_0,
  PROP_ID,
  PROP_NAME,
  PROP_DESCRIPTION,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
ctm_project_finalize (GObject *object)
{
  CtmProject *self = (CtmProject *)object;

  g_clear_pointer (&self->name, g_free);
  g_clear_pointer (&self->description, g_free);

  G_OBJECT_CLASS (ctm_project_parent_class)->finalize (object);
}

static void
ctm_project_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  CtmProject *self = (CtmProject *)object;

  switch (prop_id)
    {
    case PROP_ID:
      g_value_set_uint (value, ctm_project_get_id (self));
      break;
    case PROP_NAME:
      g_value_set_string (value, ctm_project_get_name (self));
      break;
    case PROP_DESCRIPTION:
      g_value_set_string (value, ctm_project_get_description (self));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_project_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  CtmProject *self = (CtmProject *)object;

  switch (prop_id)
    {
    case PROP_ID:
      ctm_project_set_id (self, g_value_get_uint (value));
      break;
    case PROP_NAME:
      ctm_project_set_name (self, g_value_get_string (value));
      break;
    case PROP_DESCRIPTION:
      ctm_project_set_description (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
ctm_project_class_init (CtmProjectClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GomResourceClass *resource_class = GOM_RESOURCE_CLASS(klass);

  object_class->finalize = ctm_project_finalize;
  object_class->get_property = ctm_project_get_property;
  object_class->set_property = ctm_project_set_property;

  gom_resource_class_set_table(resource_class, "project");

  properties [PROP_ID] =
    g_param_spec_uint ("id",
                       "ID",
                       "The internal id of the project",
                       0,
                       G_MAXUINT,
                       0,
                       G_PARAM_READWRITE);

  properties [PROP_NAME] =
    g_param_spec_string ("name",
                         "Name",
                         "The name of the project",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  properties [PROP_DESCRIPTION] =
    g_param_spec_string ("description",
                         "Description",
                         "The description of the project",
                         NULL,
                         (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, LAST_PROP, properties);

  gom_resource_class_set_primary_key(resource_class, "id");
}

static void
ctm_project_init (CtmProject *self)
{
  self->id = 0;
  self->name = NULL;
  self->description = NULL;
}

CtmProject *
ctm_project_new (void)
{
  return g_object_new (CTM_TYPE_PROJECT, NULL);
}

guint
ctm_project_get_id (CtmProject *self)
{
  return self->id;
}

void
ctm_project_set_id (CtmProject  *self,
                    const guint  id)
{
  self->id = id;
}

const char *
ctm_project_get_name (CtmProject *self)
{
  g_return_val_if_fail (self, NULL);

  return self->name;
}

void
ctm_project_set_name (CtmProject *self,
                      const char *name)
{
  if (g_strcmp0 (name, self->name))
    {
      g_clear_pointer (&self->name, g_free);
      self->name = g_strdup (name);
    }
}

const char *
ctm_project_get_description (CtmProject *self)
{
  return self->description;
}

void
ctm_project_set_description (CtmProject *self,
                             const char *description)
{
  if (g_strcmp0 (description, self->description))
    {
      g_clear_pointer (&self->description, g_free);
      self->description = g_strdup (description);
    }
}
