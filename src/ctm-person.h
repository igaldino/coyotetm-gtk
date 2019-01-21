/* ctm-person.h
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

#pragma once

#include <gom/gom.h>

G_BEGIN_DECLS

#define CTM_TYPE_PERSON (ctm_person_get_type())

G_DECLARE_FINAL_TYPE (CtmPerson, ctm_person, CTM, PERSON, GomResource)

CtmPerson   *ctm_person_new      (void);

guint        ctm_person_get_id   (CtmPerson   *self);
void         ctm_person_set_id   (CtmPerson   *self,
                                  const guint  id);

const char  *ctm_person_get_name (CtmPerson   *self);
void         ctm_person_set_name (CtmPerson   *self,
                                  const char  *name);

G_END_DECLS

