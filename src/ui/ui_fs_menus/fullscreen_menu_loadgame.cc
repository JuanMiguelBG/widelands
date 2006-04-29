/*
 * Copyright (C) 2002 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <stdio.h>
#include "error.h"
#include "fullscreen_menu_loadgame.h"
#include "game.h"
#include "game_loader.h"
#include "game_preload_data_packet.h"
#include "graphic.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_multilinetextarea.h"
#include "ui_textarea.h"
#include "wlapplication.h"

/*
==============================================================================

Fullscreen_Menu_LoadGame

==============================================================================
*/

Fullscreen_Menu_LoadGame::Fullscreen_Menu_LoadGame(Game *g, bool is_singleplayer)
	: Fullscreen_Menu_Base("choosemapmenu.jpg")
{
	// Text
   UITextarea* title= new UITextarea(this, MENU_XRES/2, 90, _("Choose saved game!"), Align_HCenter);
   title->set_font(UI_FONT_BIG, UI_FONT_CLR_FG);

	// UIButtons
	UIButton* b;

	b = new UIButton(this, 410, 356, 174, 24, 0, 0);
	b->clickedid.set(this, &Fullscreen_Menu_LoadGame::end_modal);
	b->set_title("Back");

	m_ok = new UIButton(this, 410, 386, 174, 24, 2, 0);
	m_ok->clicked.set(this, &Fullscreen_Menu_LoadGame::ok);
	m_ok->set_title(_("OK"));
	m_ok->set_enabled(false);

	// Create the list area
	list = new UIListselect(this, 12, 163, 363, 297);
	list->selected.set(this, &Fullscreen_Menu_LoadGame::map_selected);
   list->double_clicked.set(this, &Fullscreen_Menu_LoadGame::double_clicked);

	// Info fields
	new UITextarea(this, 450, 180, _("Map Name:"), Align_Right);
	tamapname = new UITextarea(this, 460, 180, "");
	new UITextarea(this, 450, 200, _("Gametime:"), Align_Right);
	tagametime = new UITextarea(this, 460, 200, "");

   fill_list();
}

Fullscreen_Menu_LoadGame::~Fullscreen_Menu_LoadGame()
{
}

void Fullscreen_Menu_LoadGame::ok()
{
   std::string filename=static_cast<const char*>(list->get_selection());

   m_filename = filename;

   end_modal(1);
}

void Fullscreen_Menu_LoadGame::map_selected(int id)
{
   const char* name=static_cast<const char*>(list->get_selection());

   if (name)
   {
      FileSystem* fs = g_fs->MakeSubFileSystem( name );
      Game_Loader gl(fs, game);
      Game_Preload_Data_Packet gpdp;
      gl.preload_game(&gpdp); // This has worked before, no problem

      m_ok->set_enabled(true);
      tamapname->set_text(gpdp.get_mapname());

      char buf[200];
      uint gametime = gpdp.get_gametime();

      int hours = gametime / 3600000;
      gametime -= hours * 3600000;
      int minutes = gametime / 60000;

      sprintf(buf, "%02i:%02i", hours, minutes);
      tagametime->set_text(buf);

      delete fs;
   } else {
      tamapname->set_text("");
      tagametime->set_text("");
   }
}

/*
 * listbox got double clicked
 */
void Fullscreen_Menu_LoadGame::double_clicked(int i) {
   // Ok
   ok();

}

/*
 * fill the file list
 */
void Fullscreen_Menu_LoadGame::fill_list(void) {
   // Fill it with all files we find.
   g_fs->FindFiles("ssave", "*", &m_gamefiles, 1);

   Game_Preload_Data_Packet gpdp;

   for(filenameset_t::iterator pname = m_gamefiles.begin(); pname != m_gamefiles.end(); pname++) {
      const char *name = pname->c_str();

      Game_Loader* gl = 0;
      FileSystem* fs = 0;

      try {
         fs = g_fs->MakeSubFileSystem( name );
         gl = new Game_Loader(fs, game);
         gl->preload_game(&gpdp);

         char* fname = strdup(FS_Filename(name));
         FS_StripExtension(fname);
         list->add_entry(fname, reinterpret_cast<void*>(const_cast<char*>(name)));
         free(fname);

      } catch(wexception& ) {
         // we simply skip illegal entries
      }
      if( gl )
         delete gl;
      if( fs )
         delete fs;
   }

   if(list->get_nr_entries())
      list->select(0);
}
