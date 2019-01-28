/* ctm-task.h
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

typedef struct _CtmTask CtmTask;

#include <gom/gom.h>
#include "ctm-model.h"
#include "ctm-util.h"

G_BEGIN_DECLS

#define CTM_TYPE_TASK (ctm_task_get_type())

G_DECLARE_FINAL_TYPE (CtmTask, ctm_task, CTM, TASK, GomResource)

CtmTask    *ctm_task_new                 (void);

guint       ctm_task_get_id              (CtmTask         *self);
void        ctm_task_set_id              (CtmTask         *self,
                                          const guint      id);

guint       ctm_task_get_person_id       (CtmTask         *self);
void        ctm_task_set_person_id       (CtmTask         *self,
                                          const guint      person_id);

guint       ctm_task_get_project_id      (CtmTask         *self);
void        ctm_task_set_project_id      (CtmTask         *self,
                                          const guint      project_id);

const char *ctm_task_get_description     (CtmTask         *self);
void        ctm_task_set_description     (CtmTask         *self,
                                          const char      *description);

const char *ctm_task_get_notes           (CtmTask         *self);
void        ctm_task_set_notes           (CtmTask         *self,
                                          const char      *notes);

GDateTime  *ctm_task_get_begin           (CtmTask         *self);
void        ctm_task_set_begin           (CtmTask         *self,
                                          GDateTime       *begin);

GDateTime  *ctm_task_get_end             (CtmTask         *self);
void        ctm_task_set_end             (CtmTask         *self,
                                          GDateTime       *end);

GDateTime  *ctm_task_get_due             (CtmTask         *self);
void        ctm_task_set_due             (CtmTask         *self,
                                          GDateTime       *due);

guint       ctm_task_get_status          (CtmTask         *self);
void        ctm_task_set_status          (CtmTask         *self,
                                          CtmStatusType    status);

guint       ctm_task_get_priority        (CtmTask         *self);
void        ctm_task_set_priority        (CtmTask         *self,
                                          CtmPriorityType  priority);

GDateTime  *ctm_task_get_created         (CtmTask         *self);
void        ctm_task_set_created         (CtmTask         *self,
                                          GDateTime       *created);

GDateTime  *ctm_task_get_updated         (CtmTask         *self);
void        ctm_task_set_updated         (CtmTask         *self,
                                          GDateTime       *updated);

G_END_DECLS

