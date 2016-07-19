#ifndef __V_SMOT_ALGO_HPP__
#define __V_SMOT_ALGO_HPP__
#include "utility.hpp"
#include "debug.hpp"
#include "utility/videotype.hpp"

using namespace UtilityLib;

class VSMotAlgoData;
class VE_LIBRARY_API VSMotAlgo
{
public:

	VSMotAlgo(bool bFullFrame);
	~VSMotAlgo();
public:
	bool ProcessFrame(RawFrame& frame);
	bool GetOutput(VBlobVec &pOutput, s32 &w, s32 &h);

public:
    VSMotAlgoData * m_data;
};
#endif /* __V_SMOT_ALGO_HPP__ */
