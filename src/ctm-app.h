/* ctm-app.h
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

#include <gtk/gtk.h>
#include "ctm-db.h"

G_BEGIN_DECLS

#define CTM_TYPE_APP (ctm_app_get_type ())

G_DECLARE_FINAL_TYPE (CtmApp, ctm_app, CTM, APP, GtkApplication)

CtmApp *ctm_app_new     (void);

CtmDB  *ctm_app_get_db  (CtmApp *self);

void    ctm_app_message (GtkWindow      *parent,
                         const char     *message);

G_END_DECLS

