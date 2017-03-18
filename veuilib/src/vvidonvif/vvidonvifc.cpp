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
#include "vvidonvif/vvidonvifc.hpp"

#include "onvifclient.hpp"
#include "Poco/URI.h"
#include "Poco/String.h"


bool VVidOnvifC::CheckOnline(astring strUser, astring strPasswd, astring strUrl)
{
	bool bRet = false;
	OnvifClientDevice * pClient  = new OnvifClientDevice(strUrl, 
	 					strUser, strPasswd);
	if (pClient->GetCapabilities() == SOAP_OK)
	{
		bRet = true;
	}

	delete pClient;
	 
	return bRet;
}
bool VVidOnvifC::GetProfiles(astring strUser, astring strPasswd, astring strUrl, 
		VVidOnvifProfileMap &pMap)
{
	bool bRet = false;
	OnvifClientDevice * pClient  = new OnvifClientDevice(strUrl, 
	 					strUser, strPasswd);
	if (pClient->GetCapabilities() == SOAP_OK)
	{
		OnvifClientMedia *pMedia = new OnvifClientMedia(*pClient);

		_trt__GetProfilesResponse profiles;

		if (pMedia->GetProfiles(profiles) == SOAP_OK)
		{
			if (profiles.Profiles.size() > 0)
			{
				bRet = true;
			}
			/* loop to get the profile */
			for (vector<tt__Profile * >::iterator iter =
					profiles.Profiles.begin(); 
					iter < profiles.Profiles.end(); ++iter)
			{

				tt__Profile * pProfile = *iter;
				if (pProfile && pProfile->VideoEncoderConfiguration)
				{
					tt__VideoEncoderConfiguration *pEnc = pProfile->VideoEncoderConfiguration;
					VVidOnvifProfile oneProfile;
					oneProfile.strProfileToken = pProfile->token;
					switch (pEnc->Encoding)
					{
						case tt__VideoEncoding__JPEG:
						{
							oneProfile.strCodecName = "MJPEG";
							break;
						}
						case tt__VideoEncoding__MPEG4:
						{
							oneProfile.strCodecName = "MPEG4";
							break;
						}
						case tt__VideoEncoding__H264:
						{
							oneProfile.strCodecName = "H264";
							break;
						}
						default:
						   	oneProfile.strCodecName = "UnSupport";
							break;
					};
					if (pEnc->Resolution)
					{
						oneProfile.nWidth = pEnc->Resolution->Width;
						oneProfile.nHeight = pEnc->Resolution->Height;
					}
					if (pEnc->RateControl)
					{
						oneProfile.nBandwidth = pEnc->RateControl->BitrateLimit;
						oneProfile.nFps = pEnc->RateControl->FrameRateLimit;
					}
					oneProfile.strNameDisplay = oneProfile.strCodecName + " " + std::to_string(oneProfile.nWidth) +  " " +
						+"x" + " " + std::to_string(oneProfile.nHeight) + " " +  std::to_string(oneProfile.nFps) +
						"fps" + " " +  std::to_string(oneProfile.nBandwidth) + "bps";

					/* Get the Stream url */
					
					pMap[pProfile->token] = oneProfile;
				}
				
			}
		}

		delete pMedia;
		
	}

	if (pMap.size() > 0)
	{
		VVidOnvifProfileMap::iterator it = pMap.begin(); 
		for(; it!=pMap.end(); ++it)
		{
			OnvifClientMedia *pMedia = new OnvifClientMedia(*pClient);
			_trt__GetStreamUriResponse url;
			if (pMedia->GetStreamUrl((*it).first, url) == SOAP_OK)
			{
				if (url.MediaUri)
				{
					(*it).second.strRtspUrl = Poco::replace(url.MediaUri->Uri, "&amp;", "&");
					(*it).second.bGotUrl = true;
				}
			}
			delete pMedia;
		}		
	}

	delete pClient;
	 
	return bRet;
}


class VVidOnvifCPtzPri
{
public:
	VVidOnvifCPtzPri()
	:pOnvifDevice(NULL), pPtzService(NULL)
	{
	}
	~VVidOnvifCPtzPri()
	{
		if (pPtzService)
			delete pPtzService;
		if (pOnvifDevice)
			delete pOnvifDevice;
	}
public:
	OnvifClientDevice *pOnvifDevice;
	OnvifClientPTZ *pPtzService;
	astring strToken;
};


