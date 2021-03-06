/* 
Story Editor - Story editor application developed wiht GTK3 and C.

Copyright (C) (C) Tuomas Lähteenmäki, 2020

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sqlite3.h>
#include <gtk/gtk.h>

#define NUM_WORDS  1   // how many names generated.

int lineCounter(char filename[]);  // Count how many lines are txt files.
char rmaleName(int nline);
char commonfamilyNames();

//------------------------------------------------------------------
// Custom structure that holds pointers to widgets and user variables

typedef struct {
// Add pointers to widgets below
// GtkWidget *w_window_main;
//------------------------------------------------------------------

   	GtkWidget *w_selectDatabase;
   	GtkWidget *w_newCharacterWindow; // Pointer to new character dialog window
	GtkWidget *w_aboutWindow; // Pointer to about dialog window
	GtkWidget *w_settingsWindow; // Pointer to settings window
} 	app_widgets;

	

int main(int argc, char *argv[])
{
	
    	GtkBuilder      *builder; 
    	GtkWidget       *window;
    	app_widgets     *widgets = g_slice_new(app_widgets); // Instantiate structure, allocating memory for it

    	gtk_init(&argc, &argv); // init Gtk

//------------------------------------------------------------------
// Establish contacht with xml code user to adjust widget settings
//------------------------------------------------------------------
   
    	builder = gtk_builder_new_from_file("glade/window_main.glade");

    	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//------------------------------------------------------------------
// Get pointers to widgets here
// widgets->w_x  = GTK_WIDGET(gtk_builder_get_object(builder, "x"));
//------------------------------------------------------------------

	widgets->w_selectDatabase  = GTK_WIDGET(gtk_builder_get_object(builder, "selectDatabase"));
	widgets->w_newCharacterWindow  = GTK_WIDGET(gtk_builder_get_object(builder, "newCharacterWindow"));
    	widgets->w_aboutWindow  = GTK_WIDGET(gtk_builder_get_object(builder, "aboutWindow"));
	widgets->w_settingsWindow = GTK_WIDGET(gtk_builder_get_object(builder, "settingsWindow"));

    	
    	gtk_builder_connect_signals(builder, widgets); // Widgets pointer are passed to all widget handler functions as the user_data parameter

    	g_object_unref(builder);
	
    	gtk_widget_show(window);                
    	gtk_main();
    	g_slice_free(app_widgets, widgets); // Free up widget structure memory

    	return 0;
}

// Dummy handler function
void x(GtkButton *button, app_widgets *app_wdgts){ }


//---------------------------------------------------------------------
// MENU ITEMS ACTIVATE



void on_menuCreateNewCharacter_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts){
	gtk_widget_show(app_wdgts->w_newCharacterWindow);
}

void on_menuSettingsWindow_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts){
	gtk_widget_show(app_wdgts->w_settingsWindow);
}


void on_menuQuit_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts){
	printf("Quit program.\n");
	gtk_main_quit();
}

void on_menuAbout_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts){
	gtk_widget_show(app_wdgts->w_aboutWindow);
}


//--------------------------------------------------------------------------------------
// RESPONSES


// newCharaterWindow dialog box and cancel button working (not x button top application)
//--------------------------------------------------------------------------------------

void on_newCharacterWindow_response(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_newCharacterWindow);
}

void on_cancelBtn_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_newCharacterWindow);
}

void on_nameGeneratorBtn_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Clicked generate name button.\n");

    	time_t t;
	
	int nline = lineCounter("names.txt"); 
	

    	// Open words file 
    	FILE *fp = fopen("names.txt", "r");

    	if (fp == NULL) {
        	perror("Unable to locate word list");
        	exit(EXIT_FAILURE);
    	}

    	
    	// fseek(fp, 0, SEEK_SET);  // Return the beginner of the file


    	// Count words in file 
    	char word[nline];
    	long wc = 0;
    	while (fgets(word, sizeof word, fp) != NULL) {
            ++wc;
    	}

    	// Store random words in array 
    	char randwords[NUM_WORDS][nline];
    	srand((unsigned)time(&t));
    	for (size_t i = 0; i < NUM_WORDS; i++) {
             rewind(fp);
        	int sel = rand() % wc + 1;
        	for (int j = 0; j < sel; j++) {
            	if (fgets(word, sizeof word, fp) == NULL) {
                	perror("Error in fgets()");
            	}
        	}
        	strcpy(randwords[i], word);
    	}

    	if (fclose(fp) != 0) {
        perror("Unable to close file");
    	}
    	fclose(fp);

    	// Display results 
    	for (size_t i = 0; i < NUM_WORDS; i++) {
        	printf("%s", randwords[i]);
    	}
	

	
}

// About dialog box close button working (not x button top application)
//--------------------------------------------------------------------------------------
void on_aboutWindow_response(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_aboutWindow);
}
void on_closeBtn_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_aboutWindow);
}


void on_settingsCloseBtn1_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Settings cancel button clicked.\n");
	gtk_widget_hide(app_wdgts->w_settingsWindow);
}

void on_settingsSaveBtn1_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Settings save button clicked.\n");

}

//--------------------------------------------------------------------------------------
// OTHERS


// when open button clicked in selectDatabase menu
void on_openDatabaseButton_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Open selected database, clicked open button.\n");
}

void on_addNewstoryDatabase_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Add new story in database, clicked add button.\n");
}

// when close button clicked in selectDatabase menu
void on_closeButton_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	gtk_widget_hide(app_wdgts->w_selectDatabase);
}

// when delete button clicked in selectDatabase menu
void on_deleteStoryDatabase_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Delete story in database, clicked delete button.\n");
}

// when edit button clicked in selectDatabase menu
void on_editStoryDatabase_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
	printf("Edit selected database, clicked edit button.\n");
}

void on_globalSettingsBtn_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts) 
{
	printf("Toolbar global settings button pressed.\n");
} 

void on_selectDatabase_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts) 
{
	printf("Toolbar database button pressed.\n");
} 

void on_globalShortcutsBtn_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts) 
{
	printf("Toolbar shortcuts button pressed.\n");
} 

void on_nameGeneratorBt_clicked(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts) 
{
	printf("Toolbar name generator button pressed.\n");
} 

// called when window is closed
void on_window_main_destroy()
{
	gtk_main_quit();
}

int lineCounter(char filename[]) 
{
	int nline=0;
	
	/* Open words file */
	
    	FILE *fp = fopen(filename, "r");

    	if (fp == NULL) {
        	perror("Unable to locate word list");
        	exit(EXIT_FAILURE);
    	}

    	int c = getc(fp);
    	while (c != EOF){	
 	    c = getc(fp);
            if (c== '\n')
               nline++;
    	}
	fclose(fp);
	return nline;
}

