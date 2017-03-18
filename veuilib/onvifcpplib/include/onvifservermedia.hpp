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
#ifndef __ONVIF_SERVER_MEDIA__
#define __ONVIF_SERVER_MEDIA__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "soapStub.h"
#include "soapMediaBindingService.h"
#include "wsseapi.h"


using namespace std;

class OnvifServerMedia : public MediaBindingService
{
public:
    OnvifServerMedia(int nStreamId);
    ~OnvifServerMedia();
public:
	/// Create a copy
	virtual	MediaBindingService *copy()
	{
		return NULL;
	}
	///
	/// Service operations (you should define these):
	/// Note: compile with -DWITH_PURE_VIRTUAL for pure virtual methods
	///

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_trt__GetServiceCapabilities *trt__GetServiceCapabilities, _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) 
	{
		return SOAP_OK;
	}

	/// Web service operation 'GetVideoSources' (returns error code or SOAP_OK)
	virtual	int GetVideoSources(_trt__GetVideoSources *trt__GetVideoSources, _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) 
	{
		 string m_strLoalAddr = inet_ntoa(m_pSoap->local.sin_addr);
		static tt__VideoResolution Resolution;
		Resolution.Width = 1920;
		Resolution.Height = 1080;
		tt__VideoSource* VideoSource = soap_new_tt__VideoSource(m_pSoap, 1);
		VideoSource->Framerate = 60;
		VideoSource->token= "1";
		VideoSource->Resolution = &Resolution;
		VideoSource->Imaging = NULL;
		VideoSource->Extension = NULL;
		trt__GetVideoSourcesResponse->VideoSources.push_back(VideoSource);

		return SOAP_OK;
	}

	/// Web service operation 'GetAudioSources' (returns error code or SOAP_OK)
	virtual	int GetAudioSources(_trt__GetAudioSources *trt__GetAudioSources, _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputs' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputs(_trt__GetAudioOutputs *trt__GetAudioOutputs, _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {return SOAP_OK;}

	/// Web service operation 'CreateProfile' (returns error code or SOAP_OK)
	virtual	int CreateProfile(_trt__CreateProfile *trt__CreateProfile, _trt__CreateProfileResponse *trt__CreateProfileResponse) {return SOAP_OK;}

	/// Web service operation 'GetProfile' (returns error code or SOAP_OK)
	virtual	int GetProfile(_trt__GetProfile *trt__GetProfile, _trt__GetProfileResponse *trt__GetProfileResponse) 
	{
		string m_strLoalAddr = inet_ntoa(m_pSoap->local.sin_addr);
		static tt__VideoResolution Resolution;
		Resolution.Width = 1920;
		Resolution.Height = 1080;
		static tt__H264Configuration H264;
		H264.H264Profile = tt__H264Profile__Baseline;
		tt__Profile* Profile = soap_new_tt__Profile(m_pSoap, 1);
		
		tt__VideoSourceConfiguration *VideoSourceConf = 
							soap_new_tt__VideoSourceConfiguration(m_pSoap, 1);
		VideoSourceConf->SourceToken = "1";
		VideoSourceConf->Name = "1";
		VideoSourceConf->token = "1";
		VideoSourceConf->UseCount = 10;
		tt__VideoEncoderConfiguration *VideoEncoderConf = 
							soap_new_tt__VideoEncoderConfiguration(m_pSoap, 1);
		VideoEncoderConf->Encoding = tt__VideoEncoding__H264;
		VideoEncoderConf->Name = "1";
		VideoEncoderConf->token = "1";
		VideoEncoderConf->Quality = 1.0;
		VideoEncoderConf->H264 = &H264;
		VideoEncoderConf->Resolution = &Resolution;
		VideoEncoderConf->UseCount = 10;
		VideoEncoderConf->Multicast = NULL;
		VideoEncoderConf->SessionTimeout = 60;
		Profile->token = "1";
		Profile->Name = "1";
		Profile->VideoSourceConfiguration = VideoSourceConf;
		Profile->VideoEncoderConfiguration = VideoEncoderConf;

		trt__GetProfileResponse->Profile = Profile;
		return SOAP_OK;
	}

	/// Web service operation 'GetProfiles' (returns error code or SOAP_OK)
	virtual	int GetProfiles(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse *trt__GetProfilesResponse) 
	{
		string m_strLoalAddr = inet_ntoa(m_pSoap->local.sin_addr);
		static tt__VideoResolution Resolution;
		Resolution.Width = 1920;
		Resolution.Height = 1080;
		static tt__H264Configuration H264;
		H264.H264Profile = tt__H264Profile__Baseline;
		tt__Profile* Profile = soap_new_tt__Profile(m_pSoap, 1);
		tt__VideoSourceConfiguration *VideoSourceConf = 
							soap_new_tt__VideoSourceConfiguration(m_pSoap, 1);
		VideoSourceConf->SourceToken = "1";
		VideoSourceConf->Name = "1";
		VideoSourceConf->token = "1";
		VideoSourceConf->UseCount = 10;
		tt__VideoEncoderConfiguration *VideoEncoderConf = 
							soap_new_tt__VideoEncoderConfiguration(m_pSoap, 1);
		VideoEncoderConf->Encoding = tt__VideoEncoding__H264;
		VideoEncoderConf->Name = "1";
		VideoEncoderConf->token = "1";
		VideoEncoderConf->Quality = 1.0;
		VideoEncoderConf->H264 = &H264;
		VideoEncoderConf->Resolution = &Resolution;
		VideoEncoderConf->UseCount = 10;
		VideoEncoderConf->Multicast = NULL;
		VideoEncoderConf->SessionTimeout = 60;
		Profile->token = "1";
		Profile->Name = "1";
		Profile->VideoSourceConfiguration = VideoSourceConf;
		Profile->VideoEncoderConfiguration = VideoEncoderConf;

		trt__GetProfilesResponse->Profiles.push_back(Profile);
		return SOAP_OK;
	}

	/// Web service operation 'AddVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int AddVideoEncoderConfiguration(_trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int AddVideoSourceConfiguration(_trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioEncoderConfiguration(_trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioSourceConfiguration(_trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddPTZConfiguration' (returns error code or SOAP_OK)
	virtual	int AddPTZConfiguration(_trt__AddPTZConfiguration *trt__AddPTZConfiguration, _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int AddVideoAnalyticsConfiguration(_trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int AddMetadataConfiguration(_trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioOutputConfiguration(_trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioDecoderConfiguration(_trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveVideoEncoderConfiguration(_trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveVideoSourceConfiguration(_trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioEncoderConfiguration(_trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioSourceConfiguration(_trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemovePTZConfiguration' (returns error code or SOAP_OK)
	virtual	int RemovePTZConfiguration(_trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveVideoAnalyticsConfiguration(_trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveMetadataConfiguration(_trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioOutputConfiguration(_trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioDecoderConfiguration(_trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'DeleteProfile' (returns error code or SOAP_OK)
	virtual	int DeleteProfile(_trt__DeleteProfile *trt__DeleteProfile, _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceConfigurations(_trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetVideoEncoderConfigurations(_trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioSourceConfigurations(_trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioEncoderConfigurations(_trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoAnalyticsConfigurations' (returns error code or SOAP_OK)
	virtual	int GetVideoAnalyticsConfigurations(_trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetMetadataConfigurations' (returns error code or SOAP_OK)
	virtual	int GetMetadataConfigurations(_trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputConfigurations(_trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioDecoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioDecoderConfigurations(_trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceConfiguration(_trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int GetVideoEncoderConfiguration(_trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioSourceConfiguration(_trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioEncoderConfiguration(_trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int GetVideoAnalyticsConfiguration(_trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int GetMetadataConfiguration(_trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputConfiguration(_trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioDecoderConfiguration(_trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleVideoEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleVideoEncoderConfigurations(_trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleVideoSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleVideoSourceConfigurations(_trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioEncoderConfigurations(_trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioSourceConfigurations(_trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleVideoAnalyticsConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleVideoAnalyticsConfigurations(_trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleMetadataConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleMetadataConfigurations(_trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioOutputConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioOutputConfigurations(_trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioDecoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioDecoderConfigurations(_trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int SetVideoSourceConfiguration(_trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int SetVideoEncoderConfiguration(_trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioSourceConfiguration(_trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioEncoderConfiguration(_trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int SetVideoAnalyticsConfiguration(_trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int SetMetadataConfiguration(_trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioOutputConfiguration(_trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioDecoderConfiguration(_trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceConfigurationOptions(_trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoEncoderConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetVideoEncoderConfigurationOptions(_trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSourceConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioSourceConfigurationOptions(_trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioEncoderConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioEncoderConfigurationOptions(_trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetMetadataConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetMetadataConfigurationOptions(_trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputConfigurationOptions(_trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioDecoderConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioDecoderConfigurationOptions(_trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetGuaranteedNumberOfVideoEncoderInstances' (returns error code or SOAP_OK)
	virtual	int GetGuaranteedNumberOfVideoEncoderInstances(_trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {return SOAP_OK;}

	/// Web service operation 'GetStreamUri' (returns error code or SOAP_OK)
	virtual	int GetStreamUri(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse *trt__GetStreamUriResponse) 
	{
		string m_strLoalAddr = inet_ntoa(m_pSoap->local.sin_addr);
		tt__MediaUri* Uri = soap_new_tt__MediaUri(m_pSoap, 1);
		char buff[1024];
		sprintf(buff, "%d", m_nStreamId);
		string strStreamId = buff;
		Uri->Uri = "rtsp://" + m_strLoalAddr + ":9554" + "/liveview" + strStreamId;
		Uri->InvalidAfterConnect = false;
		Uri->InvalidAfterReboot = false;
		Uri->Timeout = 60;
		printf("%s  %d Get Uri\n", __FILE__, __LINE__);
		trt__GetStreamUriResponse->MediaUri = Uri;
		
		return SOAP_OK;
	}

	/// Web service operation 'StartMulticastStreaming' (returns error code or SOAP_OK)
	virtual	int StartMulticastStreaming(_trt__StartMulticastStreaming *trt__StartMulticastStreaming, _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse) {return SOAP_OK;}

	/// Web service operation 'StopMulticastStreaming' (returns error code or SOAP_OK)
	virtual	int StopMulticastStreaming(_trt__StopMulticastStreaming *trt__StopMulticastStreaming, _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {return SOAP_OK;}

	/// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
	virtual	int SetSynchronizationPoint(_trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {return SOAP_OK;}

	/// Web service operation 'GetSnapshotUri' (returns error code or SOAP_OK)
	virtual	int GetSnapshotUri(_trt__GetSnapshotUri *trt__GetSnapshotUri, _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceModes' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceModes(_trt__GetVideoSourceModes *trt__GetVideoSourceModes, _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoSourceMode' (returns error code or SOAP_OK)
	virtual	int SetVideoSourceMode(_trt__SetVideoSourceMode *trt__SetVideoSourceMode, _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse) {return SOAP_OK;}

	/// Web service operation 'GetOSDs' (returns error code or SOAP_OK)
	virtual	int GetOSDs(_trt__GetOSDs *trt__GetOSDs, _trt__GetOSDsResponse *trt__GetOSDsResponse) {return SOAP_OK;}

	/// Web service operation 'GetOSD' (returns error code or SOAP_OK)
	virtual	int GetOSD(_trt__GetOSD *trt__GetOSD, _trt__GetOSDResponse *trt__GetOSDResponse) {return SOAP_OK;}

	/// Web service operation 'GetOSDOptions' (returns error code or SOAP_OK)
	virtual	int GetOSDOptions(_trt__GetOSDOptions *trt__GetOSDOptions, _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'SetOSD' (returns error code or SOAP_OK)
	virtual	int SetOSD(_trt__SetOSD *trt__SetOSD, _trt__SetOSDResponse *trt__SetOSDResponse) {return SOAP_OK;}

	/// Web service operation 'CreateOSD' (returns error code or SOAP_OK)
	virtual	int CreateOSD(_trt__CreateOSD *trt__CreateOSD, _trt__CreateOSDResponse *trt__CreateOSDResponse) {return SOAP_OK;}

	/// Web service operation 'DeleteOSD' (returns error code or SOAP_OK)
	virtual	int DeleteOSD(_trt__DeleteOSD *trt__DeleteOSD, _trt__DeleteOSDResponse *trt__DeleteOSDResponse) {return SOAP_OK;}

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities_(_trt__GetServiceCapabilities *trt__GetServiceCapabilities, _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSources' (returns error code or SOAP_OK)
	virtual	int GetVideoSources_(_trt__GetVideoSources *trt__GetVideoSources, _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSources' (returns error code or SOAP_OK)
	virtual	int GetAudioSources_(_trt__GetAudioSources *trt__GetAudioSources, _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputs' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputs_(_trt__GetAudioOutputs *trt__GetAudioOutputs, _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {return SOAP_OK;}

	/// Web service operation 'CreateProfile' (returns error code or SOAP_OK)
	virtual	int CreateProfile_(_trt__CreateProfile *trt__CreateProfile, _trt__CreateProfileResponse *trt__CreateProfileResponse) {return SOAP_OK;}

	/// Web service operation 'GetProfile' (returns error code or SOAP_OK)
	virtual	int GetProfile_(_trt__GetProfile *trt__GetProfile, _trt__GetProfileResponse *trt__GetProfileResponse) {return SOAP_OK;}

	/// Web service operation 'GetProfiles' (returns error code or SOAP_OK)
	virtual	int GetProfiles_(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse *trt__GetProfilesResponse) {return SOAP_OK;}

	/// Web service operation 'AddVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int AddVideoEncoderConfiguration_(_trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int AddVideoSourceConfiguration_(_trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioEncoderConfiguration_(_trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioSourceConfiguration_(_trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddPTZConfiguration' (returns error code or SOAP_OK)
	virtual	int AddPTZConfiguration_(_trt__AddPTZConfiguration *trt__AddPTZConfiguration, _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int AddVideoAnalyticsConfiguration_(_trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int AddMetadataConfiguration_(_trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioOutputConfiguration_(_trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'AddAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int AddAudioDecoderConfiguration_(_trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveVideoEncoderConfiguration_(_trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveVideoSourceConfiguration_(_trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioEncoderConfiguration_(_trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioSourceConfiguration_(_trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemovePTZConfiguration' (returns error code or SOAP_OK)
	virtual	int RemovePTZConfiguration_(_trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveVideoAnalyticsConfiguration_(_trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveMetadataConfiguration_(_trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioOutputConfiguration_(_trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'RemoveAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int RemoveAudioDecoderConfiguration_(_trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'DeleteProfile' (returns error code or SOAP_OK)
	virtual	int DeleteProfile_(_trt__DeleteProfile *trt__DeleteProfile, _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceConfigurations_(_trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetVideoEncoderConfigurations_(_trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioSourceConfigurations_(_trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioEncoderConfigurations_(_trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoAnalyticsConfigurations' (returns error code or SOAP_OK)
	virtual	int GetVideoAnalyticsConfigurations_(_trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetMetadataConfigurations' (returns error code or SOAP_OK)
	virtual	int GetMetadataConfigurations_(_trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputConfigurations_(_trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioDecoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetAudioDecoderConfigurations_(_trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceConfiguration_(_trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int GetVideoEncoderConfiguration_(_trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioSourceConfiguration_(_trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioEncoderConfiguration_(_trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int GetVideoAnalyticsConfiguration_(_trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int GetMetadataConfiguration_(_trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputConfiguration_(_trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int GetAudioDecoderConfiguration_(_trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleVideoEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleVideoEncoderConfigurations_(_trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleVideoSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleVideoSourceConfigurations_(_trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioEncoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioEncoderConfigurations_(_trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioSourceConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioSourceConfigurations_(_trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleVideoAnalyticsConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleVideoAnalyticsConfigurations_(_trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleMetadataConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleMetadataConfigurations_(_trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioOutputConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioOutputConfigurations_(_trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'GetCompatibleAudioDecoderConfigurations' (returns error code or SOAP_OK)
	virtual	int GetCompatibleAudioDecoderConfigurations_(_trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int SetVideoSourceConfiguration_(_trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int SetVideoEncoderConfiguration_(_trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioSourceConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioSourceConfiguration_(_trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioEncoderConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioEncoderConfiguration_(_trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoAnalyticsConfiguration' (returns error code or SOAP_OK)
	virtual	int SetVideoAnalyticsConfiguration_(_trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetMetadataConfiguration' (returns error code or SOAP_OK)
	virtual	int SetMetadataConfiguration_(_trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioOutputConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioOutputConfiguration_(_trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'SetAudioDecoderConfiguration' (returns error code or SOAP_OK)
	virtual	int SetAudioDecoderConfiguration_(_trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceConfigurationOptions_(_trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoEncoderConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetVideoEncoderConfigurationOptions_(_trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioSourceConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioSourceConfigurationOptions_(_trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioEncoderConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioEncoderConfigurationOptions_(_trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetMetadataConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetMetadataConfigurationOptions_(_trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioOutputConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioOutputConfigurationOptions_(_trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetAudioDecoderConfigurationOptions' (returns error code or SOAP_OK)
	virtual	int GetAudioDecoderConfigurationOptions_(_trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'GetGuaranteedNumberOfVideoEncoderInstances' (returns error code or SOAP_OK)
	virtual	int GetGuaranteedNumberOfVideoEncoderInstances_(_trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {return SOAP_OK;}

	/// Web service operation 'GetStreamUri' (returns error code or SOAP_OK)
	virtual	int GetStreamUri_(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {return SOAP_OK;}

	/// Web service operation 'StartMulticastStreaming' (returns error code or SOAP_OK)
	virtual	int StartMulticastStreaming_(_trt__StartMulticastStreaming *trt__StartMulticastStreaming, _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse) {return SOAP_OK;}

	/// Web service operation 'StopMulticastStreaming' (returns error code or SOAP_OK)
	virtual	int StopMulticastStreaming_(_trt__StopMulticastStreaming *trt__StopMulticastStreaming, _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {return SOAP_OK;}

	/// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
	virtual	int SetSynchronizationPoint_(_trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {return SOAP_OK;}

	/// Web service operation 'GetSnapshotUri' (returns error code or SOAP_OK)
	virtual	int GetSnapshotUri_(_trt__GetSnapshotUri *trt__GetSnapshotUri, _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {return SOAP_OK;}

	/// Web service operation 'GetVideoSourceModes' (returns error code or SOAP_OK)
	virtual	int GetVideoSourceModes_(_trt__GetVideoSourceModes *trt__GetVideoSourceModes, _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse) {return SOAP_OK;}

	/// Web service operation 'SetVideoSourceMode' (returns error code or SOAP_OK)
	virtual	int SetVideoSourceMode_(_trt__SetVideoSourceMode *trt__SetVideoSourceMode, _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse) {return SOAP_OK;}

	/// Web service operation 'GetOSDs' (returns error code or SOAP_OK)
	virtual	int GetOSDs_(_trt__GetOSDs *trt__GetOSDs, _trt__GetOSDsResponse *trt__GetOSDsResponse) {return SOAP_OK;}

	/// Web service operation 'GetOSD' (returns error code or SOAP_OK)
	virtual	int GetOSD_(_trt__GetOSD *trt__GetOSD, _trt__GetOSDResponse *trt__GetOSDResponse) {return SOAP_OK;}

	/// Web service operation 'GetOSDOptions' (returns error code or SOAP_OK)
	virtual	int GetOSDOptions_(_trt__GetOSDOptions *trt__GetOSDOptions, _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse) {return SOAP_OK;}

	/// Web service operation 'SetOSD' (returns error code or SOAP_OK)
	virtual	int SetOSD_(_trt__SetOSD *trt__SetOSD, _trt__SetOSDResponse *trt__SetOSDResponse) {return SOAP_OK;}

	/// Web service operation 'CreateOSD' (returns error code or SOAP_OK)
	virtual	int CreateOSD_(_trt__CreateOSD *trt__CreateOSD, _trt__CreateOSDResponse *trt__CreateOSDResponse) {return SOAP_OK;}

	/// Web service operation 'DeleteOSD' (returns error code or SOAP_OK)
	virtual	int DeleteOSD_(_trt__DeleteOSD *trt__DeleteOSD, _trt__DeleteOSDResponse *trt__DeleteOSDResponse) {return SOAP_OK;}
private:
	struct soap * m_pSoap;
	int m_nStreamId;
};

inline OnvifServerMedia::OnvifServerMedia(int nStreamId)
: MediaBindingService(), m_pSoap(this), m_nStreamId(nStreamId)
{
     
}

inline OnvifServerMedia::~OnvifServerMedia()
{

}

#endif 
