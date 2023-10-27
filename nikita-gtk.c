#include <libxml/parser.h>
#include <libsoup/soup.h>
#include <libsoup/soup-message.h>
#include <glib/gstdio.h>
#include <glib/goption.h>
#include <gtk/gtk.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkwindow.h>

typedef struct {
        GtkWidget *widget;
        gint index;
        const gchar *title;
        GtkAssistantPageType type;
        gboolean complete;
} PageInfo;

GtkWidget *role_entry;
GtkWidget *role_label;
GtkWidget *server_entry;
GtkWidget *server_label;
GtkWidget *login_entry;
GtkWidget *login_label;

GMainLoop *main_loop;

/* Commandline options */
static gint timeout = 3600; /* seconds */
static gint time_threshold;
GMainLoop *main_loops;

static GOptionEntry entries[] =
{
        { "timeout",
          't',
          0,
          G_OPTION_ARG_INT,
          &timeout,
          "Exit after T seconds. Default: 3600",
          "T" },
        { "time-threshold",
          'i',
          0,
          G_OPTION_ARG_INT,
          &time_threshold,
          "Only report location update after T seconds. "
	  "Default: 0 (report new location without any delay)",
          "T" },
        { "filename",
          'f',
          0,
          G_OPTION_ARG_STRING,
          &server_entry,
          "Filename.  Default: file:///tmp/arkivstruktur.xml",
          "F" },
        { "noark5v5-server",
          's',
          0,
          G_OPTION_ARG_STRING,
          &server_entry,
          "Noark5v5 Server.  Default: http://www.arkivarium.no:8096/noark5v5",
          "S" },
        { NULL }
};



static void nikita_gtk_wizard_bootup(GtkAssistant * assistant, gpointer data)
{
  SoupMessage *msg;
  SoupMessageHeaders *response_headers;
  SoupSession *session;
  guint status;
  session = soup_session_sync_new ();
  msg = soup_message_new ("GET", "http://www.arkivarium.no:8092/noark5v5");
  soup_message_set_request (msg, "application/xml",
			    SOUP_MEMORY_COPY,
			    g_strdup("arkivdel"), strlen("arkivdel"));
  status = soup_session_send_message (session, msg);
  g_object_unref (msg);
  g_object_unref (session);
}

static void nikita_gtk_wizard_apply(GtkAssistant * assistant, gpointer data)
{
  SoupMessage *msg;
  SoupMessageHeaders *response_headers;
  SoupSession *session;
  guint status;
  session = soup_session_sync_new ();
  msg = soup_message_new ("GET", "http://www.arkivarium.no:8092/noark5v5");
  soup_message_set_request (msg, "application/xml",
			    SOUP_MEMORY_COPY,
			    g_strdup("arkivdel"), strlen("arkivdel"));
  status = soup_session_send_message (session, msg);
  g_object_unref (msg);
  g_object_unref (session);
}

static void nikita_gtk_wizard_close(GtkAssistant * assistant, gpointer data)
{
        gtk_main_quit();
}

static void nikita_gtk_wizard_cancel(GtkAssistant * assistant, gpointer data)
{
        if (!main_loops) {
                g_error("Quit more loops than there are.");
        } else {
                GMainLoop *loop = main_loops;
                g_main_loop_quit(loop);
                gtk_main_quit();
        }
}

static void on_volume_increase(GtkWidget *widget, gpointer data) {
}

static void on_volume_archival(GtkWidget *widget, gpointer data) {
}

static void on_volume_searcher(GtkWidget *widget, gpointer data) {
}

static gint
my_popup_handler (GtkWidget *widget, GdkEvent *event)
{
  GtkMenu *menu;
  GdkEventButton *event_button;

  g_return_val_if_fail (widget != NULL, FALSE);
  g_return_val_if_fail (GTK_IS_MENU (widget), FALSE);
  g_return_val_if_fail (event != NULL, FALSE);

  // The "widget" is the menu that was supplied when
  // `g_signal_connect_swapped()` was called.
  menu = GTK_MENU (widget);

  if (event->type == GDK_BUTTON_PRESS)
    {
      event_button = (GdkEventButton *) event;
      if (event_button->button == GDK_BUTTON_SECONDARY)
        {
          gtk_menu_popup (menu, NULL, NULL, NULL, NULL,
                          event_button->button, event_button->time);
          return TRUE;
        }
    }

  return FALSE;
}

static void nikita_gtk_wizard_entry_changed(GtkEditable * editable,
					    GtkAssistant * assistant,
					    gpointer *data)
{
        return;
}

static void nikita_gtk_wizard_button_toggled(GtkCheckButton * checkbutton,
					     GtkAssistant * assistant)
{
  return;
}

