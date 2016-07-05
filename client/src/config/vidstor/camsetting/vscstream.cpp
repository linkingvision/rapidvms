typedef std::map<int, std::string> ProfilesMap;


	/* Update the param  */
	updateParamValue(ui.lineEditName, m_Param.m_Conf.data.conf.Name);

	updateParamValue(ui.lineEditIP, m_Param.m_Conf.data.conf.IP);
	updateParamValue(ui.lineEditPort, m_Param.m_Conf.data.conf.Port);
	updateParamValue(ui.lineEditUser, m_Param.m_Conf.data.conf.User);
	updateParamValue(ui.lineEditPassword, m_Param.m_Conf.data.conf.Password);

	updateParamValue(ui.lineOnvifAddr, m_Param.m_Conf.data.conf.OnvifAddress);
	
	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();

	astring IP = m_Param.m_Conf.data.conf.IP;
	astring Port = m_Param.m_Conf.data.conf.Port;
	astring User = m_Param.m_Conf.data.conf.User;
	astring Password = m_Param.m_Conf.data.conf.Password;
	astring OnvifAddress = m_Param.m_Conf.data.conf.OnvifAddress;

	astring OnvifDeviceService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
	astring urlSubStream = "rtsp://" + IP + ":" + "554" + "/";

	DeviceManagement *pDm = new DeviceManagement(OnvifDeviceService.c_str(), 
	                        User.c_str(), Password.c_str());

	if (pDm  == NULL)
	{
	    VDC_DEBUG( "%s new DeviceManagement error \n",__FUNCTION__);
	    goto end;
	}

	Capabilities * pMediaCap = pDm->getCapabilitiesMedia();
	if (pMediaCap == NULL)
	{
	    VDC_DEBUG( "%s getCapabilitiesMedia error \n",__FUNCTION__);
	    delete pDm;
	    goto end;
	}

	QCoreApplication::processEvents();

	MediaManagement *pMedia = new MediaManagement(pMediaCap->mediaXAddr(), 
	                            User.c_str(), Password.c_str());
	if (pMedia == NULL)
	{
	    VDC_DEBUG( "%s new MediaManagement error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    goto end;
	}

	QCoreApplication::processEvents();
	
	Profiles *pProfileS = pMedia->getProfiles();
	if (pProfileS == NULL)
	{
	    VDC_DEBUG( "%s new getProfiles error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    delete pMedia;
	    goto end;
	}

	QCoreApplication::processEvents();

	if (pProfileS != NULL && pProfileS->m_toKenPro.size() <= 0)
	{
	    VDC_DEBUG( "%s new getProfiles error \n",__FUNCTION__);
	    delete pDm;
	    delete pMediaCap;
	    delete pMedia;
	    goto end;
	}

	m_bUpdateProfiles = TRUE;

	for ( int i=0; i!=pProfileS->m_toKenPro.size(); ++i )
	{
		m_ProfilesMap[i] = pProfileS->m_toKenPro.at(i).toStdString();
		s8 profileDisplay[1024];
		sprintf(profileDisplay, "%s %dx%d %dfps %dbps", 
				pProfileS->m_encodingVec.at(i).toStdString().c_str(), pProfileS->m_widthVec.at(i), 
				pProfileS->m_heightVec.at(i), pProfileS->m_frameRateLimitVec.at(i), 
				pProfileS->m_bitrateLimitVec.at(i));

		m_ProfilesMapDisplay[i] = profileDisplay;
	}

	QCoreApplication::processEvents();

	delete pDm;
	delete pMediaCap;
	delete pMedia;
	delete pProfileS;

end:
	delete loading;

	/* update error, show a error message */
	if (m_bUpdateProfiles == FALSE)
	{
		QMessageBox msgBox(this);
		//Set text
		msgBox.setWindowTitle(tr("Warning"));
		msgBox.setText(tr("Update Profiles error ..."));
		    //Set predefined icon, icon is show on left side of text.
		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
		    //set inforative text
		//msgBox.setInformativeText("Just show infornation.");
		    //Add ok and cancel button.
		msgBox.setStandardButtons(QMessageBox::Ok);
		    //Set focus of ok button
		msgBox.setDefaultButton(QMessageBox::Ok);

		    //execute message box. method exec() return the button value of cliecke button
		int ret = msgBox.exec();

		return;
	}

	int mainStreamIndex = 0;
	int subStreamIndex = 0;
	{
		ProfilesMap::iterator it = m_ProfilesMap.begin(); 
		for(; it!=m_ProfilesMap.end(); ++it)
		{
			std::string strMainStream = m_Param.m_Conf.data.conf.OnvifProfileToken;
			std::string strSubStream = m_Param.m_Conf.data.conf.OnvifProfileToken2;
			if ((*it).second == strMainStream)
			{
				mainStreamIndex = (*it).first;
			}

			if ((*it).second == strSubStream)
			{
				subStreamIndex = (*it).first;
			}
		}
	}

	/* update with current profile */
	ProfilesMap::iterator it = m_ProfilesMapDisplay.begin(); 
	for(; it!=m_ProfilesMapDisplay.end(); ++it)
	{
		ui.comboBoxMain->insertItem((*it).first, (*it).second.c_str());
		ui.comboBoxSub->insertItem((*it).first, (*it).second.c_str());
	}

	ui.comboBoxMain->setCurrentIndex(mainStreamIndex);
	ui.comboBoxSub->setCurrentIndex(subStreamIndex);