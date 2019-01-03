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
  gboolean        run_tests;
};

G_DEFINE_TYPE (CtmApp, ctm_app, GTK_TYPE_APPLICATION);

static void
ctm_app_about (GSimpleAction *action,
               GVariant      *parameter,
               gpointer       app);

static void
ctm_app_quit (GSimpleAction *action,
              GVariant      *parameter,
              gpointer       app);

static GActionEntry ctm_app_menu[] =
{
  {"about", ctm_app_about, NULL, NULL, NULL},
  {"quit", ctm_app_quit, NULL, NULL, NULL}
};

static GOptionEntry ctm_app_options[] =
{
  {"run-tests", 't', 0, G_OPTION_ARG_NONE, NULL, "Run application tests", NULL},
  {NULL}
};

static void
ctm_app_about (GSimpleAction *action,
               GVariant      *parameter,
               gpointer       app)
{
  GtkWindow *win = gtk_application_get_active_window (GTK_APPLICATION (app));
  const gchar *authors[] = {"Isaque Galdino <igaldino@gmail.com>", NULL};

  gtk_show_about_dialog (GTK_WINDOW (win),
                         "program-name", "Coyote Task Manager",
                         "version", PACKAGE_VERSION,
                         "copyright", "(C) 2000-2017 Isaque Galdino",
                         "license-type", GTK_LICENSE_GPL_3_0,
                         "website", PACKAGE_URL,
                         "comments", "Manage tasks like a coyote",
                         "authors", authors,
                         "logo-icon-name", "coyotetm",
                         "title", "About CoyoteTM",
                         NULL);
}

static void
ctm_app_quit (GSimpleAction *action,
              GVariant      *parameter,
              gpointer       app)
{
  g_application_quit (G_APPLICATION (app));
}

static void
ctm_app_init (CtmApp *self)
{
  self->db = NULL;
  self->run_tests = FALSE;

  g_application_add_main_option_entries (G_APPLICATION (self), ctm_app_options);
}

static void
ctm_app_startup (GApplication *app)
{
  /* GtkBuilder *builder;
  GMenuModel *menu; */
  const gchar *quit_accels[2] = {"<Ctrl>Q", NULL};

  G_APPLICATION_CLASS (ctm_app_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP (app),
                                   ctm_app_menu,
                                   G_N_ELEMENTS (ctm_app_menu),
                                   app);

  gtk_application_set_accels_for_action (GTK_APPLICATION (app),
                                         "app.quit",
                                         quit_accels);

  /* builder = gtk_builder_new_from_resource ("/org/gtk/coyotetm/menu.ui");
  menu = G_MENU_MODEL (gtk_builder_get_object (builder, "menu"));
  gtk_application_set_app_menu (GTK_APPLICATION (app), menu);
  g_object_unref (builder); */
}

static gint
ctm_app_command_line (GApplication            *app,
                      GApplicationCommandLine *command_line)
{
  CtmApp *self = CTM_APP (app);
  GVariantDict *options = g_application_command_line_get_options_dict (command_line);

  if (g_variant_dict_contains (options, "run-tests"))
    self->run_tests = TRUE;

  g_application_activate (app);

  return 0;
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
ctm_app_activate (GApplication *app)
{
  CtmApp *self = CTM_APP (app);
  GtkWindow *window = gtk_application_get_active_window (GTK_APPLICATION (app));
  const char *db_name = g_build_filename (g_get_user_data_dir (), "coyotetm.db", NULL);
  GFile *db_file = NULL;

  if (self->run_tests)
    {
      db_file = g_file_new_for_path (db_name);
      g_file_delete (db_file, NULL, NULL);
      g_clear_object (&db_file);
    }

  self->db = ctm_db_new ();
  ctm_db_set_filename (self->db, db_name);
  ctm_db_open (self->db);

  if (self->run_tests)
    {
      ctm_db_test (self->db);
    }

  if (!window)
    window = GTK_WINDOW (ctm_window_new (self));

  gtk_window_present (window);
}

static void
ctm_app_class_init (CtmAppClass *class)
{
  G_APPLICATION_CLASS (class)->startup = ctm_app_startup;
  G_APPLICATION_CLASS (class)->command_line = ctm_app_command_line;
  G_APPLICATION_CLASS (class)->activate = ctm_app_activate;
  G_APPLICATION_CLASS (class)->shutdown = ctm_app_shutdown;
}

CtmApp *
ctm_app_new (void)
{
  return g_object_new (CTM_TYPE_APP,
                       "application-id", "org.gtk.coyotetm",
                       "flags", G_APPLICATION_HANDLES_COMMAND_LINE,
                       NULL);
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

