/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Mapinfo.h"

namespace Gameplay
{
	MapInfo::MapInfo()
	{
		bgm = "";
	}

	MapInfo::~MapInfo() {}

	void MapInfo::loadinfo(node src, vector2d<int32_t> walls, vector2d<int32_t> borders)
	{
		string oldbgm = bgm;

		node info = src["info"];
		if (info["VRLeft"].data_type() == node::type::integer)
		{
			mapwalls = vector2d<int32_t>(info["VRLeft"], info["VRRight"]);
			mapborders = vector2d<int32_t>(info["VRTop"], info["VRBottom"]);
		}
		else
		{
			mapwalls = walls;
			mapborders = borders;
		}

		string bgmpath = info["bgm"];
		size_t split = bgmpath.find('/');
		bgm = bgmpath.substr(0, split) + ".img/" + bgmpath.substr(split + 1);
		if (bgm != oldbgm)
		{
			newbgm = true;
		}
		else
		{
			newbgm = false;
		}

		cloud = info["cloud"].get_bool();
		fieldlimit = info["fieldLimit"];
		hideminimap = info["hideMinimap"].get_bool();
		mapmark = info["mapMark"];
		swim = info["swim"].get_bool();
		town = info["town"].get_bool();

		node stsrc = src["seat"];
		for (node sub = stsrc.begin(); sub != stsrc.end(); ++sub)
		{
			Seat seat;
			seat.pos = vector2d<int32_t>(sub.x(), sub.y());
			seats.push_back(seat);
		}

		node lrsrc = src["ladders"];
		for (node lrnode = lrsrc.begin(); lrnode != lrsrc.end(); ++lrnode)
		{
			Ladder ladder;
			ladder.x = lrnode["x"];
			ladder.y1 = lrnode["y1"];
			ladder.y2 = lrnode["y2"];
			ladder.ladder = lrnode["l"].get_bool();
			ladders.push_back(ladder);
		}
	}

	bool MapInfo::hasnewbgm() const
	{
		return newbgm;
	}

	const string& MapInfo::getbgm() const
	{
		return bgm;
	}

	vector2d<int32_t> MapInfo::getwalls() const
	{
		return mapwalls;
	}

	vector2d<int32_t> MapInfo::getborders() const
	{
		return mapborders;
	}

	const Seat* MapInfo::findseat(vector2d<int32_t> pos) const
	{
		vector2d<int32_t> hor = vector2d<int32_t>(pos.x() - 10, pos.x() + 10);
		vector2d<int32_t> ver = vector2d<int32_t>(pos.y() - 10, pos.y() + 10);
		for (vector<Seat>::const_iterator stit = seats.begin(); stit != seats.end(); ++stit)
		{
			if (hor.contains(stit->pos.x()) && ver.contains(stit->pos.y()))
			{
				return stit._Ptr;
			}
		}
		return nullptr;
	}

	const Ladder* MapInfo::findladder(vector2d<int32_t> pos) const
	{
		vector2d<int32_t> hor = vector2d<int32_t>(pos.x() - 25, pos.x() + 25);
		for (vector<Ladder>::const_iterator lrit = ladders.begin(); lrit != ladders.end(); ++lrit)
		{
			vector2d<int32_t> lrver = vector2d<int32_t>(lrit->y1 - 5, lrit->y2);
			if (hor.contains(lrit->x) && lrver.contains(pos.y()))
			{
				return lrit._Ptr;
			}
		}
		return nullptr;
	}
}