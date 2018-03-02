/* ctm-app.c
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ctm-app.h"
#include "ctm-window.h"

struct _CtmApp
{
  GtkApplication  parent_instance;
  CtmDB          *db;
};

G_DEFINE_TYPE (CtmApp, ctm_app, GTK_TYPE_APPLICATION);

static void
ctm_app_init (CtmApp *self)
{
  self->db = NULL;
}

static void
about_activated (GSimpleAction *action,
								 GVariant      *parameter,
								 gpointer       app)
{
  GtkWindow *win = gtk_application_get_active_window (GTK_APPLICATION (app));
  const gchar *authors[] = {"Isaque Galdino <igaldino@gmail.com>", NULL};

  gtk_show_about_dialog (GTK_WINDOW (win),
												 "program-name", "CoyoteTM",
												 "version", g_strdup_printf("%s", PACKAGE_VERSION),
												 "copyright", "(C) 2000-2017 Isaque Galdino",
												 "license-type", GTK_LICENSE_GPL_3_0,
												 "website", "http://sourceforge.net/projects/coyotetm/",
												 "comments", "Manage tasks like a coyote",
												 "authors", authors,
												 "logo-icon-name", "coyotetm",
												 "title", "About CoyoteTM",
												 NULL);
}

static void
quit_activated (GSimpleAction *action,
								GVariant      *parameter,
								gpointer       app)
{
  g_application_quit (G_APPLICATION (app));
}

static GActionEntry menu_entries[] =
{
  {"about", about_activated, NULL, NULL, NULL},
  {"quit", quit_activated, NULL, NULL, NULL}
};

static void
ctm_app_startup (GApplication *self)
{
  /* GtkBuilder *builder;
  GMenuModel *menu; */
  const gchar *quit_accels[2] = {"<Ctrl>Q", NULL};
  CtmApp *app = CTM_APP (self);

  G_APPLICATION_CLASS (ctm_app_parent_class)->startup (self);

  app->db = ctm_db_new ();
  ctm_db_set_filename (app->db,
											 g_build_filename (g_get_user_data_dir (),
																				 "coyotetm.db",
																				 NULL));
  ctm_db_open (app->db);

  g_action_map_add_action_entries (G_ACTION_MAP (self),
																	 menu_entries,
																	 G_N_ELEMENTS (menu_entries),
																	 self);

  gtk_application_set_accels_for_action (GTK_APPLICATION (self),
																				 "app.quit",
																				 quit_accels);

  /* builder = gtk_builder_new_from_resource ("/org/gtk/coyotetm/menu.ui");
  menu = G_MENU_MODEL (gtk_builder_get_object (builder, "menu"));
  gtk_application_set_app_menu (GTK_APPLICATION (self), menu);
  g_object_unref (builder); */
}

static void
ctm_app_shutdown (GApplication *app)
{
  CtmApp *self = CTM_APP (app);

  ctm_db_close (self->db);
  g_clear_object (&self->db);

  G_APPLICATION_CLASS (ctm_app_parent_class)->shutdown (app);
}

static void
ctm_app_activate (GApplication *self)
{
  GtkWindow *window = gtk_application_get_active_window (GTK_APPLICATION (self));
	if (!window)
		window = GTK_WINDOW (ctm_window_new (CTM_APP (self)));
  gtk_window_present (window);
}

static void
ctm_app_class_init (CtmAppClass *class)
{
  G_APPLICATION_CLASS (class)->startup = ctm_app_startup;
  G_APPLICATION_CLASS (class)->activate = ctm_app_activate;
  G_APPLICATION_CLASS (class)->shutdown = ctm_app_shutdown;
}

CtmApp *
ctm_app_new (void)
{
  return g_object_new (CTM_TYPE_APP, "application-id", "org.gtk.coyotetm", NULL);
}

CtmDB *
ctm_app_get_db (CtmApp *self)
{
  return self->db;
}

void
ctm_app_message (GtkWindow  *parent,
								 const char *message)
{
  GtkWidget *dialog = gtk_message_dialog_new (parent,
																							GTK_DIALOG_DESTROY_WITH_PARENT,
																							GTK_MESSAGE_INFO,
																							GTK_BUTTONS_CLOSE,
																							message);

  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

