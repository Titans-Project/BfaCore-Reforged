/*
 * Copyright 2021 BfaCore Reforged
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Banner.h"
#include "GitRevision.h"
#include "StringFormat.h"

void Trinity::Banner::Show(char const* applicationName, void(*log)(char const* text), void(*logExtraInfo)())
{
    log(Trinity::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str() );
    log("<Ctrl-C> to stop.\n                                                                    ");
    log(" /#######  /######         /######                                                     ");
    log("| ##__  ##/##__  ##       /##__  ##                                                    ");
    log("| ##  \\ #| ##  \\__/######| ##  \\__/ /######  /######  /######                       ");
    log("| #######| ####  |____  #| ##      /##__  ##/##__  ##/##__  ##                         ");
    log("| ##__  #| ##_/   /######| ##     | ##  \\ #| ##  \\__| ########                       ");
    log("| ##  \\ #| ##    /##__  #| ##    #| ##  | #| ##     | ##_____/                        ");
    log("| #######| ##    | ######|  ######|  ######| ##     |  #######                         ");
    log("|_______/|__/    \\_______/\\______/ \\______/|__/      \\_______/                     ");
	log("																						");
	log("	   World of Warcraft Emulator | Battle for Azeroth 8.3.7                            ");
    log("	https://discord.gg/hTtSYh7WbQ | bfacore.reforged@gmail.com \n                        ");

    if (logExtraInfo)
        logExtraInfo();
}
