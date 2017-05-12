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
#ifndef __ONVIF_SERVER_DEVICE__
#define __ONVIF_SERVER_DEVICE__

#include <string>
#include <map>
#include <iostream>
#include <list>

#include "soapStub.h"
#include "soapH.h"
#include "soapDeviceBindingService.h"
#include "wsseapi.h"

using namespace std;

class OnvifServerDevice : public DeviceBindingService
{
public:
    OnvifServerDevice(string strDeviceEndpoint, 
		string strMediaEndpoint, string strReceiverEndpoint, 
		string strEventEndpoint);
    ~OnvifServerDevice();
public:
	virtual	DeviceBindingService *copy()
	{
		return NULL;
	}
	/// Web service operation 'GetServices' (returns error code or SOAP_OK)
	virtual	int GetServices(_tds__GetServices *tds__GetServices, _tds__GetServicesResponse *tds__GetServicesResponse) 
	{

		{
			tt__OnvifVersion *version = soap_new_tt__OnvifVersion(m_pSoap, 1);
			version->Major = 2;
			version->Minor = 4;
			tds__Service *service = soap_new_tds__Service(m_pSoap, 1);
			service->XAddr = m_strDeviceEndpoint;
			service->Namespace = "http://www.onvif.org/ver10/device/wsdl";
			service->Version = version;
			tds__GetServicesResponse->Service.push_back(service);
		}
		{
			tt__OnvifVersion *version = soap_new_tt__OnvifVersion(m_pSoap, 1);
			version->Major = 2;
			version->Minor = 4;
			tds__Service *service = soap_new_tds__Service(m_pSoap, 1);
			service->XAddr = m_strEventEndpoint;
			service->Namespace = "http://www.onvif.org/ver10/media/wsdl";
			service->Version = version;
			tds__GetServicesResponse->Service.push_back(service);
		}
	
		{
			tt__OnvifVersion *version = soap_new_tt__OnvifVersion(m_pSoap, 1);
			version->Major = 2;
			version->Minor = 4;
			tds__Service *service = soap_new_tds__Service(m_pSoap, 1);
			service->XAddr = m_strReceiverEndpoint;
			service->Namespace = "http://www.onvif.org/ver10/receiver/wsdl";
			service->Version = version;
			tds__GetServicesResponse->Service.push_back(service);
		}
		{
			tt__OnvifVersion *version = soap_new_tt__OnvifVersion(m_pSoap, 1);
			version->Major = 2;
			version->Minor = 4;
			tds__Service *service = soap_new_tds__Service(m_pSoap, 1);
			service->XAddr = m_strEventEndpoint;
			service->Namespace = "http://www.onvif.org/ver10/events/wsdl";
			service->Version = version;
			tds__GetServicesResponse->Service.push_back(service);
		}

		return SOAP_OK;
	};

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities(_tds__GetServiceCapabilities *tds__GetServiceCapabilities, _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse) 
	{
		return SOAP_OK;
	};

	/// Web service operation 'GetDeviceInformation' (returns error code or SOAP_OK)
	virtual	int GetDeviceInformation(_tds__GetDeviceInformation *tds__GetDeviceInformation, _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse) 
	{
		tds__GetDeviceInformationResponse->Manufacturer    = "OpenCVR";
		tds__GetDeviceInformationResponse->Model           = "OpenCVR";
		tds__GetDeviceInformationResponse->FirmwareVersion = "OpenCVR";
		tds__GetDeviceInformationResponse->SerialNumber    = "00:00:00:00:00:01";
		tds__GetDeviceInformationResponse->HardwareId      = "001200123";

		return SOAP_OK;
	};

	/// Web service operation 'SetSystemDateAndTime' (returns error code or SOAP_OK)
	virtual	int SetSystemDateAndTime(_tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse) 
	{
	
		printf("SetSystemDateAndTime=========\n");
		return SOAP_OK;
	}

