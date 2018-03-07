/* ctm-project.h
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

#pragma once

#include <gom/gom.h>

G_BEGIN_DECLS

#define CTM_TYPE_PROJECT (ctm_project_get_type())

G_DECLARE_FINAL_TYPE (CtmProject, ctm_project, CTM, PROJECT, GomResource)

CtmProject  *ctm_project_new             (void);

guint        ctm_project_get_id          (CtmProject  *self);
void         ctm_project_set_id          (CtmProject  *self,
                                          const guint  id);

const gchar *ctm_project_get_name        (CtmProject  *self);
void         ctm_project_set_name        (CtmProject  *self,
                                          const gchar *name);

const gchar *ctm_project_get_description (CtmProject  *self);
void         ctm_project_set_description (CtmProject  *self,
                                          const gchar *description);

G_END_DECLS

