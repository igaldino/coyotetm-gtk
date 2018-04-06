/* ctm-db.h
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

#include <glib-object.h>
#include "ctm-person.h"
#include "ctm-project.h"
#include "ctm-task.h"
#include "ctm-event.h"

G_BEGIN_DECLS

#define CTM_TYPE_DB (ctm_db_get_type ())
G_DECLARE_FINAL_TYPE (CtmDB, ctm_db, CTM, DB, GObject)

CtmDB       *ctm_db_new               (void);

const gchar *ctm_db_get_filename      (CtmDB       *self);
void         ctm_db_set_filename      (CtmDB       *self,
                                       const gchar *name);

void         ctm_db_open              (CtmDB       *self);
void         ctm_db_close             (CtmDB       *self);

void         ctm_db_save              (CtmDB       *self,
                                       GomResource *resource);

GPtrArray   *ctm_db_get_all_people    (CtmDB       *self);
CtmPerson   *ctm_db_get_person_by_id  (CtmDB       *self,
                                       guint        id);

GPtrArray   *ctm_db_get_all_projects  (CtmDB       *self);
CtmProject  *ctm_db_get_project_by_id (CtmDB       *self,
                                       guint        id);

GPtrArray   *ctm_db_get_all_tasks     (CtmDB       *self);
CtmTask     *ctm_db_get_task_by_id    (CtmDB       *self,
                                       guint        id);

GPtrArray   *ctm_db_get_all_events    (CtmDB       *self);
CtmEvent    *ctm_db_get_event_by_id   (CtmDB       *self,
                                       guint        id);

G_END_DECLS

