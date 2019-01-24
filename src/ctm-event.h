/* ctm-event.h
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
#include "ctm-util.h"

G_BEGIN_DECLS

#define CTM_TYPE_EVENT (ctm_event_get_type())

G_DECLARE_FINAL_TYPE (CtmEvent, ctm_event, CTM, EVENT, GomResource)

CtmEvent   *ctm_event_new         (void);

guint       ctm_event_get_id      (CtmEvent    *self);
void        ctm_event_set_id      (CtmEvent    *self,
                                   const guint  id);

guint       ctm_event_get_task_id (CtmEvent    *self);
void        ctm_event_set_task_id (CtmEvent    *self,
                                   const guint  task_id);

GDateTime  *ctm_event_get_when    (CtmEvent    *self);
void        ctm_event_set_when    (CtmEvent    *self,
                                   GDateTime   *when);

gfloat      ctm_event_get_time    (CtmEvent    *self);
void        ctm_event_set_time    (CtmEvent    *self,
                                   gfloat       time);

const char *ctm_event_get_notes   (CtmEvent    *self);
void        ctm_event_set_notes   (CtmEvent    *self,
                                   const char  *notes);

G_END_DECLS

