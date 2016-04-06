/*
 * Copyright (C) 2002-2004, 2006-2009 by the Widelands Development Team
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "editor/ui_menus/editor_tool_change_height_options_menu.h"

#include <cstdio>
#include <string>

#include <boost/format.hpp>

#include "base/i18n.h"
#include "editor/editorinteractive.h"
#include "editor/tools/editor_increase_height_tool.h"
#include "editor/tools/editor_set_height_tool.h"
#include "graphic/graphic.h"
#include "logic/widelands_geometry.h"

EditorToolChangeHeightOptionsMenu::EditorToolChangeHeightOptionsMenu
	(EditorInteractive          & parent,
	 EditorIncreaseHeightTool & increase_tool,
	 UI::UniqueWindow::Registry  & registry)
	:
	EditorToolOptionsMenu(parent, registry, 350, 100, _("Height Tools Options")),
	increase_tool_(increase_tool),
	box_(this, hmargin(), vmargin(), UI::Box::Vertical, 0, 0, vspacing()),
	change_by_(&box_, 0, 0, get_inner_w() - 2 * hmargin(), 80,
				  1, 1, MAX_FIELD_HEIGHT_DIFF,
				  _("Increase/Decrease Value"), UI::SpinBox::Units::kNone,
				  g_gr->images().get("images/ui_basic/but1.png"),
				  UI::SpinBox::Type::kSmall),
	set_to_(&box_, 0, 0, get_inner_w() - 2 * hmargin(), 80,
			  10, 0, MAX_FIELD_HEIGHT,
			  _("Set Value"), UI::SpinBox::Units::kNone,
			  g_gr->images().get("images/ui_basic/but1.png"),
			  UI::SpinBox::Type::kSmall)
{
	change_by_.set_tooltip(
				/** TRANSLATORS: Editor change height access keys. **/
				(boost::format(_("Use %s to increase, %s to decrease"))
				 /** TRANSLATORS: This is an access key combination. Localize, but do not change the key. **/
				 % _("Click")
				 /** TRANSLATORS: This is an access key combination. Localize, but do not change the key. **/
				 % _("Shift + Click")).str());
	set_to_.set_tooltip(
				/** TRANSLATORS: Editor set height access key. **/
				(boost::format(_("Use %s to set to this value"))
				 /** TRANSLATORS: This is an access key combination. Localize, but do not change the key. **/
				 % _("Ctrl + Click")).str());

	change_by_.changed.connect
		(boost::bind
		 (&EditorToolChangeHeightOptionsMenu::update_change_by, boost::ref(*this)));
	set_to_.changed.connect
		(boost::bind
		 (&EditorToolChangeHeightOptionsMenu::update_set_to, boost::ref(*this)));

	box_.add(&change_by_, UI::Align::kLeft);
	box_.add(&set_to_, UI::Align::kLeft);
	box_.set_size(get_inner_w() - 2 * hmargin(), change_by_.get_h() + set_to_.get_h() + vspacing());
	set_inner_size(box_.get_w() + 2 * hmargin(), box_.get_h() + 2 * vspacing());
}

void EditorToolChangeHeightOptionsMenu::update_change_by() {
	int32_t change_by = change_by_.get_value();
	assert(change_by > 0);
	assert(change_by <= MAX_FIELD_HEIGHT_DIFF);

	increase_tool_                .set_change_by(change_by);
	increase_tool_.decrease_tool().set_change_by(change_by);
	select_correct_tool();
}

void EditorToolChangeHeightOptionsMenu::update_set_to() {
	int32_t set_to = set_to_.get_value();
	assert(set_to >= 0);
	assert(set_to <= MAX_FIELD_HEIGHT);
	increase_tool_.set_tool().set_interval(Widelands::HeightInterval(set_to, set_to));
	select_correct_tool();
}