char rmaleName(int nline) 
{
   	time_t t;
	

    	/* Open words file */
    	FILE *fp = fopen("names.txt", "r");

    	if (fp == NULL) {
        	perror("Unable to locate word list");
        	exit(EXIT_FAILURE);
    	}

    	
        fseek(fp, 0, SEEK_SET);  // Return the beginner of the file

    	/* Count words in file */
    	char word[nline];
    	long wc = 0;
    	while (fgets(word, sizeof word, fp) != NULL) {
            ++wc;
    	}

    	/* Store random words in array */
    	char randwords[NUM_WORDS][nline];
    	srand((unsigned)time(&t));
    	for (size_t i = 0; i < NUM_WORDS; i++) {
             rewind(fp);
        	int sel = rand() % wc + 1;
        	for (int j = 0; j < sel; j++) {
            	if (fgets(word, sizeof word, fp) == NULL) {
                	perror("Error in fgets()");
            	}
        	}
        	strcpy(randwords[i], word);
    	}

    	if (fclose(fp) != 0) {
        perror("Unable to close file");
    	}
    	fclose(fp);

    	/* Display results */
    	for (size_t i = 0; i < NUM_WORDS; i++) {
        	printf("%s", randwords[i]);
	
    	}
	return 0;
}

char commonfamilyNames() 
{
	 /* 	
	 This print now all table names in command line.

	 The most common first names for men in 1860-69 at Finland. 
	 This based on https://www.tuomas.salste.net/suku/kela-etunimet.html#M1 research.
	 The purpose of this is to initially generate one or more names for use by the program. 
	 The lists grow over the years, so the solution needs to be developed. This first list is only 14 names long. 
	 Maybe last list hundreds of thousands names. And Later add middle names and lastnames. 
	 */

	int i;
	// The most 14 common first names for men in 1860-69 at Finland.
	// const char* firstnamesMen[14] = {"Juho", "Kalle", "Matti", "Johan", "Antti", "Heikki", "Karl", "Frans", "Kustaa", "Pekka", "August", "Otto", "Mikko", "Jaakko"};

	// The most 2 common middle names for men in 1860-69 at Finland.
	 // const char* middlenamesMen[2] = {"Kustaa", "Vihtori"};
		
	// The most 50 common family names is 1939–1958 at Finland.
	const char* lastnames[50] = { "Virtanen", "Nieminen", "Mäkinen", "Laine", "Korhonen", "Koskinen", "Järvinen", "Mäkelä", "Hämäläinen", "Lehtinen", "Lehtonen", "Salminen", "Heinonen", "Saarinen", "Niemi", "Tuominen", "Salonen", "Heikkinen", "Rantanen", "Salo", "Jokinen", "Aaltonen", "Laitinen", "Turunen", "Johansson", "Lahtinen", "Heikkilä", "Laaksonen", "Karjalainen", "Kinnunen", "Ahonen", "Savolainen", "Laakso", "Toivonen", "Mattila", "Miettinen", "Leppänen", "Aalto", "Nurmi", "Hiltunen", "Lehto", "Leinonen", "Peltonen", "Väisänen", "Kallio", "Hakala", "Lindholm", "Ojala", "Manninen", "Karlsson"};
	
    	for (i=0;i<14;i++) {

        	printf("%s\n", lastnames[i]);
		
    	}
	return 0;
 }
