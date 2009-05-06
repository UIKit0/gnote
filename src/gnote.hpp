/*
 * gnote
 *
 * Copyright (C) 2009 Hubert Figuiere
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




#ifndef _GNOTE_HPP_
#define _GNOTE_HPP_

#include <string>

#include <glibmm/optioncontext.h>
#include <glibmm/ustring.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/statusicon.h>

#include "base/singleton.hpp"
#include "actionmanager.hpp"
#include "keybinder.hpp"
#include "tray.hpp"

namespace gnote {

class PreferencesDialog;
class NoteManager;

class Gnote
  : public base::Singleton<Gnote>
{
public:
  Gnote();
  ~Gnote();
  int main(int argc, char **argv);
  std::string get_note_path(const std::string & override_path);
  NoteManager & default_note_manager()
    {
      return *m_manager;
    }
  IKeybinder & keybinder()
    {
      return *m_keybinder;
    }

  void setup_global_actions();
  void start_tray_icon();
  bool check_tray_icon_showing();

  void on_new_note_action();
  void on_quit_gnote_action();
  void on_preferences_response(int res);
  void on_show_preferences_action();
  void on_show_help_action();
  void on_show_about_action();
  void open_search_all();
  void open_note_sync_window();

  static std::string conf_dir();
  static bool tray_icon_showing()
    {
      return s_tray_icon_showing;
    }
  bool is_panel_applet()
    {
      return m_is_panel_applet;
    }
  void set_tray(const Tray::Ptr & tray)
    {
      m_tray = tray;
    }
private:
  NoteManager *m_manager;
  IKeybinder  *m_keybinder;
  Glib::RefPtr<Gtk::IconTheme> m_icon_theme;
  static bool s_tray_icon_showing;
  Glib::RefPtr<TrayIcon> m_tray_icon;
  Tray::Ptr m_tray;
  bool m_is_panel_applet;
  PreferencesDialog *m_prefsdlg;
};


class GnoteCommandLine
  : public Glib::OptionGroup
{
public:
  GnoteCommandLine();
  int execute();

  const Glib::ustring & note_path() const
    {
      return m_note_path;
    }
  bool        needs_execute() const;
  bool        use_panel_applet() const
    {
      return m_use_panel;
    }

private:
  void        print_version();

  bool        m_new_note;
  bool        m_open_start_here;
  bool        m_use_panel;
  bool        m_show_version;
  std::string m_open_note_name;
  std::string m_open_note_uri;
  std::string m_open_external_note_path;
  Glib::ustring m_search;
  Glib::ustring m_note_path;
};

}

#endif
