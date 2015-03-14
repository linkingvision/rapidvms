# $Id: Makefile.am,v 1.0.0-0 2011/03/03 13:28:14 Cnangel Exp $

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = autogen.sh \
			 libchardet.spec \
			 .gitignore \
			 $(NULL)

SUBDIRS = src \
		  pkgconfig \
		  $(NULL)

# install libchardet.pc
pcdatadir								= $(libdir)/pkgconfig
pcdata_DATA								= pkgconfig/@PACKAGE@.pc \
										  $(NULL)
# define rpm build root
RPMBUILDTOPDIR = $(shell unset RPMBUILDTOPDIR; if [ -f ${HOME}/.rpmmacros ]; then export RPMBUILDTOPDIR=`grep _topdir ${HOME}/.rpmmacros|awk '{print $$2}'`; else export RPMBUILDTOPDIR=/usr/src/redhat; fi; echo $${RPMBUILDTOPDIR};)

.PHONY: rpm

rpm: dist @PACKAGE@.spec
	$(RM) ${RPMBUILDTOPDIR}/SOURCES/$(distdir).tar.gz
	$(RM) ${RPMBUILDTOPDIR}/SPECS/@PACKAGE@.spec
	$(LN_S) `pwd`/$(distdir).tar.gz ${RPMBUILDTOPDIR}/SOURCES
	$(LN_S) `pwd`/@PACKAGE@.spec ${RPMBUILDTOPDIR}/SPECS
	rpmbuild -ba ${RPMBUILDTOPDIR}/SPECS/@PACKAGE@.spec

CLEANFILES = *~ *.log *.tar.gz 