VVidOnvifCPtz::VVidOnvifCPtz()
:m_pPri(NULL), m_bInit(false)
{

}
VVidOnvifCPtz::~VVidOnvifCPtz()
{
	if (m_pPri)
		delete m_pPri;
}
bool VVidOnvifCPtz::Init(astring strUser, astring strPasswd, astring strUrl)
{
	if (m_bInit == true && m_pPri)
	{
		delete m_pPri;
		m_pPri = NULL;
		m_bInit = false;
	}

	m_pPri = new VVidOnvifCPtzPri();

	/* Init Get PTZ url */
	bool bRet = false;
	m_pPri->pOnvifDevice  = new OnvifClientDevice(strUrl, 
	 					strUser, strPasswd);

	m_pPri->pPtzService = new OnvifClientPTZ(*(m_pPri->pOnvifDevice));

	VVidOnvifProfileMap profileMap;

	if (VVidOnvifC::GetProfiles(strUser, strPasswd, strUrl, profileMap) == true)
	{
		VDC_DEBUG( "%s new Got profiles size %d \n",__FUNCTION__, profileMap.size());
	}else
	{
		VDC_DEBUG( "%s %s Get Stream url failed \n",__FUNCTION__, 
					strUrl.c_str());
		return false;
	}

	if (profileMap.size() > 0)
	{
		m_pPri->strToken = profileMap.begin()->first;
	}else
	{
		return false;
	}

	m_bInit = true;

	return true;
}
bool VVidOnvifCPtz::PtzAction(VVidOnvifCPtzAct act, float speed)
{
	if (m_bInit == false || m_pPri == NULL)
	{
		return false;
	}
	
	switch (act)
	{
		case VVID_PTZ_UP:	
		{
			_tptz__ContinuousMoveResponse resp;
			tt__PTZSpeed oSpeed;
			tt__Vector2D pan;
			pan.x = 0;
			pan.y = speed;
			oSpeed.PanTilt = &pan;
			oSpeed.Zoom = NULL;
			LONG64 Timeout = 100;
		
			m_pPri->pPtzService->ContinuousMove(resp, oSpeed, Timeout, m_pPri->strToken);
			break;
		}
		case VVID_PTZ_DOWN:	
		{
			_tptz__ContinuousMoveResponse resp;
			tt__PTZSpeed oSpeed;
			tt__Vector2D pan;
			pan.x = 0;
			pan.y = 0 - speed;
			oSpeed.PanTilt = &pan;
			oSpeed.Zoom = NULL;
			LONG64 Timeout = 100;
		
			m_pPri->pPtzService->ContinuousMove(resp, oSpeed, Timeout, m_pPri->strToken);
			break;
		}
		case VVID_PTZ_LEFT:	
		{
			_tptz__ContinuousMoveResponse resp;
			tt__PTZSpeed oSpeed;
			tt__Vector2D pan;
			pan.x = 0 - speed;
			pan.y = 0;
			oSpeed.PanTilt = &pan;
			oSpeed.Zoom = NULL;
			LONG64 Timeout = 100;
		
			m_pPri->pPtzService->ContinuousMove(resp, oSpeed, Timeout, m_pPri->strToken);
			break;
		}
		case VVID_PTZ_RIGHT:	
		{
			_tptz__ContinuousMoveResponse resp;
			tt__PTZSpeed oSpeed;
			tt__Vector2D pan;
			pan.x = speed;
			pan.y = 0;
			oSpeed.PanTilt = &pan;
			oSpeed.Zoom = NULL;
			LONG64 Timeout = 100;
		
			m_pPri->pPtzService->ContinuousMove(resp, oSpeed, Timeout, m_pPri->strToken);
			break;
		}
		case VVID_PTZ_ZOOM_IN:	
		{
			_tptz__ContinuousMoveResponse resp;
			tt__PTZSpeed oSpeed;
			tt__Vector2D pan;
			tt__Vector1D zoom;
			zoom.x = speed;
			pan.x = 0;
			pan.y = 0;
			oSpeed.PanTilt = &pan;
			oSpeed.Zoom = &zoom;
			LONG64 Timeout = 100;
		
			m_pPri->pPtzService->ContinuousMove(resp, oSpeed, Timeout, m_pPri->strToken);
			break;
		}
		case VVID_PTZ_ZOOM_OUT:	
		{
			_tptz__ContinuousMoveResponse resp;
			tt__PTZSpeed oSpeed;
			tt__Vector2D pan;
			tt__Vector1D zoom;
			zoom.x = 0 - speed;
			pan.x = 0;
			pan.y = 0;
			oSpeed.PanTilt = &pan;
			oSpeed.Zoom = &zoom;
			LONG64 Timeout = 100;
		
			m_pPri->pPtzService->ContinuousMove(resp, oSpeed, Timeout, m_pPri->strToken);
			break;
		}
		case VVID_PTZ_STOP:	
		{
			_tptz__StopResponse resp;
			m_pPri->pPtzService->Stop(resp, false, false, m_pPri->strToken);
			break;
		}
		default:
			break;
	}

	return true;
}


