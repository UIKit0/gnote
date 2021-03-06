/*
 * gnote
 *
 * Copyright (C) 2011-2014 Aurimas Cernius
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



/* ported from */
/***************************************************************************
 *  ActionManager.cs
 *
 *  Copyright (C) 2005-2006 Novell, Inc.
 *  Written by Aaron Bockover <aaron@abock.org>
 ****************************************************************************/

/*  THIS FILE IS LICENSED UNDER THE MIT LICENSE AS OUTLINED IMMEDIATELY BELOW:
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glibmm/i18n.h>
#include <gtkmm/window.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/image.h>
#include <gtkmm/stock.h>

#include "sharp/string.hpp"
#include "debug.hpp"
#include "actionmanager.hpp"
#include "iconmanager.hpp"

namespace gnote {

  ActionManager::ActionManager()
    : m_main_window_actions(Gtk::ActionGroup::create("MainWindow"))
  {
    populate_action_groups();
    make_app_actions();
    make_app_menu_items();
  }


  void ActionManager::populate_action_groups()
  {
    Glib::RefPtr<Gtk::Action> action;

    action = Gtk::Action::create(
      "QuitGNoteAction", Gtk::Stock::QUIT,
      _("_Quit"), _("Quit Gnote"));
    m_main_window_actions->add(action, Gtk::AccelKey("<Control>Q"));

    action = Gtk::Action::create(
      "ShowPreferencesAction", Gtk::Stock::PREFERENCES,
      _("_Preferences"), _("Gnote Preferences"));
    m_main_window_actions->add(action);

    action = Gtk::Action::create("ShowHelpAction", Gtk::Stock::HELP,
      _("_Contents"), _("Gnote Help"));
    m_main_window_actions->add(action, Gtk::AccelKey("F1"));

    action = Gtk::Action::create(
      "ShowAboutAction", Gtk::Stock::ABOUT,
      _("_About"), _("About Gnote"));
    m_main_window_actions->add(action);

    action = Gtk::Action::create(
      "TrayIconMenuAction",  _("TrayIcon"));
    m_main_window_actions->add(action);

    action = Gtk::Action::create(
      "TrayNewNoteAction", Gtk::Stock::NEW,
      _("Create _New Note"), _("Create a new note"));
    m_main_window_actions->add(action);

    action = Gtk::Action::create(
      "ShowSearchAllNotesAction", Gtk::Stock::FIND,
      _("_Search All Notes"),  _("Open the Search All Notes window"));
    m_main_window_actions->add(action);
  }

  Glib::RefPtr<Gtk::Action> ActionManager::find_action_by_name(const std::string & n) const
  {
    Glib::ListHandle<Glib::RefPtr<Gtk::Action> > actions = m_main_window_actions->get_actions();
    for(Glib::ListHandle<Glib::RefPtr<Gtk::Action> >::const_iterator iter2(actions.begin()); 
        iter2 != actions.end(); ++iter2) {
      if((*iter2)->get_name() == n) {
        return *iter2;
      }
    }
    DBG_OUT("%s not found", n.c_str());
    return Glib::RefPtr<Gtk::Action>();      
  }

  void ActionManager::make_app_actions()
  {
    add_app_action("new-note");
    add_app_action("new-window");
    add_app_action("show-preferences");
    add_app_action("about");
    add_app_action("help-contents");
    add_app_action("quit");
  }

  Glib::RefPtr<Gio::SimpleAction> ActionManager::get_app_action(const std::string & name) const
  {
    for(std::vector<Glib::RefPtr<Gio::SimpleAction> >::const_iterator iter = m_app_actions.begin();
        iter != m_app_actions.end(); ++iter) {
      if((*iter)->get_name() == name) {
        return *iter;
      }
    }
    
    return Glib::RefPtr<Gio::SimpleAction>();
  }

  Glib::RefPtr<Gio::SimpleAction> ActionManager::add_app_action(const std::string & name)
  {
    Glib::RefPtr<Gio::SimpleAction> action = Gio::SimpleAction::create(name);
    m_app_actions.push_back(action);
    return action;
  }

  void ActionManager::add_app_menu_item(int section, int order, const std::string & label,
                                        const std::string & action_def)
  {
    m_app_menu_items.insert(std::make_pair(section, AppMenuItem(order, label, action_def)));
  }

  void ActionManager::make_app_menu_items()
  {
    add_app_menu_item(APP_ACTION_NEW, 100, _("_New Note"), "app.new-note");
    add_app_menu_item(APP_ACTION_NEW, 200, _("New _Window"), "app.new-window");
    add_app_menu_item(APP_ACTION_MANAGE, 100, _("_Preferences"), "app.show-preferences");
    add_app_menu_item(APP_ACTION_LAST, 100, _("_Help"), "app.help-contents");
    add_app_menu_item(APP_ACTION_LAST, 200, _("_About"), "app.about");
    add_app_menu_item(APP_ACTION_LAST, 300, _("_Quit"), "app.quit");
  }

  Glib::RefPtr<Gio::Menu> ActionManager::get_app_menu() const
  {
    Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();

    int pos = 0;
    Glib::RefPtr<Gio::Menu> section = make_app_menu_section(APP_ACTION_NEW);
    if(section != 0) {
      menu->insert_section(pos++, section);
    }

    section = make_app_menu_section(APP_ACTION_MANAGE);
    if(section != 0) {
      menu->insert_section(pos++, section);
    }

    section = make_app_menu_section(APP_ACTION_LAST);
    if(section != 0) {
      menu->insert_section(pos++, section);
    }

    return menu;
  }

  Glib::RefPtr<Gio::Menu> ActionManager::make_app_menu_section(int sec) const
  {
    std::pair<AppMenuItemMultiMap::const_iterator, AppMenuItemMultiMap::const_iterator>
    range = m_app_menu_items.equal_range(sec);

    Glib::RefPtr<Gio::Menu> section;
    if(range.first != m_app_menu_items.end()) {
      std::vector<const AppMenuItem*> menu_items;
      for(AppMenuItemMultiMap::const_iterator iter = range.first; iter != range.second; ++iter) {
        menu_items.push_back(&iter->second);
      }
      std::sort(menu_items.begin(), menu_items.end(), AppMenuItem::ptr_comparator());

      section = Gio::Menu::create();
      for(std::vector<const AppMenuItem*>::iterator iter = menu_items.begin(); iter != menu_items.end(); ++iter) {
        section->append((*iter)->label, (*iter)->action_def);
      }
    }

    return section;
  }

  void ActionManager::add_main_window_search_action(const Glib::RefPtr<Gtk::Action> & action, int order)
  {
    add_main_window_action(m_main_window_search_actions, action, order);
    signal_main_window_search_actions_changed();
  }

  void ActionManager::remove_main_window_search_action(const std::string & name)
  {
    remove_main_window_action(m_main_window_search_actions, name);
    signal_main_window_search_actions_changed();
  }

  std::vector<Glib::RefPtr<Gtk::Action> > ActionManager::get_main_window_search_actions()
  {
    return get_main_window_actions(m_main_window_search_actions);
  }

  void ActionManager::add_main_window_action(std::map<int, Glib::RefPtr<Gtk::Action> > & actions,
                                             const Glib::RefPtr<Gtk::Action> & action, int order)
  {
    std::map<int, Glib::RefPtr<Gtk::Action> >::iterator iter = actions.find(order);
    while(iter != actions.end()) {
      iter = actions.find(++order);
    }
    actions[order] = action;
  }

  void ActionManager::remove_main_window_action(std::map<int, Glib::RefPtr<Gtk::Action> > & actions,
    const std::string & name)
  {
    for(std::map<int, Glib::RefPtr<Gtk::Action> >::iterator iter = actions.begin();
        iter != actions.end(); ++iter) {
      if(iter->second->get_name() == name) {
        actions.erase(iter);
        break;
      }
    }
  }

  std::vector<Glib::RefPtr<Gtk::Action> > ActionManager::get_main_window_actions(
    std::map<int, Glib::RefPtr<Gtk::Action> > & actions)
  {
    std::vector<Glib::RefPtr<Gtk::Action> > res;
    for(std::map<int, Glib::RefPtr<Gtk::Action> >::iterator iter = actions.begin();
        iter != actions.end(); ++iter) {
      res.push_back(iter->second);
    }
    return res;
  }

  void ActionManager::add_tray_menu_item(Gtk::MenuItem & item)
  {
    m_tray_menu_items.push_back(&item);
  }

  void ActionManager::remove_tray_menu_item(Gtk::MenuItem & item)
  {
    for(std::vector<Gtk::MenuItem*>::iterator iter = m_tray_menu_items.begin();
        iter != m_tray_menu_items.end(); ++iter) {
      if(*iter == &item) {
        m_tray_menu_items.erase(iter);
        break;
      }
    }
  }

  std::vector<Gtk::MenuItem*> ActionManager::get_tray_menu_items()
  {
    return m_tray_menu_items;
  }

}
