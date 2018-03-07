/* ctm-window.h
 *
 * Copyright (C) 2017 Isaque Galdino <igaldino@gmail.com>
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

#include "ctm-app.h"

G_BEGIN_DECLS

#define CTM_TYPE_WINDOW (ctm_window_get_type())

G_DECLARE_FINAL_TYPE (CtmWindow, ctm_window, CTM, WINDOW, GtkApplicationWindow)

CtmWindow *ctm_window_new          (CtmApp     *app);

void       ctm_window_show_myself  (CtmWindow  *self);

void       ctm_window_show_task    (CtmWindow  *self,
                                    CtmTask    *task);

void       ctm_window_show_project (CtmWindow  *self,
                                    CtmProject *project);

void       ctm_window_show_person  (CtmWindow  *self,
                                    CtmPerson  *person);

void       ctm_window_show_event   (CtmWindow  *self,
                                    CtmEvent   *event);

void       ctm_window_go_back      (CtmWindow  *self);

G_END_DECLS

