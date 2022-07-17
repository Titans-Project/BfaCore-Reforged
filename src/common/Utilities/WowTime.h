/*
 * Copyright (C) 2022 BfaCore Reforged
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

#include "Define.h"

#pragma once

namespace MS
{
    namespace Utilities
    {
        namespace Globals
        {
            namespace InSeconds
            {
                enum
                {
                    Seconds = 1,
                    Minute = 60 * InSeconds::Seconds,
                    Hour = 60 * InSeconds::Minute,
                    Day = 24 * InSeconds::Hour,
                    Week = InSeconds::Day * 7,
                };
            }

            namespace InMinutes
            {
                enum
                {
                    Minute = 1,
                    Hour = 60 * InMinutes::Minute,
                    Day = 24 * InMinutes::Hour,
                    Week = InMinutes::Day * 7,
                };
            }
        }

        class TC_COMMON_API WowTime
        {
        public:
            WowTime();

            uint32 Encode();
            static uint32 Encode(time_t p_Time);
            void Decode(uint32 p_EncodedTime);
            static WowTime FromEncodedTime(uint32 p_EncodedTime);

            void AddDays(int32 p_Count, bool p_KeepHoursAndMinutes);
            void AddMinutes(int32 p_Count);
            void AddHolidayDuration(int32 p_Duration);

            uint32 GetDaysSinceEpoch();
            time_t GetPosixTimeFromUTC();
            time_t GetPosixTime() const;
            time_t GetHourAndMinutes();

            void SetUTCTimeFromPosixTime(time_t p_PosixTime);
            void SetHourAndMinutes(uint32 p_Minutes);
            void SetHourAndMinutes(uint32 p_Hours, uint32 p_Minutes);
            bool SetDate(uint32 p_Month, uint32 p_MonthDay, uint32 p_Year);

            time_t DiffTime(WowTime const& other);
            bool IsSameDay(WowTime const& other);
            bool InRange(WowTime const& p_Left, WowTime const& right);
            void ComputeRegionTime(WowTime& other);

            bool operator==(WowTime const& other) const;
            bool operator!=(WowTime const& other) const;
            bool operator<(WowTime const& other) const;
            bool operator<=(WowTime const& other) const;
            bool operator>(WowTime const& other) const;
            bool operator>=(WowTime const& other) const;

        public:
            int32 Minute;
            int32 Hour;
            int32 WeekDay;
            int32 MonthDay;
            int32 Month;
            int32 Year;
            int32 Unk1;
            int32 YearDay;
        };

    }   ///< namespace Utilities
}   ///< namespace MS
