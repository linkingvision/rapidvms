/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#ifndef __V_SMOT_ALGO_HPP__
#define __V_SMOT_ALGO_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"

using namespace UtilityLib;

#define VVID_MOT_PROC_W 320
#define VVID_MOT_PROC_H 240

#define VVID_MOT_REGION_W (VVID_MOT_PROC_W/8)
#define VVID_MOT_REGION_H (VVID_MOT_PROC_H/8)

class VVidMotRegLine
{
public:
	VVidMotRegLine()
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i++)
		{
			enable[i] = 0;
		}
	}
	VVidMotRegLine(bool bEnable)
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i ++)
		{
			enable[i] = bEnable;
		}
	}
	VVidMotRegLine(const VVidMotRegLine &pLine)
	{
		for (int i = 0; i < VVID_MOT_REGION_W; i ++)
		{
			enable[i] = pLine.enable[i];
		}
	}

public:
	u8 enable[VVID_MOT_REGION_W];
};

class VVidMotReg
{
public:
	VVidMotReg()
	{
		VVidMotRegLine lineIn;
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			line[i] = lineIn;
		}
	}
	VVidMotReg(VVidMotRegLine pLine)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			line[i] = pLine;
		}
	}
	VVidMotReg(const VVidMotReg &pReg)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			line[i] = pReg.line[i];
		}
	}
	VVidMotReg(std::string &pReg)
	{
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			for (int j = 0; j < VVID_MOT_REGION_W; j ++)
			{
				line[i].enable[j] = pReg[i * VVID_MOT_REGION_W + j];
			}
			
		}
	}
	std::string ToString()
	{
		std::string reg;
		for (int i = 0; i < VVID_MOT_REGION_H; i ++)
		{
			for (int j = 0; j < VVID_MOT_REGION_W; j ++)
			{
				char c = line[i].enable[j];
				reg.push_back(c);
			}
			
		}

		return reg;
	}

public:
	static VVidMotReg CreateOff()
	{
		VVidMotRegLine line(0);
		
		VVidMotReg reg(line);
		
		return reg;
	}

	static VVidMotReg CreateAll()
	{
		VVidMotRegLine line(1);
		
		VVidMotReg reg(line);
		return reg;
	}
	static bool CheckStringForReg(std::string &pMot)
	{
		if (pMot.length() != VVID_MOT_REGION_W * VVID_MOT_REGION_H)
		{
			return false;
		}else
		{
			return true;
		}
	}
public:
	VVidMotRegLine line[VVID_MOT_REGION_H];
};


class VSMotAlgoData;
class VE_LIBRARY_API VSMotAlgo
{
public:

	VSMotAlgo(bool bFullFrame);
	~VSMotAlgo();
public:
	bool ProcessFrame(RawFrame& frame);
	bool SetMotReg(VVidMotReg &motReg);
	bool GetOutput(VBlobVec &pOutput, s32 &w, s32 &h);

public:
    VSMotAlgoData * m_data;
};
#endif /* __V_SMOT_ALGO_HPP__ */
