/* ctm-model.h
 *
 * Copyright 2018 Isaque Galdino <igaldino@gmail.com>
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

typedef struct _CtmModel CtmModel;

typedef enum
{
  CTM_STATUS_NOT_STARTED,
  CTM_STATUS_IN_PROGRESS,
  CTM_STATUS_COMPLETED,
  CTM_STATUS_CANCELED,
  CTM_STATUS_ON_HOLD,
  CTM_STATUS_NOO
} CtmStatusType;

typedef enum
{
  CTM_PRIORITY_CRITICAL,
  CTM_PRIORITY_HIGH,
  CTM_PRIORITY_MEDIUM,
  CTM_PRIORITY_LOW,
  CTM_PRIORITY_NONE,
  CTM_PRIORITY_NOO
} CtmPriorityType;

#include <glib-object.h>
#include <gtk/gtk.h>
#include "ctm-app.h"

G_BEGIN_DECLS

typedef enum
{
  CTM_MODEL_PERSON_COLUMN_ID,
  CTM_MODEL_PERSON_COLUMN_NAME,
  CTM_MODEL_PERSON_COLUMN_NOO
} CtmModelPersonColumnType;

typedef enum
{
  CTM_MODEL_PROJECT_COLUMN_ID,
  CTM_MODEL_PROJECT_COLUMN_NAME,
  CTM_MODEL_PROJECT_COLUMN_DESCRIPTION,
  CTM_MODEL_PROJECT_COLUMN_NOO
} CtmModelProjectColumnType;

typedef enum
{
  CTM_MODEL_TASK_COLUMN_ID,
  CTM_MODEL_TASK_COLUMN_PERSON_ID,
  CTM_MODEL_TASK_COLUMN_PERSON_NAME,
  CTM_MODEL_TASK_COLUMN_PROJECT_ID,
  CTM_MODEL_TASK_COLUMN_PROJECT_NAME,
  CTM_MODEL_TASK_COLUMN_DESCRIPTION,
  CTM_MODEL_TASK_COLUMN_NOTES,
  CTM_MODEL_TASK_COLUMN_BEGIN,
  CTM_MODEL_TASK_COLUMN_BEGIN_STRING,
  CTM_MODEL_TASK_COLUMN_END,
  CTM_MODEL_TASK_COLUMN_END_STRING,
  CTM_MODEL_TASK_COLUMN_DUE,
  CTM_MODEL_TASK_COLUMN_DUE_STRING,
  CTM_MODEL_TASK_COLUMN_STATUS,
  CTM_MODEL_TASK_COLUMN_STATUS_STRING,
  CTM_MODEL_TASK_COLUMN_PRIORITY,
  CTM_MODEL_TASK_COLUMN_PRIORITY_STRING,
  CTM_MODEL_TASK_COLUMN_CREATED,
  CTM_MODEL_TASK_COLUMN_CREATED_STRING,
  CTM_MODEL_TASK_COLUMN_UPDATED,
  CTM_MODEL_TASK_COLUMN_UPDATED_STRING,
  CTM_MODEL_TASK_COLUMN_UPDATED_SORT,
  CTM_MODEL_TASK_COLUMN_NOO
} CtmModelTaskColumnType;

typedef enum
{
  CTM_MODEL_EVENT_COLUMN_ID,
  CTM_MODEL_EVENT_COLUMN_TASK_ID,
  CTM_MODEL_EVENT_COLUMN_TASK_DESCRIPTION,
  CTM_MODEL_EVENT_COLUMN_WHEN,
  CTM_MODEL_EVENT_COLUMN_WHEN_STRING,
  CTM_MODEL_EVENT_COLUMN_TIME,
  CTM_MODEL_EVENT_COLUMN_TIME_STRING,
  CTM_MODEL_EVENT_COLUMN_NOTES,
  CTM_MODEL_EVENT_COLUMN_NOO
} CtmModelEventColumnType;

typedef enum
{
  CTM_MODEL_STATUS_COLUMN_ID,
  CTM_MODEL_STATUS_COLUMN_NAME,
  CTM_MODEL_STATUS_COLUMN_NOO
} CtmModelStatusColumnType;

typedef enum
{
  CTM_MODEL_PRIORITY_COLUMN_ID,
  CTM_MODEL_PRIORITY_COLUMN_NAME,
  CTM_MODEL_PRIORITY_COLUMN_NOO
} CtmModelPriorityColumnType;

#define CTM_TYPE_MODEL (ctm_model_get_type())

G_DECLARE_FINAL_TYPE (CtmModel, ctm_model, CTM, MODEL, GObject)

CtmModel     *ctm_model_new                      (void);

GtkListStore *ctm_model_person_get_all           (CtmModel        *self);
GtkListStore *ctm_model_project_get_all          (CtmModel        *self);
GtkListStore *ctm_model_task_get_all             (CtmModel        *self);
GtkListStore *ctm_model_event_get_all            (CtmModel        *self);
GtkListStore *ctm_model_status_get_all           (CtmModel        *self);
GtkListStore *ctm_model_priority_get_all         (CtmModel        *self);

CtmPerson    *ctm_model_person_get               (CtmModel        *self,
                                                  guint            id);
CtmProject   *ctm_model_project_get              (CtmModel        *self,
                                                  guint            id);
CtmTask      *ctm_model_task_get                 (CtmModel        *self,
                                                  guint            id);

const char   *ctm_model_status_get_description   (CtmModel        *self,
                                                  CtmStatusType    status);
const char   *ctm_model_priority_get_description (CtmModel        *self,
                                                  CtmPriorityType  priority);

G_END_DECLS
