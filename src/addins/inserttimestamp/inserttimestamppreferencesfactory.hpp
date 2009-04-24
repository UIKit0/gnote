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

#ifndef _INSERTTIMESTAMP_PREFERENCES_FACTORY_HPP__
#define _INSERTTIMESTAMP_PREFERENCES_FACTORY_HPP__


#include "addinpreferencefactory.hpp"
#include "inserttimestamppreferences.hpp"

namespace inserttimestamp {


  class InsertTimestampPreferencesFactory
    : public gnote::AddinPreferenceFactory
  {
  public:
    static gnote::AddinPreferenceFactory * create()
      {
        return new InsertTimestampPreferencesFactory;
      }
    virtual Gtk::Widget * create_preference_widget()
      {
        return Gtk::manage(new InsertTimestampPreferences());
      }
  };


}

#endif