	/// Web service operation 'GetSystemDateAndTime' (returns error code or SOAP_OK)
	virtual	int GetSystemDateAndTime(_tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
	{
		time_t t = time(NULL);
		struct tm * now = localtime( &t );	
		printf("GetSystemDateAndTime=========\n");
		
		tt__SystemDateTime* SystemDateAndTime = soap_new_tt__SystemDateTime(m_pSoap, 1);
		SystemDateAndTime->DateTimeType = tt__SetDateTimeType__Manual;
		//SystemDateAndTime->DaylightSavings
		tt__DateTime* UTCDateTime = soap_new_tt__DateTime(m_pSoap, 1);  //new tt__DateTime;
		tt__Date* UTCDate = soap_new_tt__Date(m_pSoap, 1);
		UTCDateTime->Date = UTCDate;
		tt__Time* UTCTime = soap_new_tt__Time(m_pSoap, 1);
		UTCDateTime->Time = UTCTime;
		SystemDateAndTime->UTCDateTime = UTCDateTime;
		
		SystemDateAndTime->UTCDateTime->Date->Year = now->tm_year + 1900;
		SystemDateAndTime->UTCDateTime->Date->Month = now->tm_mon + 1;
		SystemDateAndTime->UTCDateTime->Date->Day = now->tm_mday;

		SystemDateAndTime->UTCDateTime->Time->Hour = now->tm_hour;
		SystemDateAndTime->UTCDateTime->Time->Minute = now->tm_min;
		SystemDateAndTime->UTCDateTime->Time->Second = now->tm_sec;

		tds__GetSystemDateAndTimeResponse->SystemDateAndTime = SystemDateAndTime;
		
		return SOAP_OK;
	}

	/// Web service operation 'SetSystemFactoryDefault' (returns error code or SOAP_OK)
	virtual	int SetSystemFactoryDefault(_tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse) {return SOAP_OK;};

	/// Web service operation 'UpgradeSystemFirmware' (returns error code or SOAP_OK)
	virtual	int UpgradeSystemFirmware(_tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse) {return SOAP_OK;};

	/// Web service operation 'SystemReboot' (returns error code or SOAP_OK)
	virtual	int SystemReboot(_tds__SystemReboot *tds__SystemReboot, _tds__SystemRebootResponse *tds__SystemRebootResponse) {return SOAP_OK;};

	/// Web service operation 'RestoreSystem' (returns error code or SOAP_OK)
	virtual	int RestoreSystem(_tds__RestoreSystem *tds__RestoreSystem, _tds__RestoreSystemResponse *tds__RestoreSystemResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemBackup' (returns error code or SOAP_OK)
	virtual	int GetSystemBackup(_tds__GetSystemBackup *tds__GetSystemBackup, _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemLog' (returns error code or SOAP_OK)
	virtual	int GetSystemLog(_tds__GetSystemLog *tds__GetSystemLog, _tds__GetSystemLogResponse *tds__GetSystemLogResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemSupportInformation' (returns error code or SOAP_OK)
	virtual	int GetSystemSupportInformation(_tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse) {return SOAP_OK;};

	/// Web service operation 'GetScopes' (returns error code or SOAP_OK)
	virtual	int GetScopes(_tds__GetScopes *tds__GetScopes, _tds__GetScopesResponse *tds__GetScopesResponse) 
	{

		for( std::vector<std::string>::const_iterator it = m_strScopes.begin();
		 it != m_strScopes.end(); ++it ) 
		 {
#if 1
				tds__GetScopesResponse->Scopes.push_back(soap_new_tt__Scope(m_pSoap, 1));
#endif
				tds__GetScopesResponse->Scopes.back()->ScopeDef = tt__ScopeDefinition__Fixed;
				tds__GetScopesResponse->Scopes.back()->ScopeItem = *it;
		}
		return SOAP_OK;
	};

	/// Web service operation 'SetScopes' (returns error code or SOAP_OK)
	virtual	int SetScopes(_tds__SetScopes *tds__SetScopes, _tds__SetScopesResponse *tds__SetScopesResponse) {return SOAP_OK;};

	/// Web service operation 'AddScopes' (returns error code or SOAP_OK)
	virtual	int AddScopes(_tds__AddScopes *tds__AddScopes, _tds__AddScopesResponse *tds__AddScopesResponse) {return SOAP_OK;};

	/// Web service operation 'RemoveScopes' (returns error code or SOAP_OK)
	virtual	int RemoveScopes(_tds__RemoveScopes *tds__RemoveScopes, _tds__RemoveScopesResponse *tds__RemoveScopesResponse) {return SOAP_OK;};

	/// Web service operation 'GetDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int GetDiscoveryMode(_tds__GetDiscoveryMode *tds__GetDiscoveryMode, _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'SetDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int SetDiscoveryMode(_tds__SetDiscoveryMode *tds__SetDiscoveryMode, _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetRemoteDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int GetRemoteDiscoveryMode(_tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'SetRemoteDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int SetRemoteDiscoveryMode(_tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetDPAddresses' (returns error code or SOAP_OK)
	virtual	int GetDPAddresses(_tds__GetDPAddresses *tds__GetDPAddresses, _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse) {return SOAP_OK;};

	/// Web service operation 'GetEndpointReference' (returns error code or SOAP_OK)
	virtual	int GetEndpointReference(_tds__GetEndpointReference *tds__GetEndpointReference, _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse) {return SOAP_OK;};

	/// Web service operation 'GetRemoteUser' (returns error code or SOAP_OK)
	virtual	int GetRemoteUser(_tds__GetRemoteUser *tds__GetRemoteUser, _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse) {return SOAP_OK;};

	/// Web service operation 'SetRemoteUser' (returns error code or SOAP_OK)
	virtual	int SetRemoteUser(_tds__SetRemoteUser *tds__SetRemoteUser, _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse) {return SOAP_OK;};

	/// Web service operation 'GetUsers' (returns error code or SOAP_OK)
	virtual	int GetUsers(_tds__GetUsers *tds__GetUsers, _tds__GetUsersResponse *tds__GetUsersResponse) {return SOAP_OK;};

	/// Web service operation 'CreateUsers' (returns error code or SOAP_OK)
	virtual	int CreateUsers(_tds__CreateUsers *tds__CreateUsers, _tds__CreateUsersResponse *tds__CreateUsersResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteUsers' (returns error code or SOAP_OK)
	virtual	int DeleteUsers(_tds__DeleteUsers *tds__DeleteUsers, _tds__DeleteUsersResponse *tds__DeleteUsersResponse) {return SOAP_OK;};

	/// Web service operation 'SetUser' (returns error code or SOAP_OK)
	virtual	int SetUser(_tds__SetUser *tds__SetUser, _tds__SetUserResponse *tds__SetUserResponse) {return SOAP_OK;};

	/// Web service operation 'GetWsdlUrl' (returns error code or SOAP_OK)
	virtual	int GetWsdlUrl(_tds__GetWsdlUrl *tds__GetWsdlUrl, _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse) {return SOAP_OK;};

	/// Web service operation 'GetCapabilities' (returns error code or SOAP_OK)
	virtual	int GetCapabilities(_tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse) 
	{
		static bool RTPMulticast_ = false;
		static bool RTP_USCORETCP_ = true;
		static bool RTP_USCORERTSP_USCORETCP_ = true;
		tt__Capabilities *Capabilities = soap_new_tt__Capabilities(m_pSoap, 1);
		tt__CapabilitiesExtension *Extension = soap_new_tt__CapabilitiesExtension(m_pSoap, 1); 
		tt__ReceiverCapabilities *Receiver = soap_new_tt__ReceiverCapabilities(m_pSoap, 1);
		tt__DeviceCapabilities *Device = soap_new_tt__DeviceCapabilities(m_pSoap, 1);
		tt__EventCapabilities *Events = soap_new_tt__EventCapabilities(m_pSoap, 1);
		tt__MediaCapabilities *Media = soap_new_tt__MediaCapabilities(m_pSoap, 1);
		tt__RealTimeStreamingCapabilities *StreamingCap = 
								soap_new_tt__RealTimeStreamingCapabilities(m_pSoap, 1);
		StreamingCap->RTPMulticast = &RTPMulticast_;
		StreamingCap->RTP_USCORETCP = &RTP_USCORETCP_;
		StreamingCap->RTP_USCORERTSP_USCORETCP = &RTP_USCORERTSP_USCORETCP_;
		Events->XAddr = m_strEventEndpoint;
		Events->WSSubscriptionPolicySupport = false;
		Events->WSPausableSubscriptionManagerInterfaceSupport = false;
		Events->WSPullPointSupport = false;
		Receiver->XAddr = m_strReceiverEndpoint;
		Media->XAddr = m_strMediaEndpoint;
		Media->StreamingCapabilities = StreamingCap;
		
		Capabilities->Extension = Extension;
		Capabilities->Device = Device;
		Capabilities->Events = Events;
		Capabilities->Media = Media;
		Extension->Receiver = Receiver;
		tds__GetCapabilitiesResponse->Capabilities = Capabilities;
		
		
		return SOAP_OK;
	};

	/// Web service operation 'SetDPAddresses' (returns error code or SOAP_OK)
	virtual	int SetDPAddresses(_tds__SetDPAddresses *tds__SetDPAddresses, _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse) {return SOAP_OK;};

	/// Web service operation 'GetHostname' (returns error code or SOAP_OK)
	virtual	int GetHostname(_tds__GetHostname *tds__GetHostname, _tds__GetHostnameResponse *tds__GetHostnameResponse) {return SOAP_OK;};

	/// Web service operation 'SetHostname' (returns error code or SOAP_OK)
	virtual	int SetHostname(_tds__SetHostname *tds__SetHostname, _tds__SetHostnameResponse *tds__SetHostnameResponse) {return SOAP_OK;};

	/// Web service operation 'SetHostnameFromDHCP' (returns error code or SOAP_OK)
	virtual	int SetHostnameFromDHCP(_tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse) {return SOAP_OK;};

	/// Web service operation 'GetDNS' (returns error code or SOAP_OK)
	virtual	int GetDNS(_tds__GetDNS *tds__GetDNS, _tds__GetDNSResponse *tds__GetDNSResponse) {return SOAP_OK;};

	/// Web service operation 'SetDNS' (returns error code or SOAP_OK)
	virtual	int SetDNS(_tds__SetDNS *tds__SetDNS, _tds__SetDNSResponse *tds__SetDNSResponse) {return SOAP_OK;};

	/// Web service operation 'GetNTP' (returns error code or SOAP_OK)
	virtual	int GetNTP(_tds__GetNTP *tds__GetNTP, _tds__GetNTPResponse *tds__GetNTPResponse) {return SOAP_OK;};

	/// Web service operation 'SetNTP' (returns error code or SOAP_OK)
	virtual	int SetNTP(_tds__SetNTP *tds__SetNTP, _tds__SetNTPResponse *tds__SetNTPResponse) {return SOAP_OK;};

	/// Web service operation 'GetDynamicDNS' (returns error code or SOAP_OK)
	virtual	int GetDynamicDNS(_tds__GetDynamicDNS *tds__GetDynamicDNS, _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse) {return SOAP_OK;};

	/// Web service operation 'SetDynamicDNS' (returns error code or SOAP_OK)
	virtual	int SetDynamicDNS(_tds__SetDynamicDNS *tds__SetDynamicDNS, _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse) {return SOAP_OK;};

	/// Web service operation 'GetNetworkInterfaces' (returns error code or SOAP_OK)
	virtual	int GetNetworkInterfaces(_tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse) {return SOAP_OK;};

	/// Web service operation 'SetNetworkInterfaces' (returns error code or SOAP_OK)
	virtual	int SetNetworkInterfaces(_tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse) {return SOAP_OK;};

	/// Web service operation 'GetNetworkProtocols' (returns error code or SOAP_OK)
	virtual	int GetNetworkProtocols(_tds__GetNetworkProtocols *tds__GetNetworkProtocols, _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse) {return SOAP_OK;};

	/// Web service operation 'SetNetworkProtocols' (returns error code or SOAP_OK)
	virtual	int SetNetworkProtocols(_tds__SetNetworkProtocols *tds__SetNetworkProtocols, _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse) {return SOAP_OK;};

	/// Web service operation 'GetNetworkDefaultGateway' (returns error code or SOAP_OK)
	virtual	int GetNetworkDefaultGateway(_tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse) {return SOAP_OK;};

	/// Web service operation 'SetNetworkDefaultGateway' (returns error code or SOAP_OK)
	virtual	int SetNetworkDefaultGateway(_tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse) {return SOAP_OK;};

	/// Web service operation 'GetZeroConfiguration' (returns error code or SOAP_OK)
	virtual	int GetZeroConfiguration(_tds__GetZeroConfiguration *tds__GetZeroConfiguration, _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'SetZeroConfiguration' (returns error code or SOAP_OK)
	virtual	int SetZeroConfiguration(_tds__SetZeroConfiguration *tds__SetZeroConfiguration, _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int GetIPAddressFilter(_tds__GetIPAddressFilter *tds__GetIPAddressFilter, _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'SetIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int SetIPAddressFilter(_tds__SetIPAddressFilter *tds__SetIPAddressFilter, _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'AddIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int AddIPAddressFilter(_tds__AddIPAddressFilter *tds__AddIPAddressFilter, _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'RemoveIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int RemoveIPAddressFilter(_tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'GetAccessPolicy' (returns error code or SOAP_OK)
	virtual	int GetAccessPolicy(_tds__GetAccessPolicy *tds__GetAccessPolicy, _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse) {return SOAP_OK;};

	/// Web service operation 'SetAccessPolicy' (returns error code or SOAP_OK)
	virtual	int SetAccessPolicy(_tds__SetAccessPolicy *tds__SetAccessPolicy, _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse) {return SOAP_OK;};

	/// Web service operation 'CreateCertificate' (returns error code or SOAP_OK)
	virtual	int CreateCertificate(_tds__CreateCertificate *tds__CreateCertificate, _tds__CreateCertificateResponse *tds__CreateCertificateResponse) {return SOAP_OK;};

	/// Web service operation 'GetCertificates' (returns error code or SOAP_OK)
	virtual	int GetCertificates(_tds__GetCertificates *tds__GetCertificates, _tds__GetCertificatesResponse *tds__GetCertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'GetCertificatesStatus' (returns error code or SOAP_OK)
	virtual	int GetCertificatesStatus(_tds__GetCertificatesStatus *tds__GetCertificatesStatus, _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse) {return SOAP_OK;};

	/// Web service operation 'SetCertificatesStatus' (returns error code or SOAP_OK)
	virtual	int SetCertificatesStatus(_tds__SetCertificatesStatus *tds__SetCertificatesStatus, _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteCertificates' (returns error code or SOAP_OK)
	virtual	int DeleteCertificates(_tds__DeleteCertificates *tds__DeleteCertificates, _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'GetPkcs10Request' (returns error code or SOAP_OK)
	virtual	int GetPkcs10Request(_tds__GetPkcs10Request *tds__GetPkcs10Request, _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse) {return SOAP_OK;};

	/// Web service operation 'LoadCertificates' (returns error code or SOAP_OK)
	virtual	int LoadCertificates(_tds__LoadCertificates *tds__LoadCertificates, _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'GetClientCertificateMode' (returns error code or SOAP_OK)
	virtual	int GetClientCertificateMode(_tds__GetClientCertificateMode *tds__GetClientCertificateMode, _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse) {return SOAP_OK;};

	/// Web service operation 'SetClientCertificateMode' (returns error code or SOAP_OK)
	virtual	int SetClientCertificateMode(_tds__SetClientCertificateMode *tds__SetClientCertificateMode, _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetRelayOutputs' (returns error code or SOAP_OK)
	virtual	int GetRelayOutputs(_tds__GetRelayOutputs *tds__GetRelayOutputs, _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse) {return SOAP_OK;};

	/// Web service operation 'SetRelayOutputSettings' (returns error code or SOAP_OK)
	virtual	int SetRelayOutputSettings(_tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse) {return SOAP_OK;};

	/// Web service operation 'SetRelayOutputState' (returns error code or SOAP_OK)
	virtual	int SetRelayOutputState(_tds__SetRelayOutputState *tds__SetRelayOutputState, _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse) {return SOAP_OK;};

	/// Web service operation 'SendAuxiliaryCommand' (returns error code or SOAP_OK)
	virtual	int SendAuxiliaryCommand(_tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse) {return SOAP_OK;};

	/// Web service operation 'GetCACertificates' (returns error code or SOAP_OK)
	virtual	int GetCACertificates(_tds__GetCACertificates *tds__GetCACertificates, _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'LoadCertificateWithPrivateKey' (returns error code or SOAP_OK)
	virtual	int LoadCertificateWithPrivateKey(_tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse) {return SOAP_OK;};

	/// Web service operation 'GetCertificateInformation' (returns error code or SOAP_OK)
	virtual	int GetCertificateInformation(_tds__GetCertificateInformation *tds__GetCertificateInformation, _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse) {return SOAP_OK;};

	/// Web service operation 'LoadCACertificates' (returns error code or SOAP_OK)
	virtual	int LoadCACertificates(_tds__LoadCACertificates *tds__LoadCACertificates, _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'CreateDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int CreateDot1XConfiguration(_tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'SetDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int SetDot1XConfiguration(_tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int GetDot1XConfiguration(_tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot1XConfigurations' (returns error code or SOAP_OK)
	virtual	int GetDot1XConfigurations(_tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int DeleteDot1XConfiguration(_tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot11Capabilities' (returns error code or SOAP_OK)
	virtual	int GetDot11Capabilities(_tds__GetDot11Capabilities *tds__GetDot11Capabilities, _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot11Status' (returns error code or SOAP_OK)
	virtual	int GetDot11Status(_tds__GetDot11Status *tds__GetDot11Status, _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse) {return SOAP_OK;};

	/// Web service operation 'ScanAvailableDot11Networks' (returns error code or SOAP_OK)
	virtual	int ScanAvailableDot11Networks(_tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemUris' (returns error code or SOAP_OK)
	virtual	int GetSystemUris(_tds__GetSystemUris *tds__GetSystemUris, _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse) {return SOAP_OK;};

	/// Web service operation 'StartFirmwareUpgrade' (returns error code or SOAP_OK)
	virtual	int StartFirmwareUpgrade(_tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse) {return SOAP_OK;};

	/// Web service operation 'StartSystemRestore' (returns error code or SOAP_OK)
	virtual	int StartSystemRestore(_tds__StartSystemRestore *tds__StartSystemRestore, _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse) {return SOAP_OK;};

	/// Web service operation 'GetServices' (returns error code or SOAP_OK)
	virtual	int GetServices_(_tds__GetServices *tds__GetServices, _tds__GetServicesResponse *tds__GetServicesResponse) {return SOAP_OK;};

	/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
	virtual	int GetServiceCapabilities_(_tds__GetServiceCapabilities *tds__GetServiceCapabilities, _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse) {return SOAP_OK;};

	/// Web service operation 'GetDeviceInformation' (returns error code or SOAP_OK)
	virtual	int GetDeviceInformation_(_tds__GetDeviceInformation *tds__GetDeviceInformation, _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse) {return SOAP_OK;};

	/// Web service operation 'SetSystemDateAndTime' (returns error code or SOAP_OK)
	virtual	int SetSystemDateAndTime_(_tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemDateAndTime' (returns error code or SOAP_OK)
	virtual	int GetSystemDateAndTime_(_tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse) 
	{
		printf("%s----%d\n", __FILE__, __LINE__);
		return SOAP_OK;
	};

	/// Web service operation 'SetSystemFactoryDefault' (returns error code or SOAP_OK)
	virtual	int SetSystemFactoryDefault_(_tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse) {return SOAP_OK;};

	/// Web service operation 'UpgradeSystemFirmware' (returns error code or SOAP_OK)
	virtual	int UpgradeSystemFirmware_(_tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse) {return SOAP_OK;};

	/// Web service operation 'SystemReboot' (returns error code or SOAP_OK)
	virtual	int SystemReboot_(_tds__SystemReboot *tds__SystemReboot, _tds__SystemRebootResponse *tds__SystemRebootResponse) {return SOAP_OK;};

	/// Web service operation 'RestoreSystem' (returns error code or SOAP_OK)
	virtual	int RestoreSystem_(_tds__RestoreSystem *tds__RestoreSystem, _tds__RestoreSystemResponse *tds__RestoreSystemResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemBackup' (returns error code or SOAP_OK)
	virtual	int GetSystemBackup_(_tds__GetSystemBackup *tds__GetSystemBackup, _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemLog' (returns error code or SOAP_OK)
	virtual	int GetSystemLog_(_tds__GetSystemLog *tds__GetSystemLog, _tds__GetSystemLogResponse *tds__GetSystemLogResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemSupportInformation' (returns error code or SOAP_OK)
	virtual	int GetSystemSupportInformation_(_tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse) {return SOAP_OK;};

	/// Web service operation 'GetScopes' (returns error code or SOAP_OK)
	virtual	int GetScopes_(_tds__GetScopes *tds__GetScopes, _tds__GetScopesResponse *tds__GetScopesResponse) {return SOAP_OK;};

	/// Web service operation 'SetScopes' (returns error code or SOAP_OK)
	virtual	int SetScopes_(_tds__SetScopes *tds__SetScopes, _tds__SetScopesResponse *tds__SetScopesResponse) {return SOAP_OK;};

	/// Web service operation 'AddScopes' (returns error code or SOAP_OK)
	virtual	int AddScopes_(_tds__AddScopes *tds__AddScopes, _tds__AddScopesResponse *tds__AddScopesResponse) {return SOAP_OK;};

	/// Web service operation 'RemoveScopes' (returns error code or SOAP_OK)
	virtual	int RemoveScopes_(_tds__RemoveScopes *tds__RemoveScopes, _tds__RemoveScopesResponse *tds__RemoveScopesResponse) {return SOAP_OK;};

	/// Web service operation 'GetDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int GetDiscoveryMode_(_tds__GetDiscoveryMode *tds__GetDiscoveryMode, _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'SetDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int SetDiscoveryMode_(_tds__SetDiscoveryMode *tds__SetDiscoveryMode, _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetRemoteDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int GetRemoteDiscoveryMode_(_tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'SetRemoteDiscoveryMode' (returns error code or SOAP_OK)
	virtual	int SetRemoteDiscoveryMode_(_tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetDPAddresses' (returns error code or SOAP_OK)
	virtual	int GetDPAddresses_(_tds__GetDPAddresses *tds__GetDPAddresses, _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse) {return SOAP_OK;};

	/// Web service operation 'GetEndpointReference' (returns error code or SOAP_OK)
	virtual	int GetEndpointReference_(_tds__GetEndpointReference *tds__GetEndpointReference, _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse) {return SOAP_OK;};

	/// Web service operation 'GetRemoteUser' (returns error code or SOAP_OK)
	virtual	int GetRemoteUser_(_tds__GetRemoteUser *tds__GetRemoteUser, _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse) {return SOAP_OK;};

	/// Web service operation 'SetRemoteUser' (returns error code or SOAP_OK)
	virtual	int SetRemoteUser_(_tds__SetRemoteUser *tds__SetRemoteUser, _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse) {return SOAP_OK;};

	/// Web service operation 'GetUsers' (returns error code or SOAP_OK)
	virtual	int GetUsers_(_tds__GetUsers *tds__GetUsers, _tds__GetUsersResponse *tds__GetUsersResponse) {return SOAP_OK;};

	/// Web service operation 'CreateUsers' (returns error code or SOAP_OK)
	virtual	int CreateUsers_(_tds__CreateUsers *tds__CreateUsers, _tds__CreateUsersResponse *tds__CreateUsersResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteUsers' (returns error code or SOAP_OK)
	virtual	int DeleteUsers_(_tds__DeleteUsers *tds__DeleteUsers, _tds__DeleteUsersResponse *tds__DeleteUsersResponse) {return SOAP_OK;};

	/// Web service operation 'SetUser' (returns error code or SOAP_OK)
	virtual	int SetUser_(_tds__SetUser *tds__SetUser, _tds__SetUserResponse *tds__SetUserResponse) {return SOAP_OK;};

	/// Web service operation 'GetWsdlUrl' (returns error code or SOAP_OK)
	virtual	int GetWsdlUrl_(_tds__GetWsdlUrl *tds__GetWsdlUrl, _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse) {return SOAP_OK;};

	/// Web service operation 'GetCapabilities' (returns error code or SOAP_OK)
	virtual	int GetCapabilities_(_tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse) {return SOAP_OK;};

	/// Web service operation 'SetDPAddresses' (returns error code or SOAP_OK)
	virtual	int SetDPAddresses_(_tds__SetDPAddresses *tds__SetDPAddresses, _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse) {return SOAP_OK;};

	/// Web service operation 'GetHostname' (returns error code or SOAP_OK)
	virtual	int GetHostname_(_tds__GetHostname *tds__GetHostname, _tds__GetHostnameResponse *tds__GetHostnameResponse) {return SOAP_OK;};

	/// Web service operation 'SetHostname' (returns error code or SOAP_OK)
	virtual	int SetHostname_(_tds__SetHostname *tds__SetHostname, _tds__SetHostnameResponse *tds__SetHostnameResponse) {return SOAP_OK;};

	/// Web service operation 'SetHostnameFromDHCP' (returns error code or SOAP_OK)
	virtual	int SetHostnameFromDHCP_(_tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse) {return SOAP_OK;};

	/// Web service operation 'GetDNS' (returns error code or SOAP_OK)
	virtual	int GetDNS_(_tds__GetDNS *tds__GetDNS, _tds__GetDNSResponse *tds__GetDNSResponse) {return SOAP_OK;};

	/// Web service operation 'SetDNS' (returns error code or SOAP_OK)
	virtual	int SetDNS_(_tds__SetDNS *tds__SetDNS, _tds__SetDNSResponse *tds__SetDNSResponse) {return SOAP_OK;};

	/// Web service operation 'GetNTP' (returns error code or SOAP_OK)
	virtual	int GetNTP_(_tds__GetNTP *tds__GetNTP, _tds__GetNTPResponse *tds__GetNTPResponse) {return SOAP_OK;};

	/// Web service operation 'SetNTP' (returns error code or SOAP_OK)
	virtual	int SetNTP_(_tds__SetNTP *tds__SetNTP, _tds__SetNTPResponse *tds__SetNTPResponse) {return SOAP_OK;};

	/// Web service operation 'GetDynamicDNS' (returns error code or SOAP_OK)
	virtual	int GetDynamicDNS_(_tds__GetDynamicDNS *tds__GetDynamicDNS, _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse) {return SOAP_OK;};

	/// Web service operation 'SetDynamicDNS' (returns error code or SOAP_OK)
	virtual	int SetDynamicDNS_(_tds__SetDynamicDNS *tds__SetDynamicDNS, _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse) {return SOAP_OK;};

	/// Web service operation 'GetNetworkInterfaces' (returns error code or SOAP_OK)
	virtual	int GetNetworkInterfaces_(_tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse) {return SOAP_OK;};

	/// Web service operation 'SetNetworkInterfaces' (returns error code or SOAP_OK)
	virtual	int SetNetworkInterfaces_(_tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse) {return SOAP_OK;};

	/// Web service operation 'GetNetworkProtocols' (returns error code or SOAP_OK)
	virtual	int GetNetworkProtocols_(_tds__GetNetworkProtocols *tds__GetNetworkProtocols, _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse) {return SOAP_OK;};

	/// Web service operation 'SetNetworkProtocols' (returns error code or SOAP_OK)
	virtual	int SetNetworkProtocols_(_tds__SetNetworkProtocols *tds__SetNetworkProtocols, _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse) {return SOAP_OK;};

	/// Web service operation 'GetNetworkDefaultGateway' (returns error code or SOAP_OK)
	virtual	int GetNetworkDefaultGateway_(_tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse) {return SOAP_OK;};

	/// Web service operation 'SetNetworkDefaultGateway' (returns error code or SOAP_OK)
	virtual	int SetNetworkDefaultGateway_(_tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse) {return SOAP_OK;};

	/// Web service operation 'GetZeroConfiguration' (returns error code or SOAP_OK)
	virtual	int GetZeroConfiguration_(_tds__GetZeroConfiguration *tds__GetZeroConfiguration, _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'SetZeroConfiguration' (returns error code or SOAP_OK)
	virtual	int SetZeroConfiguration_(_tds__SetZeroConfiguration *tds__SetZeroConfiguration, _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int GetIPAddressFilter_(_tds__GetIPAddressFilter *tds__GetIPAddressFilter, _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'SetIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int SetIPAddressFilter_(_tds__SetIPAddressFilter *tds__SetIPAddressFilter, _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'AddIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int AddIPAddressFilter_(_tds__AddIPAddressFilter *tds__AddIPAddressFilter, _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'RemoveIPAddressFilter' (returns error code or SOAP_OK)
	virtual	int RemoveIPAddressFilter_(_tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse) {return SOAP_OK;};

	/// Web service operation 'GetAccessPolicy' (returns error code or SOAP_OK)
	virtual	int GetAccessPolicy_(_tds__GetAccessPolicy *tds__GetAccessPolicy, _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse) {return SOAP_OK;};

	/// Web service operation 'SetAccessPolicy' (returns error code or SOAP_OK)
	virtual	int SetAccessPolicy_(_tds__SetAccessPolicy *tds__SetAccessPolicy, _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse) {return SOAP_OK;};

	/// Web service operation 'CreateCertificate' (returns error code or SOAP_OK)
	virtual	int CreateCertificate_(_tds__CreateCertificate *tds__CreateCertificate, _tds__CreateCertificateResponse *tds__CreateCertificateResponse) {return SOAP_OK;};

	/// Web service operation 'GetCertificates' (returns error code or SOAP_OK)
	virtual	int GetCertificates_(_tds__GetCertificates *tds__GetCertificates, _tds__GetCertificatesResponse *tds__GetCertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'GetCertificatesStatus' (returns error code or SOAP_OK)
	virtual	int GetCertificatesStatus_(_tds__GetCertificatesStatus *tds__GetCertificatesStatus, _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse) {return SOAP_OK;};

	/// Web service operation 'SetCertificatesStatus' (returns error code or SOAP_OK)
	virtual	int SetCertificatesStatus_(_tds__SetCertificatesStatus *tds__SetCertificatesStatus, _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteCertificates' (returns error code or SOAP_OK)
	virtual	int DeleteCertificates_(_tds__DeleteCertificates *tds__DeleteCertificates, _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'GetPkcs10Request' (returns error code or SOAP_OK)
	virtual	int GetPkcs10Request_(_tds__GetPkcs10Request *tds__GetPkcs10Request, _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse) {return SOAP_OK;};

	/// Web service operation 'LoadCertificates' (returns error code or SOAP_OK)
	virtual	int LoadCertificates_(_tds__LoadCertificates *tds__LoadCertificates, _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'GetClientCertificateMode' (returns error code or SOAP_OK)
	virtual	int GetClientCertificateMode_(_tds__GetClientCertificateMode *tds__GetClientCertificateMode, _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse) {return SOAP_OK;};

	/// Web service operation 'SetClientCertificateMode' (returns error code or SOAP_OK)
	virtual	int SetClientCertificateMode_(_tds__SetClientCertificateMode *tds__SetClientCertificateMode, _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse) {return SOAP_OK;};

	/// Web service operation 'GetRelayOutputs' (returns error code or SOAP_OK)
	virtual	int GetRelayOutputs_(_tds__GetRelayOutputs *tds__GetRelayOutputs, _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse) {return SOAP_OK;};

	/// Web service operation 'SetRelayOutputSettings' (returns error code or SOAP_OK)
	virtual	int SetRelayOutputSettings_(_tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse) {return SOAP_OK;};

	/// Web service operation 'SetRelayOutputState' (returns error code or SOAP_OK)
	virtual	int SetRelayOutputState_(_tds__SetRelayOutputState *tds__SetRelayOutputState, _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse) {return SOAP_OK;};

	/// Web service operation 'SendAuxiliaryCommand' (returns error code or SOAP_OK)
	virtual	int SendAuxiliaryCommand_(_tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse) {return SOAP_OK;};

	/// Web service operation 'GetCACertificates' (returns error code or SOAP_OK)
	virtual	int GetCACertificates_(_tds__GetCACertificates *tds__GetCACertificates, _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'LoadCertificateWithPrivateKey' (returns error code or SOAP_OK)
	virtual	int LoadCertificateWithPrivateKey_(_tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse) {return SOAP_OK;};

	/// Web service operation 'GetCertificateInformation' (returns error code or SOAP_OK)
	virtual	int GetCertificateInformation_(_tds__GetCertificateInformation *tds__GetCertificateInformation, _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse) {return SOAP_OK;};

	/// Web service operation 'LoadCACertificates' (returns error code or SOAP_OK)
	virtual	int LoadCACertificates_(_tds__LoadCACertificates *tds__LoadCACertificates, _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse) {return SOAP_OK;};

	/// Web service operation 'CreateDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int CreateDot1XConfiguration_(_tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'SetDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int SetDot1XConfiguration_(_tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int GetDot1XConfiguration_(_tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot1XConfigurations' (returns error code or SOAP_OK)
	virtual	int GetDot1XConfigurations_(_tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse) {return SOAP_OK;};

	/// Web service operation 'DeleteDot1XConfiguration' (returns error code or SOAP_OK)
	virtual	int DeleteDot1XConfiguration_(_tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot11Capabilities' (returns error code or SOAP_OK)
	virtual	int GetDot11Capabilities_(_tds__GetDot11Capabilities *tds__GetDot11Capabilities, _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse) {return SOAP_OK;};

	/// Web service operation 'GetDot11Status' (returns error code or SOAP_OK)
	virtual	int GetDot11Status_(_tds__GetDot11Status *tds__GetDot11Status, _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse) {return SOAP_OK;};

	/// Web service operation 'ScanAvailableDot11Networks' (returns error code or SOAP_OK)
	virtual	int ScanAvailableDot11Networks_(_tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse) {return SOAP_OK;};

	/// Web service operation 'GetSystemUris' (returns error code or SOAP_OK)
	virtual	int GetSystemUris_(_tds__GetSystemUris *tds__GetSystemUris, _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse) {return SOAP_OK;};

	/// Web service operation 'StartFirmwareUpgrade' (returns error code or SOAP_OK)
	virtual	int StartFirmwareUpgrade_(_tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse) {return SOAP_OK;};

	/// Web service operation 'StartSystemRestore' (returns error code or SOAP_OK)
	virtual	int StartSystemRestore_(_tds__StartSystemRestore *tds__StartSystemRestore, _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse) {return SOAP_OK;};

	
public:
	struct soap * m_pSoap;
	string m_strDeviceEndpoint; 
	string m_strMediaEndpoint; 
	string m_strReceiverEndpoint;
	string m_strEventEndpoint;
	std::vector<std::string> m_strScopes;
};

inline OnvifServerDevice::OnvifServerDevice(string strDeviceEndpoint, 
		string strMediaEndpoint, string strReceiverEndpoint, string strEventEndpoint)
:DeviceBindingService(), m_pSoap(this), m_strDeviceEndpoint(strDeviceEndpoint), 
m_strMediaEndpoint(strMediaEndpoint), m_strReceiverEndpoint(strReceiverEndpoint), 
m_strEventEndpoint(strEventEndpoint)
{
	m_strScopes.push_back("onvif://www.onvif.org/name/OpenCVR");
	m_strScopes.push_back("onvif://www.onvif.org/hardware/OpenCVR");
	m_strScopes.push_back("onvif://www.onvif.org/location/Beijing");
	m_strScopes.push_back("onvif://www.onvif.org/Profile/Streaming");
	m_strScopes.push_back("onvif://www.onvif.org/Profile/Storage");
}

inline OnvifServerDevice::~OnvifServerDevice()
{

}

#endif