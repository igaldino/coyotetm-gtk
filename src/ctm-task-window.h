/* ctm-task-window.h
 *
 * Copyright 2019 Isaque Galdino <igaldino@gmail.com>
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "ctm-app.h"
#include "ctm-task.h"

G_BEGIN_DECLS

#define CTM_TYPE_TASK_WINDOW (ctm_task_window_get_type())

G_DECLARE_FINAL_TYPE (CtmTaskWindow, ctm_task_window, CTM, TASK_WINDOW, GtkWindow)

CtmTaskWindow *ctm_task_window_new       (void);

void           ctm_task_window_show_task (CtmTaskWindow *self,
                                          CtmTask       *task);

G_END_DECLS