static void nikita_gtk_wizard_button_clicked(GtkButton * button,
					     GtkAssistant * assistant)
{
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *increaseButton;
    GtkWidget *archivalButton;    
    GtkWidget *searcherButton;
    GtkWidget *introduction;
    int i = 0;
    PageInfo page[4] = {
      {NULL, -1, "Arkivarium (nikita-gtk) 0.0.1", GTK_ASSISTANT_PAGE_INTRO, TRUE},
      {NULL, -1, "Role", GTK_ASSISTANT_PAGE_CONTENT, TRUE},
      {NULL, -1, "Server", GTK_ASSISTANT_PAGE_CONTENT, TRUE},
      {NULL, -1, "Login", GTK_ASSISTANT_PAGE_CONFIRM, TRUE},
    };
    
    window = gtk_application_window_new(app);
    introduction = gtk_assistant_new();
    gtk_container_add (GTK_WINDOW (window), introduction);
    gtk_widget_set_size_request(GTK_WIDGET(introduction), 800, 600);
    gtk_window_set_title(GTK_WINDOW(introduction), "Arkivarium (nikita-gtk)");
        g_signal_connect(G_OBJECT(introduction), "destroy",
                         G_CALLBACK(gtk_main_quit), NULL);
        page[0].widget = gtk_label_new("Welcome to Arkivarium!\n\n");
        page[1].widget = gtk_box_new(FALSE, 3);
        role_label = gtk_label_new("<role>:");
        role_entry = gtk_entry_new();
        gtk_entry_set_max_length (role_entry, 256);
        gtk_box_pack_start(GTK_BOX(page[1].widget), GTK_WIDGET(role_label),
                           FALSE, FALSE, 3);
        gtk_box_pack_start(GTK_BOX(page[1].widget), GTK_WIDGET(role_entry),
                           FALSE, FALSE, 3);
	g_signal_connect(G_OBJECT(role_label), "clicked",
			 G_CALLBACK(nikita_gtk_wizard_bootup),
			 gtk_entry_get_text(GTK_ENTRY(role_entry)));
        page[2].widget = gtk_box_new(FALSE, 3);
        server_label = gtk_label_new("<server>");
        server_entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(page[2].widget), GTK_WIDGET(server_label),
			   FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(page[2].widget), GTK_WIDGET(server_entry),
			   FALSE, FALSE, 3);
	g_signal_connect(G_OBJECT(server_label), "clicked",
                         G_CALLBACK(nikita_gtk_wizard_bootup),
                         gtk_entry_get_text(GTK_ENTRY(server_entry)));
	page[3].widget = gtk_box_new(FALSE, 3);
	login_label = gtk_label_new("<credential>");
	login_entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(page[3].widget), GTK_WIDGET(login_label),
			   FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(page[3].widget), GTK_WIDGET(login_entry),
			   FALSE, FALSE, 3);
	g_signal_connect(G_OBJECT(login_label), "clicked",
                         G_CALLBACK(nikita_gtk_wizard_apply),
                         gtk_entry_get_text(GTK_ENTRY(login_entry)));
        for (i = 0; i < 4; i++) {
                page[i].index = gtk_assistant_append_page(GTK_ASSISTANT(introduction),
                                              GTK_WIDGET(page[i].widget));
                gtk_assistant_set_page_title(GTK_ASSISTANT(introduction),
                                             GTK_WIDGET(page[i].widget),
                                             page[i].title);
                gtk_assistant_set_page_type(GTK_ASSISTANT(introduction),
                                            GTK_WIDGET(page[i].widget),
                                            page[i].type);
                gtk_assistant_set_page_complete(GTK_ASSISTANT(introduction),
                                                GTK_WIDGET(page[i].widget),
                                                page[i].complete);
        }
	g_signal_connect(G_OBJECT(login_label), "clicked",
                         G_CALLBACK(nikita_gtk_wizard_bootup),
                         gtk_entry_get_text(GTK_ENTRY(login_entry)));
	g_signal_connect(G_OBJECT(role_entry), "changed",
			 G_CALLBACK(nikita_gtk_wizard_entry_changed), role_entry);			 
	g_signal_connect(G_OBJECT(server_entry), "changed",
                         G_CALLBACK(nikita_gtk_wizard_entry_changed), server_entry);
	g_signal_connect(G_OBJECT(login_entry), "changed",
			 G_CALLBACK(nikita_gtk_wizard_entry_changed), login_entry);			 
        g_signal_connect(G_OBJECT(introduction), "cancel",
                         G_CALLBACK(nikita_gtk_wizard_cancel), main_loops);
        g_signal_connect(G_OBJECT(introduction), "close",
                         G_CALLBACK(nikita_gtk_wizard_close), NULL);
        g_signal_connect(G_OBJECT(introduction), "apply",
                         G_CALLBACK(nikita_gtk_wizard_bootup), login_entry);
        gtk_widget_show_all (GTK_WIDGET (introduction));
	gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
	gtk_window_set_title(GTK_WINDOW(window), "Arkivarium (nikita-gtk)");
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
	increaseButton = gtk_button_new_with_label("Utvid arkiv");
	g_signal_connect(increaseButton, "clicked", G_CALLBACK(on_volume_increase), NULL);
	archivalButton = gtk_button_new_with_label("Bevar arkiv");
	g_signal_connect(searcherButton, "clicked", G_CALLBACK(on_volume_archival), NULL);
	searcherButton = gtk_button_new_with_label("Søk i arkiv");
	g_signal_connect(searcherButton, "clicked", G_CALLBACK(on_volume_searcher), NULL);
	GtkWidget *buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_set_border_width(GTK_CONTAINER(buttonBox), 10);
	gtk_box_pack_start(GTK_BOX(buttonBox), increaseButton, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttonBox), archivalButton, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttonBox), searcherButton, FALSE, FALSE, 0);
	
	GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, FALSE, FALSE, 0);
	
	/*
	  nikita_set_uri("http://www.arkivarium.no:8092/noark5v5");
	  nikita_view_archive);
	*/
	
	gtk_container_add(GTK_CONTAINER(window), mainBox);
	gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;
  int entries_n = 2;
  int entryNumber;
  GError *error = NULL;
  GOptionContext *context;
  context = g_option_context_new ("- Arkivarium (nikita-gtk) 0.0.1");
  g_option_context_add_main_entries (context, entries, "nikita-gtk");
  g_option_context_add_group (context, gtk_get_option_group (TRUE));
  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("Arkivarium (nikita-gtk) 0.0.1 Option parsing failed: %s\n", error->message);
    return EXIT_FAILURE;
  }
  app = gtk_application_new("www.arkivarium.no", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  
  return status;
}
