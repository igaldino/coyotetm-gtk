/* ctm-util.c
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

#include "ctm-util.h"

GDateTime *
ctm_util_get_today ()
{
  g_autoptr (GDateTime) now = NULL;

  now = g_date_time_new_now_local ();
  return g_date_time_new_local (g_date_time_get_year (now),
                                g_date_time_get_month (now),
                                g_date_time_get_day_of_month (now),
                                0, 0, 0.0);
}

GDateTime *
ctm_util_get_yesterday ()
{
  g_autoptr (GDateTime) today = NULL;

  today = ctm_util_get_today ();
  return g_date_time_add_days (today, -1);
}

GDateTime *
ctm_util_get_tomorrow ()
{
  g_autoptr (GDateTime) today = NULL;

  today = ctm_util_get_today ();
  return g_date_time_add_days (today, 1);
}

GDateTime *
ctm_util_copy_date (GDateTime *date)
{
  return g_date_time_new_local (g_date_time_get_year (date),
                                g_date_time_get_month (date),
                                g_date_time_get_day_of_month (date),
                                0, 0, 0.0);
}

char *
ctm_util_format_date (GDateTime *date)
{
  return g_date_time_format (date, "%x");
}
