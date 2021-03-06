/* ctm-util.h
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

#include <glib.h>

GDateTime *ctm_util_copy_date     (GDateTime  *date);
char      *ctm_util_format_date   (GDateTime  *date);
GDateTime *ctm_util_get_today     ();
GDateTime *ctm_util_get_tomorrow  ();
GDateTime *ctm_util_get_yesterday ();
GDateTime *ctm_util_new_date      (guint       year,
                                   guint       month,
                                   guint       day);
GDateTime *ctm_util_parse_date    (const char *date_string);
